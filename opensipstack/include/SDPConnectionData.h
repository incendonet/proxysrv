/*
 *
 * SDPConnectionData.h
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
 * $Log: SDPConnectionData.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPCONNECTIONDATA_H
#define SDPCONNECTIONDATA_H

#include "SDPHeader.h"

namespace SDP
{
  class SDPConnectionData : public SDPHeader
  {
  PCLASSINFO( SDPConnectionData, SDPHeader );
  public:

    SDPConnectionData();

    SDPConnectionData( 
      const SDPHeader & header
    );

    SDPConnectionData(
      const PString & header
    );

    SDPConnectionData(
      const PString & name,
      const PString & value
    );

    PObject * Clone()const;



   /*
   Connection Data

   c=<network type> <address type> <connection address>

   The "c=" field contains connection data.

   A session announcement must contain one "c=" field in each media
   description (see below) or a "c=" field at the session-level.  It may
   contain a session-level "c=" field and one additional "c=" field per
   media description, in which case the per-media values override the
   session-level settings for the relevant media.

   The first sub-field is the network type, which is a text string
   giving the type of network.  Initially "IN" is defined to have the
   meaning "Internet".

   The second sub-field is the address type.  This allows SDP to be used
   for sessions that are not IP based.  Currently only IP4 is defined.

   The third sub-field is the connection address.  Optional extra
   subfields may be added after the connection address depending on the
   value of the <address type> field.

   For IP4 addresses, the connection address is defined as follows:

   o Typically the connection address will be a class-D IP multicast

     group address.  If the session is not multicast, then the
     connection address contains the fully-qualified domain name or the
     unicast IP address of the expected data source or data relay or
     data sink as determined by additional attribute fields. It is not
     expected that fully-qualified domain names or unicast addresses



Handley & Jacobson          Standards Track                    [Page 12]

RFC 2327                          SDP                         April 1998


     will be given in a session description that is communicated by a
     multicast announcement, though this is not prohibited.  If a
     unicast data stream is to pass through a network address
     translator, the use of a fully-qualified domain name rather than an
     unicast IP address is RECOMMENDED.  In other cases, the use of an
     IP address to specify a particular interface on a multi-homed host
     might be required.  Thus this specification leaves the decision as
     to which to use up to the individual application, but all
     applications MUST be able to cope with receiving both formats.

   o Conferences using an IP multicast connection address must also have
     a time to live (TTL) value present in addition to the multicast
     address.  The TTL and the address together define the scope with
     which multicast packets sent in this conference will be sent. TTL
     values must be in the range 0-255.

     The TTL for the session is appended to the address using a slash as
     a separator.  An example is:

                           c=IN IP4 224.2.1.1/127

     Hierarchical or layered encoding schemes are data streams where the
     encoding from a single media source is split into a number of
     layers.  The receiver can choose the desired quality (and hence
     bandwidth) by only subscribing to a subset of these layers.  Such
     layered encodings are normally transmitted in multiple multicast
     groups to allow multicast pruning.  This technique keeps unwanted
     traffic from sites only requiring certain levels of the hierarchy.
     For applications requiring multiple multicast groups, we allow the
     following notation to be used for the connection address:

            <base multicast address>/<ttl>/<number of addresses>

     If the number of addresses is not given it is assumed to be one.
     Multicast addresses so assigned are contiguously allocated above
     the base address, so that, for example:

                          c=IN IP4 224.2.1.1/127/3

     would state that addresses 224.2.1.1, 224.2.1.2 and 224.2.1.3 are
     to be used at a ttl of 127.  This is semantically identical to
     including multiple "c=" lines in a media description:

                           c=IN IP4 224.2.1.1/127
                           c=IN IP4 224.2.1.2/127
                           c=IN IP4 224.2.1.3/127





Handley & Jacobson          Standards Track                    [Page 13]

RFC 2327                          SDP                         April 1998


     Multiple addresses or "c=" lines can only be specified on a per-
     media basis, and not for a session-level "c=" field.

     It is illegal for the slash notation described above to be used for
     IP unicast addresses.

   */

    SDPConnectionData(
      const SDPConnectionData & header
    );

    SDPConnectionData(
      const PString & networkType,
      const PString & addressType,
      const PString & address
    );

    SDPConnectionData & operator=( const PString & header );

    SDPConnectionData & operator=( const SDPConnectionData & header );

    virtual void PrintOn( 
      ostream & strm 
    )const;

    void SetNetworkType(
      const PString & value
    );

    void SetAddressType(
      const PString & value
    );

    void SetAddress(
      const PString & value
    );
    
    PINLINE const PString & GetNetworkType()const{ return m_NetworkType; };
    PINLINE const PString & GetAddressType()const{ return m_AddressType; };
    PINLINE const PString & GetAddress()const{ return m_Address; };

  protected:
    PString m_NetworkType;
    PString m_AddressType;
    PString m_Address;

  };
};


#endif

