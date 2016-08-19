/*
 *
 * SIPNATMethods.cxx
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
 * $Log: SIPNATMethods.cxx,v $
 * Revision 1.3  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/04/07 13:50:17  joegenbaclor
 * updated autoconf files in linux
 *
 * Revision 1.1  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 *
 */



#include <ptlib.h>
#include <ptclib/random.h>
#include "SIPNATMethods.h"


/// The STUN Client in this code set is directly based from PSTUNClient found in PWLIB.
/// Due to its use of internal classes it is impossible to just derive from it and modify its behavior
/// See: http://www.openh323.org/pipermail/openh323/Week-of-Mon-20060327/subject.html#76790

using namespace NAT;


// Sample server is at larry.gloo.net

#define DEFAULT_REPLY_TIMEOUT 1000
#define DEFAULT_POLL_RETRIES  5
#define DEFAULT_NUM_SOCKETS_FOR_PAIRING 4

#define new PNEW

///////////////////////////////////////////////////////////////////////

SIPSTUNClient::SIPSTUNClient(const PString & server,
                         WORD portBase, WORD portMax,
                         WORD portPairBase, WORD portPairMax)
  : serverAddress(0),
    serverPort(DefaultPort),
    replyTimeout(DEFAULT_REPLY_TIMEOUT),
    pollRetries(DEFAULT_POLL_RETRIES),
    numSocketsForPairing(DEFAULT_NUM_SOCKETS_FOR_PAIRING),
    natType(UnknownNat),
    cachedExternalAddress(0),
    timeAddressObtained(0)
{
  SetServer(server);
  SetPortRanges(portBase, portMax, portPairBase, portPairMax);
}


SIPSTUNClient::SIPSTUNClient(const PIPSocket::Address & address, WORD port,
                         WORD portBase, WORD portMax,
                         WORD portPairBase, WORD portPairMax)
  : serverAddress(address),
    serverPort(port),
    replyTimeout(DEFAULT_REPLY_TIMEOUT),
    pollRetries(DEFAULT_POLL_RETRIES),
    numSocketsForPairing(DEFAULT_NUM_SOCKETS_FOR_PAIRING),
    natType(UnknownNat),
    cachedExternalAddress(0),
    timeAddressObtained(0)
{
  SetPortRanges(portBase, portMax, portPairBase, portPairMax);
}


PString SIPSTUNClient::GetServer() const
{
  PStringStream str;
  str << serverAddress << ':' << serverPort;
  return str;
}


BOOL SIPSTUNClient::SetServer(const PString & server)
{
  PINDEX colon = server.Find(':');
  if (colon == P_MAX_INDEX) {
    if (!PIPSocket::GetHostAddress(server, serverAddress))
      return FALSE;
  }
  else {
    if (!PIPSocket::GetHostAddress(server.Left(colon), serverAddress))
      return FALSE;
    serverPort = PIPSocket::GetPortByService("udp", server.Mid(colon+1));
  }

  return serverAddress.IsValid() && serverPort != 0;
}


BOOL SIPSTUNClient::SetServer(const PIPSocket::Address & address, WORD port)
{
  serverAddress = address;
  serverPort = port;
  return serverAddress.IsValid() && serverPort != 0;
}

#pragma pack(1)

struct SIPSTUNAttribute
{
  enum Types {
    MAPPED_ADDRESS = 0x0001,
    RESPONSE_ADDRESS = 0x0002,
    CHANGE_REQUEST = 0x0003,
    SOURCE_ADDRESS = 0x0004,
    CHANGED_ADDRESS = 0x0005,
    USERNAME = 0x0006,
    PASSWORD = 0x0007,
    MESSAGE_INTEGRITY = 0x0008,
    ERROR_CODE = 0x0009,
    UNKNOWN_ATTRIBUTES = 0x000a,
    REFLECTED_FROM = 0x000b,
  };
  
  PUInt16b type;
  PUInt16b length;
  
