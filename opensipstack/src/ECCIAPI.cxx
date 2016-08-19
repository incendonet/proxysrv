/*
 *
 * ECCIAPI.cxx
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
 * $Log: ECCIAPI.cxx,v $
 * Revision 1.18  2007/01/22 14:24:24  joegenbaclor
 * Removed debug PTRACE
 *
 * Revision 1.17  2007/01/19 01:16:49  joegenbaclor
 * Commented out some PTRACE
 *
 * Revision 1.16  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.15  2007/01/12 00:31:02  joegenbaclor
 * launcher related code
 *
 * Revision 1.14  2007/01/11 10:01:24  joegenbaclor
 * more launcher code
 *
 * Revision 1.13  2007/01/04 09:36:01  joegenbaclor
 * Added XML RPC Configuration Capability
 *
 * Revision 1.12  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.11  2006/10/12 05:22:42  joegenbaclor
 * Enabled ability to make encrypted outbound calls for OPAL
 *
 * Revision 1.10  2006/09/15 09:55:51  joegenbaclor
 * Uploaded initial classes for ThreadPool implementation.
 *
 * Revision 1.9  2006/08/11 09:30:51  joegenbaclor
 * Corrected several compile warnings in GCC
 *
 * Revision 1.8  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 * Revision 1.7  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.6  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.5  2006/06/06 14:21:07  joegenbaclor
 * Corrected #endif with trailing comments and changed #ifdef P_EXPAT to #if P_EXPAT
 *
 * Revision 1.4  2006/06/06 13:30:25  joegenbaclor
 * Initial upload of PIDF files - thanks Solegy Systems!
 *
 * Revision 1.3  2006/03/20 03:51:14  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.2  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "ECCIAPI.h"

#if ENABLE_ECCI
#if P_EXPAT

using namespace Tools;

#define new PNEW

BOOL 
IsPrivateNetwork( const PIPSocket::Address & address )
{
  /// RFC 1918 allotment for private networks
  /// 10.0.0.0        -   10.255.255.255  (10/8 prefix)
  /// 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
  /// 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
 
  if( address.Byte1() == 10 )
    return TRUE;

  if( address.Byte1() == 172 )
    if( address.Byte2() >= 16 && address.Byte2() <= 31 )
      return TRUE;
    
  
  if( address.Byte1() == 192 && address.Byte2() == 168 )
    return TRUE;
  
  return FALSE;
}

void RouteTable::InitTable()
{
	ClearTable();

	CreateTable();

	// Set default IP according to route table
	PIPSocket::Address defGW;


	PIPSocket::GetGatewayAddress(defGW);
	defAddr = GetLocalAddress(defGW);

  if( defAddr.AsString() == "127.0.0.1" )
  {
	  PIPSocket::GetGatewayAddress(defAddr);
  }
 

	m_IsNetworkBridge = FALSE;
  BOOL hasPrivateIp = FALSE;
  BOOL hasPublicIp = FALSE;

	for (PINDEX i = 0; i < m_RouteTable.GetSize(); i++ )
  {
    PIPSocket::Address dest = m_RouteTable[i].GetDestination();
    if( dest.AsString() != "127.0.0.1" )
    {
      if( !hasPublicIp )
        hasPublicIp = !IsPrivateNetwork( dest );

      if( !hasPrivateIp )
        hasPrivateIp = IsPrivateNetwork( dest );
    }


  }

  m_IsNetworkBridge = hasPublicIp && hasPrivateIp;

}

void RouteTable::ClearTable()
{
  m_RouteTable.RemoveAll();
}


PIPSocket::Address RouteTable::GetLocalAddress(const Address & addr) const
{
  PIPSocket::Address localAddress;

	Address address = addr;
  PINDEX i;

  for( i = 0; i < m_RouteTable.GetSize(); i++ )
  {
    if( m_RouteTable[i].Compare( &address ) )
    {
      localAddress = m_RouteTable[i].GetDestination();

      if( localAddress.AsString() == "127.0.0.1" )
        PIPSocket::GetGatewayAddress(localAddress);

      return localAddress;
    }
  }

  localAddress = defAddr;
  return localAddress; 
}

BOOL RouteTable::IsLocalAddress( const PString & address )
{
  Address addr = GetLocalAddress( address );

  return address == addr.AsString();
}

void RouteTable::CreateTable()
{
	InterfaceTable if_table;
	if (!PIPSocket::GetInterfaceTable(if_table)) {
		return;
	}
	PIPSocket::RouteTable r_table;
	if (!PIPSocket::GetRouteTable(r_table)) 
		return;

	int i = r_table.GetSize();
	
  for (PINDEX r = 0; r < i ; ++r) 
  {
		PIPSocket::RouteEntry & r_entry = r_table[r];
		if (r_entry.GetNetMask() != INADDR_ANY)
			m_RouteTable.Append( new vxRouteEntry(r_entry, if_table) );
	}
}

void RouteTable::CreateTable(const PString & nets)
{
	PStringArray networks(nets.Tokenise(" ,;\t", FALSE));
	int i = networks.GetSize();
	if (i > 0) 
  {
		for (PINDEX r = 0; r < i ; ++r)
      	m_RouteTable.Append( new vxRouteEntry(networks[r]) );
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void ParseNetwork(const PString & cfg, PIPSocket::Address & network, PIPSocket::Address & netmask)
{
	if (cfg *= "ALL") {
		network = netmask = INADDR_ANY;
		return;
	}
	PStringArray net = cfg.Tokenise("/", FALSE);
	if (net.GetSize() < 2) {
		netmask = (DWORD(~0));
	} else if (net[1].Find('.') == P_MAX_INDEX) {
		// CIDR notation
		DWORD n = (DWORD(~0) >> net[1].AsInteger());
		netmask = PIPSocket::Host2Net(~n);
	} else {
		// decimal dot notation
		netmask = net[1];
	}
	network = PIPSocket::Address(net[0]) & netmask; // normalize
}


// class RouteTable::vxRouteEntry
RouteTable::vxRouteEntry::vxRouteEntry(
	const PString & net
) : PIPSocket::RouteEntry(0)
{
	destination = net.Tokenise("/", FALSE)[0];
	ParseNetwork(net, network, net_mask);
}

RouteTable::vxRouteEntry::vxRouteEntry(
	const PIPSocket::RouteEntry & re,
	const InterfaceTable & it
) : PIPSocket::RouteEntry(re)
{
	PINDEX i;
	for (i = 0; i < it.GetSize(); ++i) {
		const Address & ip = it[i].GetAddress();
		if (Compare(&ip)) {
			destination = ip;
			return;
		}
	}
	for (i = 0; i < it.GetSize(); ++i)
		if (it[i].GetName() == interfaceName) {
			destination = it[i].GetAddress();
			return;
		}
}

PINLINE bool RouteTable::vxRouteEntry::Compare(const Address *ip) const
{
    return (*ip == destination) || ((*ip & net_mask) == network);
}

/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

HTTPServer::HTTPServer()
{
  m_IgnoreSignatures = FALSE;
  m_HTTPListeningSocket = NULL;
  m_HTTPThreads.DisallowDeleteObjects();
}


HTTPServer::~HTTPServer()
{
  ShutdownListener();
}



BOOL HTTPServer::OnStart()
{
  if (!Initialize())
    return FALSE;

  return TRUE;
}


void HTTPServer::OnStop()
{
  ShutdownListener();
}


BOOL HTTPServer::GetListenerAddress(
  PIPSocket::Address & address,
  WORD & port
)
{
  if(m_HTTPListeningSocket == NULL )
    return FALSE;
  PString hostName = ((PTCPSocket*)m_HTTPListeningSocket)->GetHostName();
  ///BOOL ok = ((PTCPSocket*)m_HTTPListeningSocket)->GetHostAddress( hostName, address );
  
  address = GetLocalAddress();
  port = ((PTCPSocket*)m_HTTPListeningSocket)->GetPort();

  return TRUE;
}

BOOL HTTPServer::ListenForHTTP(
  WORD port,
  PSocket::Reusability reuse,
  PINDEX stackSize
)
{
  if (m_HTTPListeningSocket != NULL &&
      m_HTTPListeningSocket->GetPort() == port &&
      m_HTTPListeningSocket->IsOpen())
    return TRUE;

  return ListenForHTTP(new PTCPSocket(port), reuse, stackSize);
}


BOOL HTTPServer::ListenForHTTP(
  PSocket * listener,
  PSocket::Reusability reuse,
  PINDEX stackSize
)
{
  if (m_HTTPListeningSocket != NULL)
    ShutdownListener();

  m_HTTPListeningSocket = PAssertNULL(listener);
  if (!m_HTTPListeningSocket->Listen(5, 0, reuse)) 
  {
    return FALSE;
  }

  if (stackSize > 1000)
    new HTTPServerThread(stackSize, *this);

  return TRUE;
}


void HTTPServer::ShutdownListener()
{
  if (m_HTTPListeningSocket == NULL)
    return;

  if (!m_HTTPListeningSocket->IsOpen())
    return;

  m_HTTPListeningSocket->Close();

  httpThreadsMutex.Wait();
  for (PINDEX i = 0; i < m_HTTPThreads.GetSize(); i++)
    m_HTTPThreads[i].Close();

  while (m_HTTPThreads.GetSize() > 0) 
  {
    httpThreadsMutex.Signal();
    PThread::Current()->Sleep( 1 );
    httpThreadsMutex.Wait();
  }

  httpThreadsMutex.Signal();

  delete m_HTTPListeningSocket;
  m_HTTPListeningSocket = NULL;
}




PTCPSocket * HTTPServer::AcceptHTTP()
{
  if (m_HTTPListeningSocket == NULL)
    return NULL;

  if (!m_HTTPListeningSocket->IsOpen())
    return NULL;

  // get a socket when a client connects
  PTCPSocket * socket = new PTCPSocket;
  if (socket->Accept(*m_HTTPListeningSocket))
    return socket;

  if (socket->GetErrorCode() != PChannel::Interrupted)
  {
    PTRACE(1, "Accept failed for HTTP: " << socket->GetErrorText());
  }

  if (m_HTTPListeningSocket != NULL && m_HTTPListeningSocket->IsOpen())
    return socket;

  delete socket;
  return NULL;
}


BOOL HTTPServer::ProcessHTTP(PTCPSocket & socket)
{
  if (!socket.IsOpen())
    return TRUE;

  PHTTPServer * server = CreateHTTPServer(socket);
  if (server == NULL) {
    PTRACE(1, "HTTP server creation/open failed.");
    return TRUE;
  }

  // process requests
  while (server->ProcessCommand())
    ;

  // always close after the response has been sent
  delete server;

  return TRUE;
}




PHTTPServer * HTTPServer::CreateHTTPServer(PTCPSocket & socket)
{
#ifdef SO_LINGER
  const linger ling = { 1, 5 };
  socket.SetOption(SO_LINGER, &ling, sizeof(ling));
#endif

  PHTTPServer * server = OnCreateHTTPServer(httpNameSpace);

  if (server->Open(socket))
    return server;

  delete server;
  return NULL;
}


PHTTPServer * HTTPServer::OnCreateHTTPServer(const PHTTPSpace & httpNameSpace)
{
  return new PHTTPServer(httpNameSpace);
}


//////////////////////////////////////////////////////////////

HTTPServerThread::HTTPServerThread(PINDEX stackSize,
                                       HTTPServer & app)
  : PThread(stackSize, AutoDeleteThread, NormalPriority, "HTTP Service:%x"),
    process(app)
{
  process.httpThreadsMutex.Wait();
  process.m_HTTPThreads.Append(this);
  process.httpThreadsMutex.Signal();

  ECCIStackSize = stackSize;
  socket = NULL;
  Resume();
}


HTTPServerThread::~HTTPServerThread()
{
  process.httpThreadsMutex.Wait();
  process.m_HTTPThreads.Remove(this);
  process.httpThreadsMutex.Signal();
  delete socket;
}


void HTTPServerThread::Close()
{
  if (socket != NULL)
    socket->Close();
}


void HTTPServerThread::Main()
{
  PTCPSocket * socket = process.AcceptHTTP();
  if (socket != NULL) {
    new HTTPServerThread(ECCIStackSize, process);
    process.ProcessHTTP(*socket);
  }
}


/////////////////////////////////////////////////////////////////////////////////////////


static const char NoIndentElements[] = "methodName name string int boolean double dateTime.iso8601";

XMLRPCRequest::XMLRPCRequest()
  : PXML(-1, NoIndentElements)
{
  faultCode = P_MAX_INDEX;
  SetRootElement("methodResponse");
  params = NULL;
}

XMLRPCRequest::XMLRPCRequest( const PXMLRPCBlock & xml )
  : PXML( xml )
{
  if (rootElement != NULL)
    params = rootElement->GetElement("params");
}

XMLRPCRequest & XMLRPCRequest::operator =( const PXMLRPCBlock & xml )
{
  PStringStream strm;
  xml.PrintOn( strm );
  Load( strm );

  if (rootElement != NULL)
    params = rootElement->GetElement("params");

  return *this;
}

PXMLElement * XMLRPCRequest::GetParams()
{
  if (params == NULL)
    params = rootElement->AddChild(new PXMLElement(rootElement, "params"));

  return params;
}

PXMLElement * XMLRPCRequest::CreateValueElement(PXMLElement * element) 
{ 
  PXMLElement * value = new PXMLElement(NULL, "value");
  value->AddChild(element);
  element->SetParent(value);

  return value;
}

PXMLElement * XMLRPCRequest::CreateScalar(const PString & type, 
                                         const PString & scalar)
{ 
  return CreateValueElement(new PXMLElement(NULL, type, scalar));
}

PXMLElement * XMLRPCRequest::CreateScalar(const PString & str) 
{ 
  return CreateScalar("string", str);
}

PXMLElement * XMLRPCRequest::CreateScalar(int value) 
{
  return CreateScalar("int", PString(PString::Unsigned, value)); 
}

PXMLElement * XMLRPCRequest::CreateScalar(double value)
{ 
  return CreateScalar("double", psprintf("%lf", value)); 
}

PXMLElement * XMLRPCRequest::CreateDateAndTime(const PTime & time)
{
  return CreateScalar("dateTime.iso8601", PXMLRPC::PTimeToISO8601(time)); 
}

PXMLElement * XMLRPCRequest::CreateBinary(const PBYTEArray & data)
{
  return CreateScalar("base64", PBase64::Encode(data)); 
}

PXMLElement * XMLRPCRequest::CreateStruct()
{
  PAssertAlways("Not used");
  return NULL;
}


PXMLElement * XMLRPCRequest::CreateStruct(const PStringToString & dict)
{
  return CreateStruct(dict, "string");
}

PXMLElement * XMLRPCRequest::CreateStruct(const PStringToString & dict, const PString & typeStr)
{
  PXMLElement * structElement = new PXMLElement(NULL, "struct");
  PXMLElement * valueElement  = CreateValueElement(structElement);

  PINDEX i;
  for (i = 0; i < dict.GetSize(); i++) {
    PString key = dict.GetKeyAt(i);
    structElement->AddChild(CreateMember(key, CreateScalar(typeStr, dict[key])));
  }

  return valueElement;
}


PXMLElement * XMLRPCRequest::CreateStruct(const PXMLRPCStructBase & data)
{
  PXMLElement * structElement = new PXMLElement(NULL, "struct");
  PXMLElement * valueElement  = XMLRPCRequest::CreateValueElement(structElement);

  PINDEX i;
  for (i = 0; i < data.GetNumVariables(); i++) {
    PXMLElement * element;
    PXMLRPCVariableBase & variable = data.GetVariable(i);

    if (variable.IsArray())
      element = CreateArray(variable);
    else {
      PXMLRPCStructBase * nested = variable.GetStruct(0);
      if (nested != NULL)
        element = CreateStruct(*nested);
      else
        element = CreateScalar(variable.GetType(), variable.ToString(0));
    }

    structElement->AddChild(CreateMember(variable.GetName(), element));
  }

  return valueElement;
}


PXMLElement * XMLRPCRequest::CreateMember(const PString & name, PXMLElement * value)
{
  PXMLElement * member = new PXMLElement(NULL, "member");
  member->AddChild(new PXMLElement(member, "name", name));
  member->AddChild(value);

  return member;
}


PXMLElement * XMLRPCRequest::CreateArray(const PStringArray & array)
{
  return CreateArray(array, "string");
}


PXMLElement * XMLRPCRequest::CreateArray(const PStringArray & array, const PString & typeStr)
{
  PXMLElement * arrayElement = new PXMLElement(NULL, "array");

  PXMLElement * dataElement = new PXMLElement(arrayElement, "data");
  arrayElement->AddChild(dataElement);

  PINDEX i;
  for (i = 0; i < array.GetSize(); i++)
    dataElement->AddChild(CreateScalar(typeStr, array[i]));

  return CreateValueElement(arrayElement);
}


PXMLElement * XMLRPCRequest::CreateArray(const PStringArray & array, const PStringArray & types)
{
  PXMLElement * arrayElement = new PXMLElement(NULL, "array");

  PXMLElement * dataElement = new PXMLElement(arrayElement, "data");
  arrayElement->AddChild(dataElement);

  PINDEX i;
  for (i = 0; i < array.GetSize(); i++)
    dataElement->AddChild(CreateScalar(types[i], array[i]));

  return CreateValueElement(arrayElement);
}


PXMLElement * XMLRPCRequest::CreateArray(const PArray<PStringToString> & array)
{
  PXMLElement * arrayElement = new PXMLElement(NULL, "array");

  PXMLElement * dataElement = new PXMLElement(arrayElement, "data");
  arrayElement->AddChild(dataElement);

  PINDEX i;
  for (i = 0; i < array.GetSize(); i++)
    dataElement->AddChild(CreateStruct(array[i]));

  return CreateValueElement(arrayElement);
}


PXMLElement * XMLRPCRequest::CreateArray(const PXMLRPCVariableBase & array)
{
  PXMLElement * arrayElement = new PXMLElement(NULL, "array");

  PXMLElement * dataElement = new PXMLElement(arrayElement, "data");
  arrayElement->AddChild(dataElement);

  PINDEX i;
  for (i = 0; i < array.GetSize(); i++) {
    PXMLElement * element;
    PXMLRPCStructBase * structure = array.GetStruct(i);
    if (structure != NULL)
      element = CreateStruct(*structure);
    else
      element = CreateScalar(array.GetType(), array.ToString(i));
    dataElement->AddChild(element);
  }

  return CreateValueElement(arrayElement);
}


/////////////////////////////////////////////

void XMLRPCRequest::AddParam(PXMLElement * parm)
{
  GetParams();
  PXMLElement * child = params->AddChild(new PXMLElement(params, "param"));
  child->AddChild(parm);
  parm->SetParent(child);
}

void XMLRPCRequest::AddParam(const PString & str) 
{ 
  AddParam(CreateScalar(str));
}

void XMLRPCRequest::AddParam(int value) 
{
  AddParam(CreateScalar(value)); 
}

void XMLRPCRequest::AddParam(double value)
{ 
  AddParam(CreateScalar(value)); 
}

void XMLRPCRequest::AddParam(const PTime & time)
{
  AddParam(CreateDateAndTime(time)); 
}

void XMLRPCRequest::AddBinary(const PBYTEArray & data)
{
  AddParam(CreateBinary(data)); 
}

void XMLRPCRequest::AddParam(const PXMLRPCStructBase & data)
{
  AddParam(CreateStruct(data));
}

void XMLRPCRequest::AddStruct(const PStringToString & dict)
{
  AddParam(CreateStruct(dict, "string"));
}

void XMLRPCRequest::AddStruct(const PStringToString & dict, const PString & typeStr)
{
  AddParam(CreateStruct(dict, typeStr));
}

void XMLRPCRequest::AddArray(const PStringArray & array)
{
  AddParam(CreateArray(array, "string"));
}

void XMLRPCRequest::AddArray(const PStringArray & array, const PString & typeStr)
{
  AddParam(CreateArray(array, typeStr));
}

void XMLRPCRequest::AddArray(const PStringArray & array, const PStringArray & types)
{
  AddParam(CreateArray(array, types));
}

void XMLRPCRequest::AddArray(const PArray<PStringToString> & array)
{
  AddParam(CreateArray(array));
}


/////////////////////////////////////////////

BOOL XMLRPCRequest::ValidateResponse()
{
  // ensure root element exists and has correct name
  if ((rootElement == NULL) || 
      (rootElement->GetName() != "methodResponse")) {
    SetFault(PXMLRPC::ResponseRootNotMethodResponse, "Response root not methodResponse");
    PTRACE(2, "XMLRPC\t" << GetFaultText());
    return FALSE;
  }

  // determine if response returned
  if (params == NULL)
    params = rootElement->GetElement("params");
  if (params == NULL)
    return TRUE;

  // determine if fault
  if (params->GetName() == "fault") {

    // assume fault is a simple struct
    PStringToString faultInfo;
    PXMLElement * value = params->GetElement("value");
    if (value == NULL) {
      PStringStream txt;
      txt << "Fault does not contain value\n" << *this;
      SetFault(PXMLRPC::FaultyFault, txt);
    } else if (!ParseStruct(value->GetElement("struct"), faultInfo) ||
         (faultInfo.GetSize() != 2) ||
         (!faultInfo.Contains("faultCode")) ||
         (!faultInfo.Contains("faultString"))
         ) {
      PStringStream txt;
      txt << "Fault return is faulty:\n" << *this;
      SetFault(PXMLRPC::FaultyFault, txt);
      PTRACE(2, "XMLRPC\t" << GetFaultText());
      return FALSE;
    }

    // get fault code and string
    SetFault(faultInfo["faultCode"].AsInteger(), faultInfo["faultString"]);

    return FALSE;
  }

  // must be params
  else if (params->GetName() != "params") {
    SetFault(PXMLRPC::ResponseUnknownFormat, PString("Response contains unknown element") & params->GetName());
    PTRACE(2, "XMLRPC\t" << GetFaultText());
    return FALSE;
  }

  return TRUE;
}

BOOL XMLRPCRequest::ParseScalar(PXMLElement * valueElement, 
                                   PString & type, 
                                   PString & value)
{
  if (valueElement == NULL)
    return FALSE;

  if (!valueElement->IsElement())
    return FALSE;

  if (valueElement->GetName() != "value") {
    SetFault(PXMLRPC::ParamNotValue, "Scalar value does not contain value element");
    PTRACE(2, "RPCXML\t" << GetFaultText());
    return FALSE;
  }

  for (PINDEX i = 0; i < valueElement->GetSize(); i++) {
    PXMLElement * element = (PXMLElement *)valueElement->GetElement(i);

    //PTRACE( 3, "Parsing element " << *element );
    if (element != NULL && element->IsElement()) {
      type = element->GetName();
      value = element->GetData();
      //PTRACE(1, "type: " << type << " value: " << value );
      return TRUE;
    }else if (element != NULL && !element->IsElement() && !element->GetName().IsEmpty() ) {
      type = "string";
      value = element->GetName();
      //PTRACE(1, "type: " << type << " value: " << value );
      return TRUE;
    }

  }


  //SetFault(PXMLRPC::ScalarWithoutElement, "Scalar without sub-element");
  //PTRACE(2, "XMLRPC\t" << GetFaultText());
  type = "string";
  return TRUE;



}


static BOOL ParseStructBase(XMLRPCRequest & block, PXMLElement * & element)
{
  if (element == NULL)
    return FALSE;

  if (!element->IsElement())
    return FALSE;

  if (element->GetName() == "struct")
    return TRUE;

  if (element->GetName() != "value")
    block.SetFault(PXMLRPC::ParamNotStruct, "Param is not struct");
  else {
    element = element->GetElement("struct");
    if (element != NULL)
      return TRUE;

    block.SetFault(PXMLRPC::ParamNotStruct, "nested structure not present");
  }

  PTRACE(2, "XMLRPC\t" << block.GetFaultText());
  return FALSE;
}


static PXMLElement * ParseStructElement(XMLRPCRequest & block,
                                        PXMLElement * structElement,
                                        PINDEX idx,
                                        PString & name)
{
  if (structElement == NULL)
    return NULL;

  PXMLElement * member = (PXMLElement *)structElement->GetElement(idx);
  if (member == NULL)
    return NULL;

  if (!member->IsElement())
    return NULL;

  if (member->GetName() != "member") {
    PStringStream txt;
    txt << "Member " << idx << " missing";
    block.SetFault(PXMLRPC::MemberIncomplete, txt);
    PTRACE(2, "XMLRPC\t" << block.GetFaultText());
    return NULL;
  }

  PXMLElement * nameElement  = member->GetElement("name");
  PXMLElement * valueElement = member->GetElement("value");
  if ((nameElement == NULL) || (valueElement == NULL)) {
    PStringStream txt;
    txt << "Member " << idx << " incomplete";
    block.SetFault(PXMLRPC::MemberIncomplete, txt);
    PTRACE(2, "XMLRPC\t" << block.GetFaultText());
    return NULL;
  }

  if (nameElement->GetName() != "name") {
    PStringStream txt;
    txt << "Member " << idx << " unnamed";
    block.SetFault(PXMLRPC::MemberUnnamed, txt);
    PTRACE(2, "XMLRPC\t" << block.GetFaultText());
    return NULL;
  }

  name = nameElement->GetData();
  return valueElement;
}


BOOL XMLRPCRequest::ParseStruct(PXMLElement * structElement, 
                               PStringToString & structDict)
{
  if (!ParseStructBase(*this, structElement))
    return FALSE;

  for (PINDEX i = 0; i < structElement->GetSize(); i++) {
    PString name;
    PXMLElement * element = ParseStructElement(*this, structElement, i, name);
    if (element != NULL) {
      PString value;
      PString type;
      if (ParseScalar(element, type, value))
        structDict.SetAt(name, value);
    }
  }

  structDict.MakeUnique();
  return TRUE;
}


BOOL XMLRPCRequest::ParseStruct(PXMLElement * structElement, PXMLRPCStructBase & data)
{
  if (!ParseStructBase(*this, structElement))
    return FALSE;

  for (PINDEX i = 0; i < structElement->GetSize(); i++) {
    PString name;
    PXMLElement * element = ParseStructElement(*this, structElement, i, name);
    if (element != NULL) {
      PXMLRPCVariableBase * variable = data.GetVariable(name);
      if (variable != NULL) {
        if (variable->IsArray()) {
          if (!ParseArray(element, *variable))
            return FALSE;
        }
        else {
          PXMLRPCStructBase * nested = variable->GetStruct(0);
          if (nested != NULL) {
            if (!ParseStruct(element, *nested))
              return FALSE;
          }
          else {
            PString value;
            PCaselessString type;
            if (!ParseScalar(element, type, value))
              return FALSE;

            if (type != "string" && type != variable->GetType()) {
              PTRACE(2, "RPCXML\tMember " << i << " is not of expected type: " << variable->GetType());
              return FALSE;
            }

            variable->FromString(0, value);
          }
        }
      }
    }
  }

  return TRUE;
}


static PXMLElement * ParseArrayBase(XMLRPCRequest & block, PXMLElement * element)
{
  if (element == NULL)
    return NULL;

  if (!element->IsElement())
    return NULL;

  if (element->GetName() == "value")
    element = element->GetElement("array");

  if (element == NULL)
    block.SetFault(PXMLRPC::ParamNotArray, "array not present");
  else {
    if (element->GetName() != "array")
      block.SetFault(PXMLRPC::ParamNotArray, "Param is not array");
    else {
      element = element->GetElement("data");
      if (element != NULL)
        return element;
      block.SetFault(PXMLRPC::ParamNotArray, "Array param has no data");
    }
  }

  PTRACE(2, "XMLRPC\t" << block.GetFaultText());
  return NULL;
}


BOOL XMLRPCRequest::ParseArray(PXMLElement * arrayElement, PStringArray & array)
{
  PXMLElement * dataElement = ParseArrayBase(*this, arrayElement);
  if (dataElement == NULL)
    return FALSE;

  array.SetSize(dataElement->GetSize());

  PINDEX count = 0;
  for (PINDEX i = 0; i < dataElement->GetSize(); i++) {
    PString value;
    PString type;
    if (ParseScalar((PXMLElement *)dataElement->GetElement(i), type, value))
      array[count++] = value;
  }

  array.SetSize(count);
  return TRUE;
}


BOOL XMLRPCRequest::ParseArray(PXMLElement * arrayElement, PArray<PStringToString> & array)
{
  PXMLElement * dataElement = ParseArrayBase(*this, arrayElement);
  if (dataElement == NULL)
    return FALSE;

  array.SetSize(dataElement->GetSize());

  PINDEX count = 0;
  for (PINDEX i = 0; i < dataElement->GetSize(); i++) {
    PStringToString * values = new PStringToString();
    if (!ParseStruct((PXMLElement *)dataElement->GetElement(i), *values))
      return FALSE;

    array.SetAt( count++, values );
  }

  return TRUE;
}


BOOL XMLRPCRequest::ParseArray(PXMLElement * arrayElement, PXMLRPCVariableBase & array)
{
  PXMLElement * dataElement = ParseArrayBase(*this, arrayElement);
  if (dataElement == NULL)
    return FALSE;

  array.SetSize(dataElement->GetSize());

  PINDEX count = 0;
  for (PINDEX i = 0; i < dataElement->GetSize(); i++) {
    PXMLElement * element = (PXMLElement *)dataElement->GetElement(i);

    PXMLRPCStructBase * structure = array.GetStruct(i);
    if (structure != NULL) {
      if (ParseStruct(element, *structure))
        count++;
    }
    else {
      PString value;
      PCaselessString type;
      if (ParseScalar(element, type, value)) {
        if (type != "string" && type != array.GetType())
          PTRACE(2, "RPCXML\tArray entry " << i << " is not of expected type: " << array.GetType());
        else
          array.FromString(count++, value);
      }
    }
  }

  array.SetSize(count);
  return TRUE;
}


PINDEX XMLRPCRequest::GetParamCount() const
{
  if (params == NULL) 
    return 0;

  PINDEX count = 0;
  for (PINDEX i = 0; i < params->GetSize(); i++) {
    PXMLElement * element = (PXMLElement *)params->GetElement(i);
    if (element != NULL && element->IsElement() && element->GetName() == "param")
      count++;
  }
  return count;
}


PXMLElement * XMLRPCRequest::GetParam(PINDEX idx) const 
{ 
  if (params == NULL) 
    return NULL;

  PXMLElement * param = NULL;
  PINDEX i;
  for (i = 0; i < params->GetSize(); i++) {
    PXMLElement * element = (PXMLElement *)params->GetElement(i);
    if (element != NULL && element->IsElement() && element->GetName() == "param") {
      if (idx <= 0) {
        param = element;
        break;
      }
      idx--;
    }
  }

  if (param == NULL)
    return FALSE;

  for (i = 0; i < param->GetSize(); i++) {
    PXMLObject * parm = param->GetElement(i);
    if (parm != NULL && parm->IsElement())
      return (PXMLElement *)parm;
  }

  return NULL;
}


BOOL XMLRPCRequest::GetParams(PXMLRPCStructBase & data)
{
  if (params == NULL) 
    return FALSE;

  // Special case to allow for server implementations that always return
  // values as a struct rather than multiple parameters.
  if (GetParamCount() == 1 &&
          (data.GetNumVariables() > 1 || data.GetVariable(0).GetStruct(0) == NULL)) {
    PString type, value;
    if (ParseScalar(GetParam(0), type, value) && type == "struct")
      return GetParam(0, data);
  }

  for (PINDEX i = 0; i < data.GetNumVariables(); i++) {
    PXMLRPCVariableBase & variable = data.GetVariable(i);
    if (variable.IsArray()) {
      if (!ParseArray(GetParam(i), variable))
        return FALSE;
    }
    else {
      PXMLRPCStructBase * structure = variable.GetStruct(0);
      if (structure != NULL) {
        if (!GetParam(i, *structure))
          return FALSE;
      }
      else {
        PString value;
        if (!GetExpectedParam(i, variable.GetType(), value))
          return FALSE;

        variable.FromString(0, value);
      }
    }
  }

  return TRUE;
}


BOOL XMLRPCRequest::GetParam(PINDEX idx, PString & type, PString & value)
{
  // get the parameter
  if (!ParseScalar(GetParam(idx), type, value)) {
    PTRACE(3, "XMLRPC\tCannot get scalar parm " << idx);
    return FALSE;
  }

  return TRUE;
}


BOOL XMLRPCRequest::GetExpectedParam(PINDEX idx, const PString & expectedType, PString & value)
{
  PString type;

  // get parameter
  if (!GetParam(idx, type, value))
    return FALSE;

  // see if correct type
  if (!expectedType.IsEmpty() && (type != expectedType)) {
    PTRACE(3, "XMLRPC\tExpected parm " << idx << " to be " << expectedType << ", was " << type);
    return FALSE;
  }

  return TRUE;
}


BOOL XMLRPCRequest::GetParam(PINDEX idx, PString & result)
{
  return GetExpectedParam(idx, "string", result); 
}

BOOL XMLRPCRequest::GetParam(PINDEX idx, int & val)
{
  PString type, result; 
  if (!GetParam(idx, type, result))
    return FALSE;

  if ((type != "i4") && 
      (type != "int") &&
      (type != "boolean")) {
    PTRACE(3, "XMLRPC\tExpected parm " << idx << " to be intger compatible, was " << type);
    return FALSE;
  }

  val = result.AsInteger();
  return TRUE;
}

BOOL XMLRPCRequest::GetParam(PINDEX idx, double & val)
{
  PString result; 
  if (!GetExpectedParam(idx, "double", result))
    return FALSE;

  val = result.AsReal();
  return TRUE;
}

// 01234567890123456
// yyyyMMddThh:mm:ss

BOOL XMLRPCRequest::GetParam(PINDEX idx, PTime & val, int tz)
{
  PString result; 
  if (!GetExpectedParam(idx, "dateTime.iso8601", result))
    return FALSE;

  return PXMLRPC::ISO8601ToPTime(result, val, tz);
}

BOOL XMLRPCRequest::GetParam(PINDEX idx, PStringArray & result)
{
  return ParseArray(GetParam(idx), result);
}

BOOL XMLRPCRequest::GetParam(PINDEX idx, PArray<PStringToString> & result)
{
  return ParseArray(GetParam(idx), result);
}


BOOL XMLRPCRequest::GetParam(PINDEX idx, PStringToString & result)
{
  return ParseStruct(GetParam(idx), result);
}


BOOL XMLRPCRequest::GetParam(PINDEX idx, PXMLRPCStructBase & data)
{
  return ParseStruct(GetParam(idx), data);
}


//////////////////////////////////////////////////////////////////////

const WORD DefaultHTTPPort = 8090;


XMLRPCServer::XMLRPCServer()
{
  m_XMLRPCPort = DefaultHTTPPort;
  m_StackSize = 1024 * 16;
}

XMLRPCServer::~XMLRPCServer()
{
}

BOOL XMLRPCServer::RegisterMethod(
  const PString & method,
  PNotifier notifier
)
{
  m_XMLRPCServer.SetMethod(method, notifier);
  return TRUE;
}

BOOL XMLRPCServer::Initialize()
{

  // note we do NOT use Overwrite
  httpNameSpace.AddResource(&m_XMLRPCServer);

  // set up the HTTP port for listening & start the first HTTP thread
  if (ListenForHTTP(m_XMLRPCPort, PSocket::CanReuseAddress, m_StackSize ))
    PTRACE(1, "Opened master socket for XML-RPC: " << m_HTTPListeningSocket->GetPort());
  else {
    PTRACE(1, "Cannot run without HTTP port: " << m_HTTPListeningSocket->GetErrorText());
    return FALSE;
  }
  return TRUE;
}

BOOL XMLRPCServer::ListenForXMLRPC( PINDEX stackSize )
{
  m_StackSize = stackSize;
  return OnStart();
}

BOOL XMLRPCServer::SendRequest(
  PXMLRPCBlock & request,
  const PURL & url,
  XMLRPCRequest & response
)
{
  PXMLRPC rpc(url);
  PXMLRPCBlock _response;
  BOOL ok = rpc.MakeRequest(request, _response);
  response = _response;
  return ok;
}


///////////////////////////////////////////////////////////////////////////////

XMLRPCExchange::XMLRPCExchange( 
  XMLRPCServer & server 
) : m_XMLRPCServer( server ) 
{
}

XMLRPCExchange::~XMLRPCExchange()
{
}

void XMLRPCExchange::RegisterMethod(
  const PString & method 
)
{
  m_XMLRPCServer.RegisterMethod( method, PCREATE_NOTIFIER( OnXMLRPCRequest ) );
}

BOOL XMLRPCExchange::SendRequest(
  const PString & method,
  PArray<PStringToString> & requestArg,
  PArray<PStringToString> & provisionalResponse,
  const PURL & url
)
{
  PXMLRPCBlock request(method);
  XMLRPCRequest response;

  PINDEX i = 0;
  PINDEX idx = 0;

  
  for( i = 0; i < requestArg.GetSize(); i++ )
    request.AddStruct( requestArg[i] );

  if( m_XMLRPCServer.SendRequest( request, url, response ) )
  {

    for ( i = 0; i < response.GetParamCount(); i++) 
    {
      PString type;
      PString val;
      if (response.GetParam(i, type, val)) 
      {
        if (type == "struct") 
        {
          PStringToString dict;
          response.GetParam(i, dict);
          provisionalResponse.SetAt( idx++, new PStringToString( dict ) );
        }else if (type == "array") 
        {
          PArray<PStringToString> array;
          response.GetParam(i, array);
          for( PINDEX j = 0; j < array.GetSize(); j++ )
          {
            provisionalResponse.SetAt( idx++, new PStringToString( array[j] ) );
          }
        }
      }
    }
    return TRUE;
  }

  return FALSE;
  
}


void XMLRPCExchange::OnXMLRPCRequest( PXMLRPCServerParms & params, INT )
{
  OnIncomingRequest( params );
}



void XMLRPCExchange::OnIncomingRequest( 
  PXMLRPCServerParms & params 
)
{
  XMLRPCRequest request( params.request );

  // make sure methodName is speciified
  PXMLElement * methodName = request.GetElement("methodName");

  /// we are assured that methodName is existing at this point
  PAssertNULL( methodName );

  PString method = ((PXMLData *)methodName->GetElement(0))->GetString();

  PArray<PStringToString>  args;

  PINDEX idx = 0;
  PINDEX i = 0;
  for( i = 0; i < request.GetParamCount(); i++) 
  {
    PStringToString dict;
    PString type;
    PString value;

    if (request.GetParam(i, dict))
    {
      args.SetAt( idx++, new PStringToString( dict ) );
    }else
    {
      PTRACE(2, "Cannot parse XMLRPC argument " << i);
    }
  }

  PArray<PStringToString> responseParams;
  OnIncomingRequest( method, args, responseParams );

  for( PINDEX i = 0; i < responseParams.GetSize(); i++ )
    params.response.AddStruct( responseParams[i] );
}

void XMLRPCExchange::OnIncomingRequest( 
  const PString & /*method*/,
  PArray<PStringToString> &, /*requestParams*/
  PArray<PStringToString> & /*responseParams*/
)
{
}


