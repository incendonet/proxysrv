/*
 *
 * BambooDHT.cxx
 * 
 * Open SIP Stack ( OSS )
 *
 * Copyright (C) 2006 Joegen E. Baclor. All Rights Reserved.
 *
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is OpenSIPStack Library.
 *
 * The Initial Developer of the Original Code is Joegen E. Baclor.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * $Log: BambooDHT.cxx,v $
 * Revision 1.3  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/08/03 08:23:34  joegenbaclor
 * added casting of m_TTL.GetSeconds() to int when used in AddParam()
 *
 * Revision 1.1  2006/08/01 03:55:42  joegenbaclor
 * 1.  Completed Initial classes for IVR Prompt channel
 * 2.  Added support for Bamboo Dynamic Hash Table for Network Overlay support
 *
 *
 */

#include "BambooDHT.h"

#if ENABLE_BAMBOO
#include <ptclib/cypher.h>
#include "SIPTransport.h"
using namespace SIPTransports;

#define new PNEW

using namespace DHT;

#define BAMBOO_KEY_MAX 20
#define BAMBOO_VALUE_MAX 1024
#define BAMBOO_TTL_MAX 604800
#define BAMBOO_LOOKUP_SERVER "http:opendht.nyuld.net"

BambooDHT::BambooDHT( 
  const PString & appName,
  const PString & key,
  const PBYTEArray & value,
  const PTimeInterval & ttl,
  const PString & secret,
  const PURL & bambooServer
)
{
  m_Key = key;
  m_Value = value;
  m_Application = appName;
  m_Secret = secret;
  m_TTL = ttl;
  m_BambooServer = bambooServer;
}

BambooDHT::~BambooDHT()
{
}

BambooDHT::PutResult BambooDHT::Put()
{
  PWaitAndSignal lock( m_Lock );
  PBYTEArray key;
  PString keyBuff = m_Key.Left( BAMBOO_KEY_MAX );
  PINDEX i;
  for( i = 0; i < keyBuff.GetLength(); i++ )
    key[i] = (BYTE)keyBuff[i];

  PXMLRPCBlock request( "put" ), response;
  request.AddBinary( key );
  request.AddBinary( m_Value );
  request.AddParam( (int)m_TTL.GetSeconds() );
  request.AddParam( m_Application );

  PXMLRPC rpc( m_BambooServer );
  if( !rpc.MakeRequest( request, response ) )
    return BambooDHT::TryAgain;

  int result;
  response.GetParam( 0, result );
  return (BambooDHT::PutResult)result;
}

BambooDHT::PutResult BambooDHT::PutRemovable()
{
  PWaitAndSignal lock( m_Lock );
  PBYTEArray key, value;
  PString keyBuff = m_Key.Left( BAMBOO_KEY_MAX );
  PINDEX i;
  for( i = 0; i < keyBuff.GetLength(); i++ )
    key[i] = (BYTE)keyBuff[i];

  
  PXMLRPCBlock request( "put_removable" ), response;
  request.AddBinary( key );
  request.AddBinary( value );

  PString keyType;
  PBYTEArray digest;

#if P_SSL
  PMessageDigestSHA1 sha;
  PMessageDigest::Result sha1;
  sha.Encode( m_Secret, sha1 );
  keyType = "SHA";
  digest = PBYTEArray( sha1.GetPointer(), sha1.GetSize() );
#endif

  request.AddParam( keyType );
  request.AddBinary( digest );
  request.AddParam( (int)m_TTL.GetSeconds() );
  request.AddParam( m_Application );

  PXMLRPC rpc( m_BambooServer );
  if( !rpc.MakeRequest( request, response ) )
    return BambooDHT::TryAgain;

  int result;
  response.GetParam( 0, result );
  return (BambooDHT::PutResult)result;
}

BOOL BambooDHT::Get( 
  PBYTEArray & value,
  PBYTEArray & newIter,
  const PBYTEArray & iter
  
)
{
  PWaitAndSignal lock( m_Lock );
  PBYTEArray key;
  PString keyBuff = m_Key.Left( BAMBOO_KEY_MAX );
  PINDEX i;
  for( i = 0; i < keyBuff.GetLength(); i++ )
    key[i] = (BYTE)keyBuff[i];

  PXMLRPCBlock request( "get" ), response;
  request.AddBinary( key );
  request.AddParam( 1 );
  request.AddBinary( iter );
  request.AddParam( m_Application );

  PXMLRPC rpc( m_BambooServer );
  if( !rpc.MakeRequest( request, response ) )
    return BambooDHT::TryAgain;
  
  PXMLElement * params = response.GetParam( 0 );

  if( params == NULL )
    return BambooDHT::TryAgain;

  if( params->GetElement( "array" ) != NULL )
  {
      if( params->GetElement( "array" )->GetElement( "data" ) != NULL )
      {
        BOOL ok = FALSE;
        PXMLElement * data = params->GetElement( "array" )->GetElement( "data" );
        if( data->GetElement( "value" ) != NULL )
        {
          if( data->GetElement( "value" )->GetElement( "array" ) != NULL )
            if( data->GetElement( "value" )->GetElement( "array" )->GetElement( "data" ) != NULL )
              if( data->GetElement( "value" )->GetElement( "array" )->GetElement( "data" )->GetElement( "value" ) != NULL )
                if( data->GetElement( "value" )->GetElement( "array" )->GetElement( "data" )->GetElement( "value" )->GetElement( "base64" ) != NULL )
                  
              {
                PString val = PBase64::Decode( data->GetElement( "value" )->GetElement( "array" )->GetElement( "data" )->GetElement( "value" )->GetElement( "base64" )->GetData() );
                value = PBYTEArray( (BYTE*)val.GetPointer(), val.GetLength() );
                ok = TRUE;
              }
        }

        if( data->GetElement( "value", 1 ) != NULL )
        {
          if( data->GetElement( "value", 1 )->GetElement( "base64" ) != NULL )
          {
            PString iterVal = data->GetElement( "value", 1 )->GetElement( "base64" )->GetData();
            newIter = PBYTEArray( (BYTE*)iterVal.GetPointer(), iterVal.GetLength() );
          }
        }

        return ok;
      }
  }

  return FALSE;
}

BOOL BambooDHT::Remove()
{
  PWaitAndSignal lock( m_Lock );
  return TRUE;
}

PString BambooDHT::GetNearestServer()
{
  static SIPURI srv( BAMBOO_LOOKUP_SERVER );
  PIPSocket::Address addr;
  WORD port;
  SIPTransport::Resolve( srv, addr, port );
  return addr.AsString();
}

#endif