  SIPSTUNAttribute * GetNext() const { return (SIPSTUNAttribute *)(((const BYTE *)this)+length+4); }
};

class SIPSTUNAddressAttribute : public SIPSTUNAttribute
{
public:
  BYTE     pad;
  BYTE     family;
  PUInt16b port;
  BYTE     ip[4];

  PIPSocket::Address GetIP() const { return PIPSocket::Address(4, ip); }

protected:
  enum { SizeofAddressAttribute = sizeof(BYTE)+sizeof(BYTE)+sizeof(WORD)+sizeof(PIPSocket::Address) };
  void InitAddrAttr(Types newType)
  {
    type = (WORD)newType;
    length = SizeofAddressAttribute;
    pad = 0;
    family = 1;
  }
  bool IsValidAddrAttr(Types checkType) const
  {
    return type == checkType && length == SizeofAddressAttribute;
  }
};

class SIPSTUNMappedAddress : public SIPSTUNAddressAttribute
{
public:
  void Initialise() { InitAddrAttr(MAPPED_ADDRESS); }
  bool IsValid() const { return IsValidAddrAttr(MAPPED_ADDRESS); }
};

class SIPSTUNChangedAddress : public SIPSTUNAddressAttribute
{
public:
  void Initialise() { InitAddrAttr(CHANGED_ADDRESS); }
  bool IsValid() const { return IsValidAddrAttr(CHANGED_ADDRESS); }
};

class SIPSTUNChangeRequest : public SIPSTUNAttribute
{
public:
  BYTE flags[4];
  
  SIPSTUNChangeRequest() { }

  SIPSTUNChangeRequest(bool changeIP, bool changePort)
  {
    Initialise();
    SetChangeIP(changeIP);
    SetChangePort(changePort);
  }

  void Initialise()
  {
    type = CHANGE_REQUEST;
    length = sizeof(flags);
    memset(flags, 0, sizeof(flags));
  }
  bool IsValid() const { return type == CHANGE_REQUEST && length == sizeof(flags); }
  
  bool GetChangeIP() const { return (flags[3]&4) != 0; }
  void SetChangeIP(bool on) { if (on) flags[3] |= 4; else flags[3] &= ~4; }
  
  bool GetChangePort() const { return (flags[3]&2) != 0; }
  void SetChangePort(bool on) { if (on) flags[3] |= 2; else flags[3] &= ~2; }
};

class SIPSTUNMessageIntegrity : public SIPSTUNAttribute
{
public:
  BYTE hmac[20];
  
  void Initialise()
  {
    type = MESSAGE_INTEGRITY;
    length = sizeof(hmac);
    memset(hmac, 0, sizeof(hmac));
  }
  bool IsValid() const { return type == MESSAGE_INTEGRITY && length == sizeof(hmac); }
};

struct SIPSTUNMessageHeader
{
  PUInt16b       msgType;
  PUInt16b       msgLength;
  BYTE           transactionId[16];
};


#pragma pack()


class SIPSTUNMessage : public PBYTEArray
{
public:
  enum MsgType {
    BindingRequest  = 0x0001,
    BindingResponse = 0x0101,
    BindingError    = 0x0111,
      
    SharedSecretRequest  = 0x0002,
    SharedSecretResponse = 0x0102,
    SharedSecretError    = 0x0112,
  };
  
  SIPSTUNMessage()
  { }
  
  SIPSTUNMessage(MsgType newType, const BYTE * id = NULL)
    : PBYTEArray(sizeof(SIPSTUNMessageHeader))
  {
    SetType(newType, id);
  }

  void SetType(MsgType newType, const BYTE * id = NULL)
  {
    SetMinSize(sizeof(SIPSTUNMessageHeader));
    SIPSTUNMessageHeader * hdr = (SIPSTUNMessageHeader *)theArray;
    hdr->msgType = (WORD)newType;
    for (PINDEX i = 0; i < ((PINDEX)sizeof(hdr->transactionId)); i++)
      hdr->transactionId[i] = id != NULL ? id[i] : (BYTE)PRandom::Number();
  }

