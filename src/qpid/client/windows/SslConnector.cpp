/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
<<<<<<< HEAD
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
=======
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "qpid/client/TCPConnector.h"

#include "config.h"
#include "qpid/Msg.h"
#include "qpid/client/ConnectionImpl.h"
#include "qpid/client/ConnectionSettings.h"
#include "qpid/log/Statement.h"
#include "qpid/sys/Dispatcher.h"
#include "qpid/sys/Poller.h"
#include "qpid/sys/Time.h"
#include "qpid/sys/windows/check.h"
<<<<<<< HEAD
#include "qpid/sys/windows/SslAsynchIO.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <memory.h>
// security.h needs to see this to distinguish from kernel use.
#define SECURITY_WIN32
#include <security.h>
#include <Schnlsp.h>
#undef SECURITY_WIN32
#include <winsock2.h>

=======
#include "qpid/sys/windows/util.h"
#include "qpid/sys/windows/SslAsynchIO.h"
#include "qpid/sys/windows/SslCredential.h"

#include <boost/bind.hpp>

#include <memory.h>
#include <winsock2.h>



>>>>>>> 3bbfc42... Imported Upstream version 0.32
namespace qpid {
namespace client {
namespace windows {

<<<<<<< HEAD
using namespace qpid::sys;
using boost::format;
using boost::str;

=======
using qpid::sys::Socket;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

class SslConnector : public qpid::client::TCPConnector
{
    qpid::sys::windows::ClientSslAsynchIO *shim;
<<<<<<< HEAD
	boost::shared_ptr<qpid::sys::Poller> poller;
    std::string brokerHost;
    SCHANNEL_CRED cred;
    CredHandle credHandle;
    TimeStamp credExpiry;

    virtual ~SslConnector();
    void negotiationDone(SECURITY_STATUS status);

    // A number of AsynchIO callbacks go right through to TCPConnector, but
    // we can't boost::bind to a protected ancestor, so these methods redirect
    // to those TCPConnector methods.
    bool redirectReadbuff(qpid::sys::AsynchIO&, qpid::sys::AsynchIOBufferBase*);
    void redirectWritebuff(qpid::sys::AsynchIO&);
    void redirectEof(qpid::sys::AsynchIO&);
=======
    boost::shared_ptr<qpid::sys::Poller> poller;
    std::string brokerHost;
    qpid::sys::windows::SslCredential sslCredential;
    bool certLoaded;

    void negotiationDone(SECURITY_STATUS status);

    void connect(const std::string& host, const std::string& port);
    void connected(const Socket&);
>>>>>>> 3bbfc42... Imported Upstream version 0.32

public:
    SslConnector(boost::shared_ptr<qpid::sys::Poller>,
                 framing::ProtocolVersion pVersion,
<<<<<<< HEAD
                 const ConnectionSettings&, 
                 ConnectionImpl*);
    virtual void connect(const std::string& host, const std::string& port);
    virtual void connected(const Socket&);
    unsigned int getSSF();
=======
                 const ConnectionSettings&,
                 ConnectionImpl*);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

// Static constructor which registers connector here
namespace {
    Connector* create(boost::shared_ptr<qpid::sys::Poller> p,
                      framing::ProtocolVersion v,
                      const ConnectionSettings& s,
                      ConnectionImpl* c) {
        return new SslConnector(p, v, s, c);
    }

