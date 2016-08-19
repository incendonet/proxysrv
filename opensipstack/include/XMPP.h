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
 * $Log: XMPP.h,v $
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/08/07 07:13:44  joegenbaclor
 * Added XMPP/Jabber support via Google's libJingle
 *
 */

#ifndef XMPP_H
#define XMPP_H

#include "ptlib.h"
#include "ossbuildopts.h"

#if ENABLE_XMPP

#include <algorithm>
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"
#include "talk/base/taskrunner.h"
#include "talk/base/thread.h"
#include "talk/base/time.h"
#include "talk/xmpp/xmppclient.h"
#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmpptask.h"
#include "talk/base/asyncsocket.h"
#include "talk/base/bytebuffer.h"
#include "talk/base/sigslot.h"
#include "talk/xmpp/asyncsocket.h"
#include "talk/xmpp/jid.h"
#include "talk/xmpp/saslhandler.h"
#include "talk/xmpp/prexmppauth.h"
#include "talk/xmpp/xmpppassword.h"
#include "talk/xmpp/saslcookiemechanism.h"
#include "talk/xmpp/saslplainmechanism.h"

extern cricket::AsyncSocket* cricket_socket_;

namespace Jabber
{
  class XMPPSocket : public buzz::AsyncSocket, public sigslot::has_slots<> 
  {
  public:

    XMPPSocket(
      bool tls
    );

    ~XMPPSocket();

    virtual buzz::AsyncSocket::State state();
    
    virtual buzz::AsyncSocket::Error error();

    virtual bool Connect(
      const cricket::SocketAddress& addr
    );

    virtual bool Read(
      char * data, 
      size_t len, 
      size_t* len_read
    );

    virtual bool Write(
      const char * data, 
      size_t len
    );

    virtual bool Close();
    
    virtual bool StartTls(
      const std::string & domainname
    );

  private:
    
    void OnReadEvent(
      cricket::AsyncSocket * socket
    );

    void OnWriteEvent(
      cricket::AsyncSocket * socket
    );

    void OnConnectEvent(
      cricket::AsyncSocket * socket
    );

    cricket::AsyncSocket * cricket_socket_;
    buzz::AsyncSocket::State state_;
    cricket::ByteBuffer buffer_;
    bool tls_;
  };

  ////////////////////////////////////////////////////////////////////

  class XMPPPumpNotify
  {
  public:

    virtual void OnStateChange(
      buzz::XmppEngine::State state
    ) = 0;
  };

  class XMPPPump : public cricket::MessageHandler, public buzz::TaskRunner 
  {
  public:
    XMPPPump(
      XMPPPumpNotify * notify = NULL
    );

    void DoLogin(
      const buzz::XmppClientSettings & xcs,
      buzz::AsyncSocket* socket,
      buzz::PreXmppAuth* auth
    );

    void DoDisconnect();

    void OnStateChange(
      buzz::XmppEngine::State state
    );

    void WakeTasks();

    int64 CurrentTime();

    void OnMessage(
      cricket::Message *pmsg
    );

    PINLINE buzz::XmppReturnStatus SendStanza(const buzz::XmlElement *stanza);
    PINLINE buzz::XmppClient *client() { return client_; }
  private:
    buzz::XmppClient *client_;
    buzz::XmppEngine::State state_;
    XMPPPumpNotify *notify_;
  };

  class XMPPAuth: public buzz::PreXmppAuth 
  {
  public:
    XMPPAuth();

    virtual ~XMPPAuth();
    
    virtual void StartPreXmppAuth(
      const buzz::Jid & jid,
      const cricket::SocketAddress & server,
      const buzz::XmppPassword & pass,
      const std::string & auth_cookie
    );
   
    virtual bool IsAuthDone() { return done_; }
    virtual bool IsAuthorized() { return !error_; }
    virtual bool HadError() { return error_; }
    virtual buzz::CaptchaChallenge GetCaptchaChallenge() {
        return buzz::CaptchaChallenge();
    }
    virtual std::string GetAuthCookie() { return auth_cookie_; }

    virtual std::string ChooseBestSaslMechanism(
        const std::vector<std::string> & mechanisms,
        bool encrypted);

    virtual buzz::SaslMechanism * CreateSaslMechanism(
        const std::string & mechanism);

  private:
    buzz::Jid jid_;
    buzz::XmppPassword passwd_;
    std::string auth_cookie_;
    bool done_, error_;
  };

  ////////////////////////////////////////////////////////////////

  class XMPP : public cricket::Thread, XMPPPumpNotify, cricket::MessageHandler 
  {
  public:
    
    XMPP();
    
    ~XMPP();
    
    void Loop(
      int cms
    );

    void Login(
      const buzz::XmppClientSettings & xcs
    );

    void Disconnect();

    PINLINE buzz::XmppClient* client() { return pump_->client(); }

  private: void OnStateChange(
      buzz::XmppEngine::State state
    );
    
  private: void OnMessage(
      cricket::Message* pmsg
    );

  private:
    XMPPPump* pump_;
    

};
};

#endif  // ENABLE_XMPP

#endif  // XMPP_H