  const SIPSTUNMessageHeader * operator->() const { return (SIPSTUNMessageHeader *)theArray; }
  
  SIPSTUNAttribute * GetFirstAttribute() { return (SIPSTUNAttribute *)(theArray+sizeof(SIPSTUNMessageHeader)); }

  bool Validate()
  {
    int length = ((SIPSTUNMessageHeader *)theArray)->msgLength;
    SIPSTUNAttribute * attrib = GetFirstAttribute();
    while (attrib && length > 0) {
      length -= attrib->length + 4;
      attrib = attrib->GetNext();
    }

    return length == 0;  // Exactly correct length
  }

  void AddAttribute(const SIPSTUNAttribute & attribute)
  {
    SIPSTUNMessageHeader * hdr = (SIPSTUNMessageHeader *)theArray;
    int oldLength = hdr->msgLength;
    int attrSize = attribute.length + 4;
    int newLength = oldLength + attrSize;
    hdr->msgLength = (WORD)newLength;
    // hdr pointer may be invalidated by next statement
    SetMinSize(newLength+sizeof(SIPSTUNMessageHeader));
    memcpy(theArray+sizeof(SIPSTUNMessageHeader)+oldLength, &attribute, attrSize);
  }

  void SetAttribute(const SIPSTUNAttribute & attribute)
  {
    int length = ((SIPSTUNMessageHeader *)theArray)->msgLength;
    SIPSTUNAttribute * attrib = GetFirstAttribute();
    while (length > 0) {
      if (attrib->type == attribute.type) {
        if (attrib->length == attribute.length)
          *attrib = attribute;
        else {
          // More here
        }
        return;
      }

      length -= attrib->length + 4;
      attrib = attrib->GetNext();
    }

    AddAttribute(attribute);
  }

  SIPSTUNAttribute * FindAttribute(SIPSTUNAttribute::Types type)
  {
    int length = ((SIPSTUNMessageHeader *)theArray)->msgLength;
    SIPSTUNAttribute * attrib = GetFirstAttribute();
    while (length > 0) {
      if (attrib->type == type)
        return attrib;

      length -= attrib->length + 4;
      attrib = attrib->GetNext();
    }
    return NULL;
  }


  bool Read(PUDPSocket & socket)
  {
    if (!socket.Read(GetPointer(1000), 1000))
      return false;
    SetSize(socket.GetLastReadCount());
    return true;
  }
  
  bool Write(PUDPSocket & socket) const
  {
    return socket.Write(theArray, ((SIPSTUNMessageHeader *)theArray)->msgLength+sizeof(SIPSTUNMessageHeader)) != FALSE;
  }

  bool Poll(PUDPSocket & socket, const SIPSTUNMessage & request, PINDEX pollRetries)
  {
    for (PINDEX retry = 0; retry < pollRetries; retry++) {
      if (!request.Write(socket))
        break;

      if (Read(socket) && Validate() &&
            memcmp(request->transactionId, (*this)->transactionId, sizeof(request->transactionId)) == 0)
        return true;
    }

    return false;
  }
};


bool SIPSTUNClient::OpenSocket(PUDPSocket & socket, PortInfo & portInfo) const
{
  PWaitAndSignal mutex(portInfo.mutex);

  WORD startPort = portInfo.currentPort;

  do {
    portInfo.currentPort++;
    if (portInfo.currentPort > portInfo.maxPort)
      portInfo.currentPort = portInfo.basePort;

    if (socket.Listen(1, portInfo.currentPort)) {
      socket.SetSendAddress(serverAddress, serverPort);
      socket.SetReadTimeout(replyTimeout);
      return true;
    }

  } while (portInfo.currentPort != startPort);

  PTRACE(1, "STUN\tFailed to bind to local UDP port in range "
         << portInfo.currentPort << '-' << portInfo.maxPort);
  return false;
}