    struct StaticInit {
        StaticInit() {
            try {
<<<<<<< HEAD
                Connector::registerFactory("ssl", &create);
=======
                CommonOptions common("", "", QPIDC_CONF_FILE);
                qpid::sys::ssl::SslOptions options;
                common.parse(0, 0, common.clientConfig, true);
                options.parse (0, 0, common.clientConfig, true);
                Connector::registerFactory("ssl", &create);
                initWinSsl(options);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            } catch (const std::exception& e) {
                QPID_LOG(error, "Failed to initialise SSL connector: " << e.what());
            }
        };
        ~StaticInit() { }
    } init;
<<<<<<< HEAD
=======

>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void SslConnector::negotiationDone(SECURITY_STATUS status)
{
<<<<<<< HEAD
    if (status == SEC_E_OK)
        initAmqp();
    else
        connectFailed(QPID_MSG(qpid::sys::strError(status)));
}

bool SslConnector::redirectReadbuff(qpid::sys::AsynchIO& a,
                                    qpid::sys::AsynchIOBufferBase* b) {
    return readbuff(a, b);
}

void SslConnector::redirectWritebuff(qpid::sys::AsynchIO& a) {
    writebuff(a);
}

void SslConnector::redirectEof(qpid::sys::AsynchIO& a) {
    eof(a);
=======
    if (status == SEC_E_OK) {
        initAmqp();
    }
    else {
        if (status == SEC_E_INCOMPLETE_CREDENTIALS && !certLoaded) {
            // Server requested a client cert but we supplied none for the following reason:
            connectFailed(QPID_MSG(sslCredential.error()));
        }
        else
            connectFailed(QPID_MSG(qpid::sys::strError(status)));
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

SslConnector::SslConnector(boost::shared_ptr<qpid::sys::Poller> p,
                           framing::ProtocolVersion ver,
                           const ConnectionSettings& settings,
                           ConnectionImpl* cimpl)
    : TCPConnector(p, ver, settings, cimpl), shim(0), poller(p)
{
<<<<<<< HEAD
    memset(&cred, 0, sizeof(cred));
    cred.dwVersion = SCHANNEL_CRED_VERSION;
    SECURITY_STATUS status = ::AcquireCredentialsHandle(NULL,
                                                        UNISP_NAME,
                                                        SECPKG_CRED_OUTBOUND,
                                                        NULL,
                                                        &cred,
                                                        NULL,
                                                        NULL,
                                                        &credHandle,
                                                        &credExpiry);
    if (status != SEC_E_OK)
        throw QPID_WINDOWS_ERROR(status);
    QPID_LOG(debug, "SslConnector created for " << ver.toString());
}

SslConnector::~SslConnector()
{
    ::FreeCredentialsHandle(&credHandle);
}

  // Will this get reach via virtual method via boost::bind????

=======
    if (settings.sslIgnoreHostnameVerificationFailure) {
        sslCredential.ignoreHostnameVerificationFailure();
    }
    const std::string& name = (settings.sslCertName != "") ?
        settings.sslCertName : qpid::sys::ssl::SslOptions::global.certName;
    certLoaded = sslCredential.load(name);
    QPID_LOG(debug, "SslConnector created for " << ver.toString());
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
void SslConnector::connect(const std::string& host, const std::string& port) {
    brokerHost = host;
    TCPConnector::connect(host, port);
}

void SslConnector::connected(const Socket& s) {
    shim = new qpid::sys::windows::ClientSslAsynchIO(brokerHost,
                                                     s,
<<<<<<< HEAD
                                                     credHandle,
                                                     boost::bind(&SslConnector::redirectReadbuff, this, _1, _2),
                                                     boost::bind(&SslConnector::redirectEof, this, _1),
                                                     boost::bind(&SslConnector::redirectEof, this, _1),
                                                     0, // closed
                                                     0, // nobuffs
                                                     boost::bind(&SslConnector::redirectWritebuff, this, _1),
                                                     boost::bind(&SslConnector::negotiationDone, this, _1));
    start(shim);
	shim->start(poller);
}

unsigned int SslConnector::getSSF()
{
    return shim->getSslKeySize();
=======
                                                     sslCredential.handle(),
                                                     boost::bind(&SslConnector::readbuff, this, _1, _2),
                                                     boost::bind(&SslConnector::eof, this, _1),
                                                     boost::bind(&SslConnector::disconnected, this, _1),
                                                     boost::bind(&SslConnector::socketClosed, this, _1, _2),
                                                     0, // nobuffs
                                                     boost::bind(&SslConnector::writebuff, this, _1),
                                                     boost::bind(&SslConnector::negotiationDone, this, _1));
    start(shim);
    shim->start(poller);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

}}} // namespace qpid::client::windows