///////////////////////////////////////////////////////////////////////////

XMLRPCTransaction::XMLRPCTransaction(
  XMLRPCTransactionServer * server,
  Direction dir,
  const PString & method,
  const PString & transactionId,
  const PString & sessionId
) 
{
  m_TransactionServer = server;
  m_Direction = dir;
  m_Method = method;
  m_TransactionId = transactionId;
  m_SessionId = sessionId;
}

XMLRPCTransaction::~XMLRPCTransaction()
{
}

void XMLRPCTransaction::SetFinalResponse(
  const PArray<PStringToString> & response
)
{
  m_FinalResponse = response;
  m_FinalResponse.MakeUnique();
  
  if( m_Direction == Client )
  {
    m_FinalResponseEvent.Signal();
  }
}

/////////////////////////////////////////////////////////////////////////////////

XMLRPCEndpoint::XMLRPCEndpoint( 
  XMLRPCServer & server, 
  XMLRPCTransactionServer & transactionServer  
) : XMLRPCExchange( server ),
    m_TransactionServer( transactionServer )
{
}

void XMLRPCEndpoint::OnIncomingRequest( 
  const PString & method,
  PArray<PStringToString> & params,
  PArray<PStringToString> & response
)
{
  
  if( params.GetSize() > 0 )
  {
    m_TransactionServer.OnIncomingRequest( method, params, response );
  }else
  {
    //PTRACE( 1, "XML RPC Request has no parameters. method = " << method );
    PStringToString responseParams;
    responseParams.SetAt( "ResponseCode", "500" );
    responseParams.SetAt( "ResponseMessage", "Invalid Request Parameter" );
    response.SetAt( 0, new PStringToString( responseParams ) );
  }

  
}
/////////////////////////////////////////////////////////////////////////////