BOOL SIPSTUNClient::OpenSocket(
  PUDPSocket & socket,
  const PIPSocket::Address & iface,
  WORD port
)
{
  if (socket.Listen( iface, 1, port)) {
    socket.SetSendAddress(serverAddress, serverPort);
    socket.SetReadTimeout(replyTimeout);
    return TRUE;
  }

  PTRACE(1, "STUN\tFailed to bind to local UDP interface "
    << iface << ':' << port);

  return FALSE;
}


SIPSTUNClient::NatTypes SIPSTUNClient::GetNatType(BOOL force)
{
  if (!force && natType != UnknownNat)
    return natType;

  PUDPSocket socket;
  if (!OpenSocket(socket, singlePortInfo))
    return natType = UnknownNat;

  // RFC3489 discovery

  /* test I - the client sends a STUN Binding Request to a server, without
     any flags set in the CHANGE-REQUEST attribute, and without the
     RESPONSE-ADDRESS attribute. This causes the server to send the response
     back to the address and port that the request came from. */
  SIPSTUNMessage requestI(SIPSTUNMessage::BindingRequest);
  requestI.AddAttribute(SIPSTUNChangeRequest(false, false));
  SIPSTUNMessage responseI;
  if (!responseI.Poll(socket, requestI, pollRetries)) {
    if (socket.GetErrorCode(PChannel::LastWriteError) != PChannel::NoError) {
      PTRACE(1, "STUN\tError writing to server " << serverAddress << ':' << serverPort << " - " << socket.GetErrorText(PChannel::LastWriteError));
      return natType = UnknownNat; // No response usually means blocked
    }

    PTRACE(3, "STUN\tNo response to server " << serverAddress << ':' << serverPort << " - " << socket.GetErrorText(PChannel::LastReadError));
    return natType = BlockedNat; // No response usually means blocked
  }

  SIPSTUNMappedAddress * mappedAddress = (SIPSTUNMappedAddress *)responseI.FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
  if (mappedAddress == NULL) {
    PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
    return natType = UnknownNat; // Protocol error
  }

  PIPSocket::Address mappedAddressI = mappedAddress->GetIP();
  WORD mappedPortI = mappedAddress->port;
  bool notNAT = socket.GetPort() == mappedPortI && PIPSocket::IsLocalHost(mappedAddressI);

  /* Test II - the client sends a Binding Request with both the "change IP"
     and "change port" flags from the CHANGE-REQUEST attribute set. */
  SIPSTUNMessage requestII(SIPSTUNMessage::BindingRequest);
  requestII.AddAttribute(SIPSTUNChangeRequest(true, true));
  SIPSTUNMessage responseII;
  bool testII = responseII.Poll(socket, requestII, pollRetries);

  if (notNAT) {
    // Is not NAT or symmetric firewall
    return natType = (testII ? OpenNat : SymmetricFirewall);
  }

  if (testII)
    return natType = ConeNat;

  SIPSTUNChangedAddress * changedAddress = (SIPSTUNChangedAddress *)responseI.FindAttribute(SIPSTUNAttribute::CHANGED_ADDRESS);
  if (changedAddress == NULL)
    return natType = UnknownNat; // Protocol error

  // Send test I to another server, to see if restricted or symmetric
  PIPSocket::Address secondaryServer = changedAddress->GetIP();
  WORD secondaryPort = changedAddress->port;
  socket.SetSendAddress(secondaryServer, secondaryPort);
  SIPSTUNMessage requestI2(SIPSTUNMessage::BindingRequest);
  requestI2.AddAttribute(SIPSTUNChangeRequest(false, false));
  SIPSTUNMessage responseI2;
  if (!responseI2.Poll(socket, requestI2, pollRetries)) {
    PTRACE(2, "STUN\tPoll of secondary server " << secondaryServer << ':' << secondaryPort
           << " failed, NAT partially blocked by firwall rules.");
    return natType = PartialBlockedNat;
  }

  mappedAddress = (SIPSTUNMappedAddress *)responseI2.FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
  if (mappedAddress == NULL) {
    PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
    return UnknownNat; // Protocol error
  }

  if (mappedAddress->port != mappedPortI || mappedAddress->GetIP() != mappedAddressI)
    return natType = SymmetricNat;

  socket.SetSendAddress(serverAddress, serverPort);
  SIPSTUNMessage requestIII(SIPSTUNMessage::BindingRequest);
  requestIII.SetAttribute(SIPSTUNChangeRequest(false, true));
  SIPSTUNMessage responseIII;
  return natType = (responseIII.Poll(socket, requestIII, pollRetries) ? RestrictedNat : PortRestrictedNat);
}


