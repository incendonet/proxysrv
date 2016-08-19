/*
 *
 * XMPP.h
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
 * -----------------------------------
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Log: XMPP.cxx,v $
 * Revision 1.2  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/08/07 07:13:45  joegenbaclor
 * Added XMPP/Jabber support via Google's libJingle
 *
 */

#include "XMPP.h"

#if ENABLE_XMPP

#ifdef _MSC_VER
#ifndef _WIN32_WCE
#pragma comment(lib, LIBJINGLE_LIBRARY)
#endif // !_WIN32_WCE
#endif // _MSC_VER

using namespace Jabber;

#define new PNEW

#include <errno.h>
#include "talk/base/basicdefs.h"
#include "talk/base/logging.h"
#include "talk/base/thread.h"
#ifdef FEATURE_ENABLE_SSL
#include "talk/base/ssladapter.h"
#endif


XMPPSocket::XMPPSocket(bool tls) : tls_(tls) {
  cricket::Thread* pth = cricket::Thread::Current();
  cricket::AsyncSocket* socket =
    pth->socketserver()->CreateAsyncSocket(SOCK_STREAM);
#ifdef FEATURE_ENABLE_SSL
  if (tls_) {
    socket = cricket::SSLAdapter::Create(socket);
  }
#endif
  cricket_socket_ = socket;
  cricket_socket_->SignalReadEvent.connect(this, &XMPPSocket::OnReadEvent);
  cricket_socket_->SignalWriteEvent.connect(this, &XMPPSocket::OnWriteEvent);
  cricket_socket_->SignalConnectEvent.connect(this,
                                              &XMPPSocket::OnConnectEvent);
  state_ = buzz::AsyncSocket::STATE_CLOSED;
}

XMPPSocket::~XMPPSocket() {
  Close();
  delete cricket_socket_;
}

void XMPPSocket::OnReadEvent(cricket::AsyncSocket * socket) {
  SignalRead();
}

void XMPPSocket::OnWriteEvent(cricket::AsyncSocket * socket) {
  // Write bytes if there are any
  while (buffer_.Length() != 0) {
    int written = cricket_socket_->Send(buffer_.Data(), buffer_.Length());
    if (written > 0) {
      buffer_.Shift(written);
      continue;
    }
    if (!cricket_socket_->IsBlocking())
      LOG(LS_ERROR) << "Send error: " << cricket_socket_->GetError();
    return;
  }
}

void XMPPSocket::OnConnectEvent(cricket::AsyncSocket * socket) {
#if defined(FEATURE_ENABLE_SSL)
  if (state_ == buzz::AsyncSocket::STATE_TLS_CONNECTING) {
    state_ = buzz::AsyncSocket::STATE_TLS_OPEN;
    SignalSSLConnected();
    OnWriteEvent(cricket_socket_);
    return;
  }
#endif  // !defined(FEATURE_ENABLE_SSL)
  state_ = buzz::AsyncSocket::STATE_OPEN;
  SignalConnected();
}

buzz::AsyncSocket::State XMPPSocket::state() {
  return state_;
}

buzz::AsyncSocket::Error XMPPSocket::error() {
  return buzz::AsyncSocket::ERROR_NONE;
}

bool XMPPSocket::Connect(const cricket::SocketAddress& addr) {
  if (cricket_socket_->Connect(addr) < 0) {
    return cricket_socket_->IsBlocking();
  }
  return true;
}

bool XMPPSocket::Read(char * data, size_t len, size_t* len_read) {
  int read = cricket_socket_->Recv(data, len);
  if (read > 0) {
    *len_read = (size_t)read;
    return true;
  }
  return false;
}

bool XMPPSocket::Write(const char * data, size_t len) {
  buffer_.WriteBytes(data, len);
  OnWriteEvent(cricket_socket_);
  return true;
}

bool XMPPSocket::Close() {
  if (state_ != buzz::AsyncSocket::STATE_OPEN)
    return false;
  if (cricket_socket_->Close() == 0) {
    state_ = buzz::AsyncSocket::STATE_CLOSED;
    SignalClosed();
    return true;
  }
  return false;
}

bool XMPPSocket::StartTls(const std::string & domainname) {
#if defined(FEATURE_ENABLE_SSL)
  if (!tls_)
    return false;
  cricket::SSLAdapter* ssl_adapter =
    static_cast<cricket::SSLAdapter *>(cricket_socket_);
  ssl_adapter->set_ignore_bad_cert(true);
  if (ssl_adapter->StartSSL(domainname.c_str(), false) != 0)
    return false;
  state_ = buzz::AsyncSocket::STATE_TLS_CONNECTING;
  return true;
#else  // !defined(FEATURE_ENABLE_SSL)
  return false;
#endif  // !defined(FEATURE_ENABLE_SSL)
}

/////////////////////////////////////////////////////////////

XMPPAuth::XMPPAuth() : done_(false), error_(false) {
}

XMPPAuth::~XMPPAuth() {
}
  