XMLRPCTransactionServer::XMLRPCTransactionServer()  : m_TransactionQueueSync( 0, INT_MAX )
{
  m_XMLRPCServer = NULL;
}

XMLRPCTransactionServer::XMLRPCTransactionServer(
  const PStringArray & methods,
  WORD port,
  PINDEX stackSize
) : m_TransactionQueueSync( 0, INT_MAX )
{
  Initialize( methods, port, stackSize );
}

XMLRPCTransactionServer::~XMLRPCTransactionServer()
{
}



BOOL XMLRPCTransactionServer::Initialize(
  const PStringArray & methods,
  WORD port,
  PINDEX stackSize
)
{
  if( m_XMLRPCServer != NULL )
    return FALSE;

  m_XMLRPCServer = new XMLRPCServer();
  m_XMLRPCServer->SetXMLRPCPort( port );
  m_XMLRPCServer->ListenForXMLRPC( stackSize );
  m_Exchange = new XMLRPCEndpoint( *m_XMLRPCServer, *this );

  for( PINDEX i = 0; i < methods.GetSize(); i++ )
    m_Exchange->RegisterMethod( methods[i] );

  return TRUE;
}

BOOL XMLRPCTransactionServer::RegisterMethod( 
  const PString & method 
)
{
  m_Exchange->RegisterMethod( method );
  return TRUE;
}