PString SIPSTUNClient::GetNatTypeString(NatTypes type)
{
  static const char * const Names[NumNatTypes] = {
    "Unknown NAT",
    "Open NAT",
    "Cone NAT",
    "Restricted NAT",
    "Port Restricted NAT",
    "Symmetric NAT",
    "Symmetric Firewall",
    "Blocked",
    "Partially Blocked"
  };

  if (type < NumNatTypes)
    return Names[type];
  
  return psprintf("<NATType %u>", type);
}


SIPSTUNClient::RTPSupportTypes SIPSTUNClient::IsSupportingRTP(BOOL force)
{
  switch (GetNatType(force)) {

    // types that do support RTP 
    case OpenNat:
    case ConeNat:
      return RTPOK;

    // types that support RTP if media sent first
    case SymmetricFirewall:
    case RestrictedNat:
    case PortRestrictedNat:
      return RTPIfSendMedia;

    // types that do not support RTP
    case BlockedNat:
    case SymmetricNat:
      return RTPUnsupported;

    // types that have unknown RTP support
    case UnknownNat:
    case PartialBlockedNat:
    default:
      break;
  }

  return RTPUnknown;
}

BOOL SIPSTUNClient::GetExternalAddress(PIPSocket::Address & externalAddress,
                                     const PTimeInterval & maxAge)
{
  if (cachedExternalAddress.IsValid() && (PTime() - timeAddressObtained < maxAge)) {
    externalAddress = cachedExternalAddress;
    return TRUE;
  }

  externalAddress = 0; // Set to invalid address

  PUDPSocket socket;
  if (!OpenSocket(socket, singlePortInfo))
    return false;

  SIPSTUNMessage request(SIPSTUNMessage::BindingRequest);
  request.AddAttribute(SIPSTUNChangeRequest(false, false));
  SIPSTUNMessage response;
  if (!response.Poll(socket, request, pollRetries))
  {
    PTRACE(1, "STUN\tServer " << serverAddress << ':' << serverPort << " unexpectedly went offline.");
    return false;
  }

  SIPSTUNMappedAddress * mappedAddress = (SIPSTUNMappedAddress *)response.FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
  if (mappedAddress == NULL)
  {
    PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
    return false;
  }

  
  externalAddress = cachedExternalAddress = mappedAddress->GetIP();
  timeAddressObtained = PTime();
  return true;
}


