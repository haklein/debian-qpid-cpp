#ifndef _sys_ssl_Socket_h
#define _sys_ssl_Socket_h

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

#include "qpid/sys/IOHandle.h"
<<<<<<< HEAD
#include "qpid/sys/Socket.h"
=======
#include "qpid/sys/posix/BSDSocket.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <nspr.h>

#include <string>

struct sockaddr;

namespace qpid {
namespace sys {

class Duration;

namespace ssl {

<<<<<<< HEAD
class SslSocket : public qpid::sys::Socket
{
public:
    /** Create a socket wrapper for descriptor. */
    SslSocket();
=======
class SslSocket : public qpid::sys::BSDSocket
{
public:
    /** Create a socket wrapper for descriptor.
     *@param certName name of certificate to use to identify the socket
     */
    SslSocket(const std::string& certName = "", bool clientAuth = false);

    /** Proceed with connect inspite of hostname verifcation failures*/
    void ignoreHostnameVerificationFailure();
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    /** Set socket non blocking */
    void setNonblocking() const;

    /** Set tcp-nodelay */
<<<<<<< HEAD
    void setTcpNoDelay(bool nodelay) const;
=======
    void setTcpNoDelay() const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    /** Set SSL cert-name. Allows the cert-name to be set per
     * connection, overriding global cert-name settings from
     * NSSInit().*/
    void setCertName(const std::string& certName);

<<<<<<< HEAD
    void connect(const std::string& host, const std::string& port) const;
=======
    void connect(const SocketAddress&) const;
    void finishConnect(const SocketAddress&) const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    void close() const;

    /** Bind to a port and start listening.
     *@param port 0 means choose an available port.
     *@param backlog maximum number of pending connections.
<<<<<<< HEAD
     *@param certName name of certificate to use to identify the server
     *@return The bound port.
     */
    int listen(uint16_t port = 0, int backlog = 10, const std::string& certName = "localhost.localdomain", bool clientAuth = false) const;
=======
     *@return The bound port.
     */
    int listen(const SocketAddress&, int backlog = 10) const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    /**
     * Accept a connection from a socket that is already listening
     * and has an incoming connection
     */
<<<<<<< HEAD
    SslSocket* accept() const;
=======
    virtual Socket* accept() const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    // TODO The following are raw operations, maybe they need better wrapping?
    int read(void *buf, size_t count) const;
    int write(const void *buf, size_t count) const;

<<<<<<< HEAD
    uint16_t getLocalPort() const;
    uint16_t getRemotePort() const;

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    int getKeyLen() const;
    std::string getClientAuthId() const;

protected:
<<<<<<< HEAD
    mutable std::string connectname;
    mutable PRFileDesc* socket;
    std::string certname;
=======
    mutable PRFileDesc* nssSocket;
    std::string certname;
    mutable std::string url;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    /**
     * 'model' socket, with configuration to use when importing
     * accepted sockets for use as ssl sockets. Set on listen(), used
     * in accept to pass through to newly created socket instances.
     */
    mutable PRFileDesc* prototype;
<<<<<<< HEAD

    SslSocket(IOHandlePrivate* ioph, PRFileDesc* model);
    friend class SslMuxSocket;
=======
    bool hostnameVerification;

    SslSocket(int fd, PRFileDesc* model);
    friend class SslMuxSocket; // Needed for this constructor
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

class SslMuxSocket : public SslSocket
{
public:
<<<<<<< HEAD
=======
    SslMuxSocket(const std::string& certName = "", bool clientAuth = false);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    Socket* accept() const;
};

}}}
#endif  /*!_sys_ssl_Socket_h*/