PString XMLRPCTransactionServer::GenTagParameter()
{
  PBYTEArray guidArray(4);

  // Want time of UTC in 0.1 microseconds since 15 Oct 1582.
  PInt64 TimeStamp;
  static PInt64 deltaTime = PInt64(10000000)*24*60*60*
                            (  16            // Days from 15th October
                             + 31            // Days in December 1583
                             + 30            // Days in November 1583
#ifdef _WIN32
                             + (1601-1583)*365   // Whole years
                             + (1601-1583)/4);   // Leap days

  // Get nanoseconds since 1601
#ifndef _WIN32_WCE
  GetSystemTimeAsFileTime((LPFILETIME)&TimeStamp);
#else
  SYSTEMTIME SystemTime;
  GetSystemTime(&SystemTime);
  SystemTimeToFileTime(&SystemTime, (LPFILETIME)&TimeStamp);
#endif // _WIN32_WCE

  TimeStamp /= 100;
#else // _WIN32
                             + (1970-1583)*365 // Days in years
                             + (1970-1583)/4   // Leap days
                             - 3);             // Allow for 1700, 1800, 1900 not leap years

#ifdef P_VXWORKS
  struct timespec ts;
  clock_gettime(0,&ts);
  TimeStamp = (ts.tv_sec*(PInt64)1000000 + ts.tv_nsec*1000)*10;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  TimeStamp = (tv.tv_sec*(PInt64)1000000 + tv.tv_usec)*10;