BOOL SIPSTUNClient::CreateSocket(PUDPSocket * & socket)
{
  socket = NULL;

  switch (GetNatType(FALSE)) {
    case ConeNat :
    case RestrictedNat :
    case PortRestrictedNat :
      break;

    case SymmetricNat :
      if (singlePortInfo.basePort == 0 || singlePortInfo.basePort > singlePortInfo.maxPort)
      {
        PTRACE(1, "STUN\tInvalid local UDP port range "
               << singlePortInfo.currentPort << '-' << singlePortInfo.maxPort);
        return FALSE;
      }
      break;

    default : // UnknownNet, SymmetricFirewall, BlockedNat
      PTRACE(1, "STUN\tCannot create socket using NAT type " << GetNatTypeName());
      return FALSE;
  }

  SIPSTUNUDPSocket * stunSocket = new SIPSTUNUDPSocket;
  if (OpenSocket(*stunSocket, singlePortInfo))
  {
    SIPSTUNMessage request(SIPSTUNMessage::BindingRequest);
    request.AddAttribute(SIPSTUNChangeRequest(false, false));
    SIPSTUNMessage response;

    if (response.Poll(*stunSocket, request, pollRetries))
    {
      SIPSTUNMappedAddress * mappedAddress = (SIPSTUNMappedAddress *)response.FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
      if (mappedAddress != NULL)
      {
        stunSocket->externalIP = mappedAddress->GetIP();
        if (GetNatType(FALSE) != SymmetricNat)
          stunSocket->port = mappedAddress->port;
        stunSocket->SetSendAddress(0, 0);
        stunSocket->SetReadTimeout(PMaxTimeInterval);
        socket = stunSocket;
        return true;
      }

      PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
    }
    else
      PTRACE(1, "STUN\tServer " << serverAddress << ':' << serverPort << " unexpectedly went offline.");
  }

  delete stunSocket;
  return false;
}

BOOL SIPSTUNClient::CreateSocket(
  PUDPSocket *& socket,
  const PIPSocket::Address & iface,
  WORD port
)
{
  socket = NULL;

  switch (GetNatType(FALSE)) {
    case ConeNat :
    case RestrictedNat :
    case PortRestrictedNat :
      break;

    case SymmetricNat :
      if (singlePortInfo.basePort == 0 || singlePortInfo.basePort > singlePortInfo.maxPort)
      {
        PTRACE(1, "STUN\tInvalid local UDP port range "
               << singlePortInfo.currentPort << '-' << singlePortInfo.maxPort);
        return FALSE;
      }
      break;

    default : // UnknownNet, SymmetricFirewall, BlockedNat
      PTRACE(1, "STUN\tCannot create socket using NAT type " << GetNatTypeName());
      return FALSE;
  }

  SIPSTUNUDPSocket * stunSocket = new SIPSTUNUDPSocket;
  if (OpenSocket(*stunSocket, iface, port))
  {
    SIPSTUNMessage request(SIPSTUNMessage::BindingRequest);
    request.AddAttribute(SIPSTUNChangeRequest(false, false));
    SIPSTUNMessage response;

    if (response.Poll(*stunSocket, request, pollRetries))
    {
      SIPSTUNMappedAddress * mappedAddress = (SIPSTUNMappedAddress *)response.FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
      if (mappedAddress != NULL)
      {
        stunSocket->externalIP = mappedAddress->GetIP();
        if (GetNatType(FALSE) != SymmetricNat)
          stunSocket->port = mappedAddress->port;
        stunSocket->SetSendAddress(0, 0);
        stunSocket->SetReadTimeout(PMaxTimeInterval);
        socket = stunSocket;
        return TRUE;
      }

      PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
    }
    else
      PTRACE(1, "STUN\tServer " << serverAddress << ':' << serverPort << " unexpectedly went offline.");
  }

  delete stunSocket;
  return FALSE;
}

