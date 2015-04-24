/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

// This source is only used on Windows; SSPI is the Windows mechanism for
// accessing authentication mechanisms, analogous to Cyrus SASL.

<<<<<<< HEAD
#include "qpid/broker/Connection.h"
#include "qpid/log/Statement.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/framing/FieldValue.h"
=======
#include "qpid/broker/amqp_0_10/Connection.h"
#include "qpid/broker/Broker.h"
#include "qpid/log/Statement.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/framing/FieldValue.h"
#include "qpid/sys/ConnectionOutputHandler.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

#include <windows.h>

using namespace qpid::framing;
using qpid::sys::SecurityLayer;

using std::string;

namespace qpid {
namespace broker {

class NullAuthenticator : public SaslAuthenticator
{
<<<<<<< HEAD
    Connection& connection;
    framing::AMQP_ClientProxy::Connection client;
public:
    NullAuthenticator(Connection& connection);
=======
    qpid::broker::amqp_0_10::Connection& connection;
    framing::AMQP_ClientProxy::Connection client;
    string realm;
public:
    NullAuthenticator(qpid::broker::amqp_0_10::Connection& connection);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    ~NullAuthenticator();
    void getMechanisms(framing::Array& mechanisms);
    void start(const std::string& mechanism, const std::string* response);
    void step(const std::string&) {}
    std::auto_ptr<SecurityLayer> getSecurityLayer(uint16_t maxFrameSize);
};

class SspiAuthenticator : public SaslAuthenticator
{
    HANDLE userToken;
<<<<<<< HEAD
    Connection& connection;
    framing::AMQP_ClientProxy::Connection client;

public:
    SspiAuthenticator(Connection& connection);
=======
    qpid::broker::amqp_0_10::Connection& connection;
    framing::AMQP_ClientProxy::Connection client;

public:
    SspiAuthenticator(qpid::broker::amqp_0_10::Connection& connection);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    ~SspiAuthenticator();
    void getMechanisms(framing::Array& mechanisms);
    void start(const std::string& mechanism, const std::string* response);
    void step(const std::string& response);
    std::auto_ptr<SecurityLayer> getSecurityLayer(uint16_t maxFrameSize);
};

bool SaslAuthenticator::available(void)
{
    return true;
}

// Initialize the SASL mechanism; throw if it fails.
void SaslAuthenticator::init(const std::string& /*saslName*/, const std::string& /*saslConfig*/)
{
    return;
}

void SaslAuthenticator::fini(void)
{
    return;
}

<<<<<<< HEAD
std::auto_ptr<SaslAuthenticator> SaslAuthenticator::createAuthenticator(Connection& c)
{
    if (c.getBroker().getOptions().auth) {
=======
std::auto_ptr<SaslAuthenticator> SaslAuthenticator::createAuthenticator(qpid::broker::amqp_0_10::Connection& c)
{
    if (c.getBroker().isAuthenticating()) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return std::auto_ptr<SaslAuthenticator>(new SspiAuthenticator(c));
    } else {
        return std::auto_ptr<SaslAuthenticator>(new NullAuthenticator(c));
    }
}

<<<<<<< HEAD
NullAuthenticator::NullAuthenticator(Connection& c) : connection(c), client(c.getOutput()) {}
=======
NullAuthenticator::NullAuthenticator(qpid::broker::amqp_0_10::Connection& c) :
    connection(c), client(c.getOutput()), realm("@"+c.getBroker().getRealm()) {}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
NullAuthenticator::~NullAuthenticator() {}

void NullAuthenticator::getMechanisms(Array& mechanisms)
{
    mechanisms.add(boost::shared_ptr<FieldValue>(new Str16Value("ANONYMOUS")));
    mechanisms.add(boost::shared_ptr<FieldValue>(new Str16Value("PLAIN")));
}

<<<<<<< HEAD
=======
namespace {
bool endsWith(const std::string& str, const std::string& ending) {
    return (ending.size() <= str.size()) &&
        (str.compare(str.size() - ending.size(), ending.size(), ending) == 0);
}
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
void NullAuthenticator::start(const string& mechanism, const string* response)
{
    QPID_LOG(warning, "SASL: No Authentication Performed");
    if (mechanism == "PLAIN") { // Old behavior
        if (response && response->size() > 0 && (*response).c_str()[0] == (char) 0) {
            string temp = response->substr(1);
            string::size_type i = temp.find((char)0);
            string uid = temp.substr(0, i);
            string pwd = temp.substr(i + 1);
<<<<<<< HEAD
=======
            if (!endsWith(uid, realm)) uid += realm;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            connection.setUserId(uid);
        }
    } else {
        connection.setUserId("anonymous");
    }   
    client.tune(framing::CHANNEL_MAX, connection.getFrameMax(), 0, 0);
}

std::auto_ptr<SecurityLayer> NullAuthenticator::getSecurityLayer(uint16_t)
{
    std::auto_ptr<SecurityLayer> securityLayer;
    return securityLayer;
}


<<<<<<< HEAD
SspiAuthenticator::SspiAuthenticator(Connection& c) : userToken(INVALID_HANDLE_VALUE), connection(c), client(c.getOutput()) 
=======
SspiAuthenticator::SspiAuthenticator(qpid::broker::amqp_0_10::Connection& c) : userToken(INVALID_HANDLE_VALUE), connection(c), client(c.getOutput()) 
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
}

SspiAuthenticator::~SspiAuthenticator()
{
    if (INVALID_HANDLE_VALUE != userToken) {
        CloseHandle(userToken);
        userToken = INVALID_HANDLE_VALUE;
    }
}

void SspiAuthenticator::getMechanisms(Array& mechanisms)
{
    mechanisms.add(boost::shared_ptr<FieldValue>(new Str16Value(string("ANONYMOUS"))));
    mechanisms.add(boost::shared_ptr<FieldValue>(new Str16Value(string("PLAIN"))));
    QPID_LOG(info, "SASL: Mechanism list: ANONYMOUS PLAIN");
}

void SspiAuthenticator::start(const string& mechanism, const string* response)
{
    QPID_LOG(info, "SASL: Starting authentication with mechanism: " << mechanism);
    if (mechanism == "ANONYMOUS") {
        connection.setUserId("anonymous");
        client.tune(framing::CHANNEL_MAX, connection.getFrameMax(), 0, 0);
        return;
    }
    if (mechanism != "PLAIN")
        throw ConnectionForcedException("Unsupported mechanism");

    // PLAIN's response is composed of 3 strings separated by 0 bytes:
    // authorization id, authentication id (user), clear-text password.
    if (!response || response->size() == 0)
        throw ConnectionForcedException("Authentication failed");

    string::size_type i = response->find((char)0);
    string auth = response->substr(0, i);
    string::size_type j = response->find((char)0, i+1);
    string uid = response->substr(i+1, j-1);
    string pwd = response->substr(j+1);
    string dot(".");
    int error = 0;
    if (!LogonUser(const_cast<char*>(uid.c_str()),
                   const_cast<char*>(dot.c_str()),
                   const_cast<char*>(pwd.c_str()),
                   LOGON32_LOGON_NETWORK,
                   LOGON32_PROVIDER_DEFAULT,
                   &userToken))
        error = GetLastError();
    pwd.replace(0, string::npos, 1, (char)0);
    if (error != 0) {
      QPID_LOG(info,
               "SASL: Auth failed [" << error << "]: " << qpid::sys::strError(error));
        throw ConnectionForcedException("Authentication failed");
    }

    connection.setUserId(uid);
    client.tune(framing::CHANNEL_MAX, connection.getFrameMax(), 0, 0);
}
        
void SspiAuthenticator::step(const string& /*response*/)
{
  QPID_LOG(info, "SASL: Need another step!!!");
}

std::auto_ptr<SecurityLayer> SspiAuthenticator::getSecurityLayer(uint16_t)
{
    std::auto_ptr<SecurityLayer> securityLayer;
    return securityLayer;
}

}}