#endif // P_VXWORKS
#endif // _WIN32

  TimeStamp += deltaTime;

  static WORD clockSequence = (WORD)PRandom::Number();
  static PInt64 lastTimeStamp = 0;
  if (lastTimeStamp < TimeStamp)
    lastTimeStamp = TimeStamp;
  else
    clockSequence++;

  
  guidArray[0] = (BYTE)(TimeStamp&0xff);
  guidArray[1] = (BYTE)((TimeStamp>>8)&0xff);
  guidArray[2] = (BYTE)(((clockSequence>>8)&0x1f) | 0x80); // DCE compatible GUID
  guidArray[2] = (BYTE)clockSequence;


   ///generate it
   /*The branch ID inserted by an element compliant with this
    specification MUST always begin with the characters "z9hG4bK".  These
    7 characters are used as a magic cookie (7 is deemed sufficient to
    ensure that an older RFC 2543 implementation would not pick such a
    value), so that servers receiving the request can determine that the
    branch ID was constructed in the fashion described by this
   */

    static const PString magicCookie = "z9hG4bK";

  
  PStringStream strm;
  char fillchar = strm.fill();
  strm << magicCookie << hex << setfill('0')
       << setw(2) << (unsigned)(BYTE)guidArray[0]
       << setw(2) << (unsigned)(BYTE)guidArray[1]
       << setw(2) << (unsigned)(BYTE)guidArray[2]
       << setw(2) << (unsigned)(BYTE)guidArray[3]
       << dec << setfill(fillchar);

  return strm;
}