BOOL SIPSTUNClient::CreateSocketPair(PUDPSocket * & socket1,
                                   PUDPSocket * & socket2)
{
  socket1 = NULL;
  socket2 = NULL;

  switch (GetNatType(FALSE)) {
    case ConeNat :
    case RestrictedNat :
    case PortRestrictedNat :
      break;

    case SymmetricNat :
      if (pairedPortInfo.basePort == 0 || pairedPortInfo.basePort > pairedPortInfo.maxPort)
      {
        PTRACE(1, "STUN\tInvalid local UDP port range "
               << pairedPortInfo.currentPort << '-' << pairedPortInfo.maxPort);
        return FALSE;
      }
      break;

    default : // UnknownNet, SymmetricFirewall, BlockedNat
      PTRACE(1, "STUN\tCannot create socket pair using NAT type " << GetNatTypeName());
      return FALSE;
  }

  PINDEX i;

  PList<SIPSTUNUDPSocket> stunSocket;
  PList<SIPSTUNMessage> request;
  PList<SIPSTUNMessage> response;

  for (i = 0; i < numSocketsForPairing; i++)
  {
    PINDEX idx = stunSocket.Append(new SIPSTUNUDPSocket);
    if (!OpenSocket(stunSocket[idx], pairedPortInfo))
      return false;

    idx = request.Append(new SIPSTUNMessage(SIPSTUNMessage::BindingRequest));
    request[idx].AddAttribute(SIPSTUNChangeRequest(false, false));

    response.Append(new SIPSTUNMessage);
  }

  for (i = 0; i < numSocketsForPairing; i++)
  {
    if (!response[i].Poll(stunSocket[i], request[i], pollRetries))
    {
      PTRACE(1, "STUN\tServer " << serverAddress << ':' << serverPort << " unexpectedly went offline.");
      return false;
    }
  }

  for (i = 0; i < numSocketsForPairing; i++)
  {
    SIPSTUNMappedAddress * mappedAddress = (SIPSTUNMappedAddress *)response[i].FindAttribute(SIPSTUNAttribute::MAPPED_ADDRESS);
    if (mappedAddress == NULL)
    {
      PTRACE(2, "STUN\tExpected mapped address attribute from server " << serverAddress << ':' << serverPort);
      return false;
    }
    if (GetNatType(FALSE) != SymmetricNat)
      stunSocket[i].port = mappedAddress->port;
    stunSocket[i].externalIP = mappedAddress->GetIP();
  }

  for (i = 0; i < numSocketsForPairing; i++)
  {
    for (PINDEX j = 0; j < numSocketsForPairing; j++)
    {
      if ((stunSocket[i].port&1) == 0 && (stunSocket[i].port+1) == stunSocket[j].port) {
        stunSocket[i].SetSendAddress(0, 0);
        stunSocket[i].SetReadTimeout(PMaxTimeInterval);
        stunSocket[j].SetSendAddress(0, 0);
        stunSocket[j].SetReadTimeout(PMaxTimeInterval);
        socket1 = &stunSocket[i];
        socket2 = &stunSocket[j];
        stunSocket.DisallowDeleteObjects();
        stunSocket.Remove(socket1);
        stunSocket.Remove(socket2);
        stunSocket.AllowDeleteObjects();
        return true;
      }
    }
  }

  PTRACE(2, "STUN\tCould not get a pair of adjacent port numbers from NAT");
  return false;
}

BOOL SIPSTUNClient::IsAvailable() 
{ 

  switch (GetNatType(FALSE)) {
    case ConeNat :
    case RestrictedNat :
    case PortRestrictedNat :
      break;

    case SymmetricNat :
      if (pairedPortInfo.basePort == 0 || pairedPortInfo.basePort > pairedPortInfo.maxPort)
         return FALSE;
      
      break;

    default : // UnknownNet, SymmetricFirewall, BlockedNat
      return FALSE;
  }

  return TRUE; 
}

////////////////////////////////////////////////////////////////

SIPSTUNUDPSocket::SIPSTUNUDPSocket()
  : externalIP(0)
{
}


BOOL SIPSTUNUDPSocket::GetLocalAddress(Address & addr)
{
  if (!externalIP.IsValid())
    return PUDPSocket::GetLocalAddress(addr);

  addr = externalIP;
  return true;
}


BOOL SIPSTUNUDPSocket::GetLocalAddress(Address & addr, WORD & port)
{
  if (!externalIP.IsValid())
    return PUDPSocket::GetLocalAddress(addr, port);

  addr = externalIP;
  port = GetPort();
  return true;
}


// End of File ////////////////////////////////////////////////////////////////