void XMPPAuth::StartPreXmppAuth(const buzz::Jid & jid,
                                const cricket::SocketAddress & server,
                                const buzz::XmppPassword & pass,
                                const std::string & auth_cookie) {
  jid_ = jid;
  passwd_ = pass;
  auth_cookie_ = auth_cookie;
  error_ = auth_cookie.empty();
  done_ = true;

  SignalAuthDone();
}
  
std::string XMPPAuth::ChooseBestSaslMechanism(
    const std::vector<std::string> & mechanisms,
    bool encrypted) {
  std::vector<std::string>::const_iterator it;

  // a token is the weakest auth - 15s, service-limited, so prefer it.
  it = std::find(mechanisms.begin(), mechanisms.end(), "X-GOOGLE-TOKEN");
  if (it != mechanisms.end())
    return "X-GOOGLE-TOKEN";

  // a cookie is the next weakest - 14 days
  it = std::find(mechanisms.begin(), mechanisms.end(), "X-GOOGLE-COOKIE");
  if (it != mechanisms.end())
    return "X-GOOGLE-COOKIE";

  // never pass @google.com passwords without encryption!!
  if (!encrypted && (jid_.domain() == "google.com"))
    return "";

  // as a last resort, use plain authentication
  if (jid_.domain() != "google.com") {
    it = std::find(mechanisms.begin(), mechanisms.end(), "PLAIN");
    if (it != mechanisms.end())
      return "PLAIN";
  }

  // No good mechanism found
 return "";
}

buzz::SaslMechanism* XMPPAuth::CreateSaslMechanism(
    const std::string & mechanism) {
  if (mechanism == "X-GOOGLE-TOKEN") {
    return new buzz::SaslCookieMechanism(mechanism, jid_.Str(), auth_cookie_);
  //} else if (mechanism == "X-GOOGLE-COOKIE") {
  //  return new buzz::SaslCookieMechanism(mechanism, jid.Str(), sid_);
  } else if (mechanism == "PLAIN") {
    return new buzz::SaslPlainMechanism(jid_, passwd_);
  } else {
    return NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////

XMPPPump::XMPPPump(XMPPPumpNotify * notify) {
  state_ = buzz::XmppEngine::STATE_NONE;
  notify_ = notify;
  client_ = new buzz::XmppClient(this);  // NOTE: deleted by TaskRunner
}

void XMPPPump::DoLogin(const buzz::XmppClientSettings & xcs,
                       buzz::AsyncSocket* socket,
                       buzz::PreXmppAuth* auth) {
  OnStateChange(buzz::XmppEngine::STATE_START);
  client_->SignalStateChange.connect(this, &XMPPPump::OnStateChange);
  client_->Connect(xcs, socket, auth);
  client_->Start();
}

void XMPPPump::DoDisconnect() {
  client_->Disconnect();
  OnStateChange(buzz::XmppEngine::STATE_CLOSED);
}

void XMPPPump::OnStateChange(buzz::XmppEngine::State state) {
  if (state_ == state)
    return;
  state_ = state;
  if (notify_ != NULL)
    notify_->OnStateChange(state);
}

void XMPPPump::WakeTasks() {
  cricket::Thread::Current()->Post(this);
}

int64 XMPPPump::CurrentTime() {
  return (int64)cricket::Time();
}

void XMPPPump::OnMessage(cricket::Message *pmsg) {
  RunTasks();
}

buzz::XmppReturnStatus XMPPPump::SendStanza(const buzz::XmlElement *stanza) {
  return client_->SendStanza(stanza);
}


///////////////////////////////////////////////////////////

namespace {

const uint32 MSG_LOGIN = 1;
const uint32 MSG_DISCONNECT = 2;

struct LoginData: public cricket::MessageData {
  LoginData(const buzz::XmppClientSettings& s) : xcs(s) {}
  virtual ~LoginData() {}

  buzz::XmppClientSettings xcs;
};

} // namespace

XMPP::XMPP() {
  pump_ = new XMPPPump(this);
}

XMPP::~XMPP() {
  delete pump_;
}

void XMPP::Loop(int cms) {
  Thread::Loop(cms);
}

void XMPP::Login(const buzz::XmppClientSettings& xcs) {
  Post(this, MSG_LOGIN, new LoginData(xcs));
}

void XMPP::Disconnect() {
  Post(this, MSG_DISCONNECT);
}

void XMPP::OnStateChange(buzz::XmppEngine::State state) {
}

void XMPP::OnMessage(cricket::Message* pmsg) {
  if (pmsg->message_id == MSG_LOGIN) {
    assert(pmsg->pdata);
    LoginData* data = reinterpret_cast<LoginData*>(pmsg->pdata);
    pump_->DoLogin(data->xcs, new XMPPSocket(false), new XMPPAuth());
    delete data;
  } else if (pmsg->message_id == MSG_DISCONNECT) {
    pump_->DoDisconnect();
  } else {
    assert(false);
  }
}


#endif  // ENABLE_XMPP