XMLRPCTransactionServer::ResponseCode XMLRPCTransactionServer::SendRequest(
  const PString & method,
  PArray<PStringToString>  & request,
  PArray<PStringToString>  & response,
  const PTimeInterval & timeOut, 
  const PURL & url,
  const PString & sid
)
{
  PString sessionId = sid.IsEmpty() ? GenTagParameter() : sid;
  PString transactionId = GenTagParameter();
  
  PStringStream id;
  id << sessionId << "-" << transactionId;
  
  XMLRPCTransaction * trn = new XMLRPCTransaction( 
    this, 
    XMLRPCTransaction::Client, 
    method, 
    transactionId, 
    sessionId );

  {
    PWaitAndSignal lock( m_TransactionsMutex );
    m_Transactions.SetAt( id, trn );
  }

  /// make the request
  if( request.GetSize() == 0 )
    request.SetAt( 0, new PStringToString() );

  PStringToString & params = request[0];
  params.SetAt( "SessionId", sessionId );
  params.SetAt( "Tag", transactionId );

  //PTRACE( 2, "XMLRPC Sending Request - " << method << " Tag=" << transactionId );


  PIPSocket::Address replyAddress;
  WORD replyPort;
  m_XMLRPCServer->GetListenerAddress( replyAddress, replyPort );
  params.SetAt( "ResponseAddress", replyAddress.AsString() );
  params.SetAt( "ResponsePort", PString( replyPort ) );

  PArray<PStringToString> provisionalResponseArray;

  ResponseCode prCode = SendRequest( method, request, provisionalResponseArray, url );

  if( prCode != Success )
  {
    //PTRACE( 2, "XMLRPC Sending Request FAILED!!! - " << method << " Tag=" << transactionId );

    return prCode;
  }

  if( !trn->WaitForFinalReponseEvent( timeOut ) )
    return FailureOnFinal;

  response = trn->GetFinalResponse();
  response.MakeUnique();

  {
    PWaitAndSignal lock( m_TransactionsMutex );
    m_Transactions.RemoveAt( id );
  }

  return Success;
}

XMLRPCTransactionServer::ResponseCode XMLRPCTransactionServer::SendRequest(
  const PString & method,
  PArray<PStringToString>  & request,
  PArray<PStringToString>  & provisionalResponse,
  const PURL & url
)
{
  PStringToString & cmdParams = request[0];

  if( cmdParams.GetAt( "ResponseAddress" ) == NULL )
  {
    PIPSocket::Address addr;
    WORD port;
    m_XMLRPCServer->GetListenerAddress( addr, port );
    cmdParams.SetAt( "ResponseAddress", addr.AsString() );
    if( cmdParams.GetAt( "ResponsePort" ) == NULL )
      cmdParams.SetAt( "ResponsePort", PString( port ) );
    if( cmdParams.GetAt( "Tag" ) == NULL )
      cmdParams.SetAt( "Tag", PString( PTimer::Tick().GetMilliSeconds() ) );
  }


  if( !m_Exchange->SendRequest( method, request, provisionalResponse, url ) )
    return FailureOnSend;

  ///check if provisional response contains an error
  if( provisionalResponse.GetSize() == 0 )
  {
    //PTRACE( 1, "Provisional Response is lacking response code" );
    //cerr << "Provisional Response is lacking response code" << endl;
    return FailureOnProvision;
  }

  PString * responseCode = provisionalResponse[0].GetAt( "ResponseCode" ) ;
  
  if( responseCode == NULL )
  {
    //PTRACE( 1, "Error On Provisional Response - " << endl << provisionalResponse[0] );
    return FailureOnProvision;
  }

  if( *responseCode != "0" )
  {
    //PTRACE( 1, "Error On Provisional Response - " << endl << provisionalResponse[0] );
    return FailureOnProvision;
  }

  return Success;
}

void XMLRPCTransactionServer::OnIncomingRequest(
  const PString & method,
  PArray<PStringToString>  & request,
  PArray<PStringToString>  & response
)
{

  PStringToString * responseParam = new PStringToString();
  if( request.GetSize() == 0 )
  {
    responseParam->SetAt( "ResponseCode", "999" );
    responseParam->SetAt( "ResponseMessage", "Missing Request Parameter" );
    response.SetAt(0, responseParam );
    return;
  }

  PString * tag = request[0].GetAt( "Tag" );
  if( tag != NULL )
  {
    //PTRACE( 2, "XMLRPC Incoming Request - " << method << " Tag=" << *tag );
  }

  //if( method == "call.acctstop" )
  //  PINDEX temp = 0;

  if( !FindTransactionAndSetResponse(
    method,
    request
  ) )
  {
    if( method.Find( ".ack" ) == P_MAX_INDEX )
    {
      /// this is a new request message  
      /// we should create a server transaction
      /// check if we have a tag
      PStringToString & headers = request[0];
      PString * sid = headers.GetAt( "SessionId" );
      PString * tag = headers.GetAt( "Tag" );
      PString * address = headers.GetAt( "ResponseAddress" );
      PString * port = headers.GetAt( "ResponsePort" );

      if( tag == NULL )
      {
        responseParam->SetAt( "ResponseCode", "999" );
        responseParam->SetAt( "ResponseMessage", "Missing Tag Parameter" );
        response.SetAt(0, responseParam );
        return;
      }

      if( address == NULL )
      {
        responseParam->SetAt( "ResponseCode", "999" );
        responseParam->SetAt( "ResponseMessage", "Missing ResponseAddress Parameter" );
        response.SetAt(0, responseParam );
        return;
      }

      if( port == NULL )
      {
        responseParam->SetAt( "ResponseCode", "999" );
        responseParam->SetAt( "ResponseMessage", "Missing ResponsePort Parameter" );
        response.SetAt(0, responseParam );
        return;
      }
      
      PStringStream url;
      url << "http://" << *address << ":" << *port;
      
      OnIncomingTransaction(
        PURL( url ),
        method, 
        sid == NULL ? PString::Empty() : *sid,
        *tag,
        request,
        *responseParam
      );

      response.SetAt(0, responseParam );

      return;

    }
  }

  responseParam->SetAt( "ResponseCode", "0" );
  responseParam->SetAt( "ResponseMessage", "Ok" );
  response.SetAt(0, responseParam );
}

void XMLRPCTransactionServer::OnIncomingTransaction(
  PURL responseURL,
  const PString & method,
  const PString & sessionId,
  const PString & tag,
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  XMLRPCTransaction * trn = new XMLRPCTransaction(
    this,
    XMLRPCTransaction::Server,
    method,
    tag,
    sessionId
  );

  trn->SetRequest( request );
  trn->SetResponseURL( responseURL );

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Queued" );

  PWaitAndSignal lock(m_TransactionQueueMutex);
  m_TransactionQueue.Enqueue( trn );
  m_TransactionQueueSync.Signal();
}

BOOL XMLRPCTransactionServer::FindTransactionAndSetResponse(
  const PString & method,
  PArray<PStringToString> & request 
)
{
  if( request.GetSize() == 0 )
    return FALSE;


  if( method.Find( ".ack" ) == P_MAX_INDEX )
  {
    return FALSE;
  }



  // generate the id
  PStringStream id;
  PString * sid = request[0].GetAt( "SessionId" );
  PString * tag = request[0].GetAt( "Tag" );

  if( sid == NULL || tag == NULL )
    return FALSE;

  id << *sid << "-" << *tag;

  //PTRACE( 2, "Finding XML RPC Transaction " << id );
  
  PWaitAndSignal lock( m_TransactionsMutex );

  XMLRPCTransaction * trn = m_Transactions.GetAt( id );

  if( trn == NULL )
    return FALSE;

  //PTRACE( 2, "XML RPC Transaction " << id << " FOUND" );

  trn->SetFinalResponse( request );

  return TRUE;
}


XMLRPCTransaction * XMLRPCTransactionServer::DequeueTransaction()
{
  m_TransactionQueueSync.Wait();
  PWaitAndSignal lock( m_TransactionQueueMutex );
  return m_TransactionQueue.Dequeue();
}

/////////////////////////////////////////////////////////////////////////



CallControlClient::XMLRPCProcessor::XMLRPCProcessor(
  CallControlClient & callControl
  ) : PThread( 1024 * 2, AutoDeleteThread ),
      m_CallControl( callControl )
{
  Resume();
}

void CallControlClient::XMLRPCProcessor::Main()
{
  for(;;)
    if( !m_CallControl.ProcessCCIEvents() )
      break;
}

/////////////////////////////////////////////////////////////
CallControlClient::CallControlClient()
{
}

CallControlClient::~CallControlClient()
{
}

BOOL CallControlClient::CCIInitialize(
  WORD listenerPort,
  const PString & serverURI
)
{

  m_ServerURL = serverURI;

  if( m_XMLRPCServer == NULL )
  {
    m_XMLRPCServer = new XMLRPCServer();
    m_XMLRPCServer->SetXMLRPCPort( listenerPort );
    m_XMLRPCServer->ListenForXMLRPC( 1024 * 2 );
    m_Exchange = new XMLRPCEndpoint( *m_XMLRPCServer, *this );

    /// commands we send to the server
    RegisterMethod( "CCI.SHUTDOWN.ack" );
    RegisterMethod( "CCI.CALL.ack" );
    RegisterMethod( "CCI.HANGUP.ack" );
    RegisterMethod( "CCI.BRIDGE.ack" );
    RegisterMethod( "CCI.ANSWER.ack" );
    RegisterMethod( "CCI.SET.ack" );
    RegisterMethod( "CCI.GET.ack" );
    RegisterMethod( "CCI.PLAY.ack" );
    RegisterMethod( "CCI.SPEAK.ack" );
    RegisterMethod( "CCI.RECORD.ack" );
    RegisterMethod( "CCI.RECORDSTOP.ack" );
    RegisterMethod( "CCI.HOLD.ack" );
    RegisterMethod( "CCI.UNHOLD.ack" );
    RegisterMethod( "CCI.EXTERNAL.ack" );
    
    /// events we receive from the server
    RegisterMethod( "CCI.EVENT.INCOMING" );
    RegisterMethod( "CCI.EVENT.PROCEED" );
    RegisterMethod( "CCI.EVENT.CONNECT" );
    RegisterMethod( "CCI.EVENT.MEDIA" );
    RegisterMethod( "CCI.EVENT.DTMF" );
    RegisterMethod( "CCI.EVENT.RTP" );
    RegisterMethod( "CCI.EVENT.HANGUP" );
    RegisterMethod( "CCI.EVENT.PLAY" );
    RegisterMethod( "CCI.EVENT.SPEAK" );
    RegisterMethod( "CCI.EVENT.RECORD" );
    RegisterMethod( "CCI.RECORDSTOP" );
    RegisterMethod( "CCI.EVENT.HOLD" );
    RegisterMethod( "CCI.EVENT.UNHOLD" );
    m_XMLRPCProcessor = new XMLRPCProcessor( *this );
  }

  return TRUE;
}

BOOL CallControlClient::ProcessCCIEvents()
{
  XMLRPCTransaction * trn = DequeueTransaction();
  
  if( trn == NULL )
    return FALSE;
  PArray<PStringToString> requestParams = trn->GetRequest();

  PString method = trn->GetMethod();
  PStringToString params = requestParams[0];
  
  //PString * sid = params.GetAt( "SessionId" );
  //PString * tag = params.GetAt( "Tag" );
  //PString * address = params.GetAt( "ReplyAddress" );
  //PString * port = params.GetAt( "ReplyPort" );

  if( method *= "CCI.SHUTDOWN.ack" )
  {
  }else if( method *= "CCI.CALL.ack" )
  {
  }else if( method *= "CCI.HANGUP.ack" )
  {
  }else if( method *= "CCI.BRIDGE.ack" )
  {
  }else if( method *= "CCI.ANSWER.ack" )
  {
  }else if( method *= "CCI.SET.ack" )
  {
  }else if( method *= "CCI.GET.ack" )
  {
  }else if( method *= "CCI.PLAY.ack" )
  {
  }else if( method *= "CCI.SPEAK.ack" )
  {
  }else if( method *= "CCI.RECORD.ack" )
  {
  }else if( method *= "CCI.RECORDSTOP.ack" )
  {
  }else if( method *= "CCI.HOLD.ack" )
  {
  }else if( method *= "CCI.UNHOLD.ack" )
  {
  }else if( method *= "CCI.EXTERNAL.ack" )
  {
  }else if( method *= "CCI.EVENT.INCOMING" )
  {
  }else if( method *= "CCI.EVENT.PROCEED" )
  {
  }else if( method *= "CCI.EVENT.CONNECT" )
  {
  }else if( method *= "CCI.EVENT.MEDIA" )
  {
  }else if( method *= "CCI.EVENT.DTMF" )
  {
  }else if( method *= "CCI.EVENT.RTP" )
  {
  }else if( method *= "CCI.EVENT.HANGUP" )
  {
  }else if( method *= "CCI.EVENT.PLAY" )
  {
  }else if( method *= "CCI.EVENT.SPEAK" )
  {
  }else if( method *= "CCI.EVENT.RECORD" )
  {
  }else
  {
  }

  delete trn;

  return TRUE;
}

BOOL CallControlClient::MakeCall(
  const PString & uri
)
{
    PArray<PStringToString>  cmd;
    PArray<PStringToString>  response;
    PStringToString * cmdParams = new PStringToString();
    cmd.SetAt( 0, cmdParams );
    cmdParams->SetAt( "RouteURI", uri );
    return SendRequest( "CCI.CALL", cmd, response, 5000, m_ServerURL ) == XMLRPCTransactionServer::Success;
}
#endif //ENABLE_ECCI
#endif // P_EXPAT


