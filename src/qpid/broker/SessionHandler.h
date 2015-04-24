#ifndef QPID_BROKER_SESSIONHANDLER_H
#define QPID_BROKER_SESSIONHANDLER_H

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
<<<<<<< HEAD

=======
#include "qpid/broker/BrokerImportExport.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/amqp_0_10/SessionHandler.h"
#include "qpid/broker/SessionHandler.h"
#include "qpid/framing/AMQP_ClientProxy.h"
#include <boost/shared_ptr.hpp>

namespace qpid {
class SessionState;

namespace broker {
<<<<<<< HEAD

class Connection;
class ConnectionState;
=======
namespace amqp_0_10 {
class Connection;
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
class SessionState;

/**
 * A SessionHandler is associated with each active channel. It
 * receives incoming frames, handles session controls and manages the
 * association between the channel and a session.
 */
<<<<<<< HEAD
class SessionHandler : public amqp_0_10::SessionHandler {
=======
class SessionHandler : public qpid::amqp_0_10::SessionHandler {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
  public:
    class ErrorListener {
      public:
        virtual ~ErrorListener() {}
<<<<<<< HEAD
        virtual void connectionException(
            framing::connection::CloseCode code, const std::string& msg) = 0;
        virtual void channelException(
            framing::session::DetachCode, const std::string& msg) = 0;
        virtual void executionException(
            framing::execution::ErrorCode, const std::string& msg) = 0;
=======

        /** Called when there is an outgoing connection-exception */
        virtual void connectionException(
            framing::connection::CloseCode code, const std::string& msg) = 0;
        /** Called when there is an outgoing channel-exception */
        virtual void channelException(
            framing::session::DetachCode, const std::string& msg) = 0;
        /** Called when there is an outgoing execution-exception */
        virtual void executionException(
            framing::execution::ErrorCode, const std::string& msg) = 0;

        /** Called when there is an incoming execution-exception.
         * Useful for inter-broker bridges.
         */
        virtual void incomingExecutionException(
            framing::execution::ErrorCode, const std::string& msg) = 0;

>>>>>>> 3bbfc42... Imported Upstream version 0.32
        /** Called when it is safe to delete the ErrorListener. */
        virtual void detach() = 0;
    };

    /**
     *@param e must not be deleted until ErrorListener::detach has been called */
<<<<<<< HEAD
    SessionHandler(Connection&, framing::ChannelId);
=======
    SessionHandler(amqp_0_10::Connection&, framing::ChannelId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    ~SessionHandler();

    /** Get broker::SessionState */
    SessionState* getSession() { return session.get(); }
    const SessionState* getSession() const { return session.get(); }

<<<<<<< HEAD
    ConnectionState& getConnection();
    const ConnectionState& getConnection() const;
=======
    QPID_BROKER_EXTERN amqp_0_10::Connection& getConnection();
    QPID_BROKER_EXTERN const amqp_0_10::Connection& getConnection() const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    framing::AMQP_ClientProxy& getProxy() { return proxy; }
    const framing::AMQP_ClientProxy& getProxy() const { return proxy; }

<<<<<<< HEAD
    /**
     * If commands are sent based on the local time (e.g. in timers), they don't have
     * a well-defined ordering across cluster nodes.
     * This proxy is for sending such commands. In a clustered broker it will take steps
     * to synchronize command order across the cluster. In a stand-alone broker
     * it is just a synonym for getProxy()
     */
    framing::AMQP_ClientProxy& getClusterOrderProxy() {
        return clusterOrderProxy.get() ? *clusterOrderProxy : proxy;
    }

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    virtual void handleDetach();
    void attached(const std::string& name);//used by 'pushing' inter-broker bridges
    void attachAs(const std::string& name);//used by 'pulling' inter-broker bridges

<<<<<<< HEAD
    void setErrorListener(boost::shared_ptr<ErrorListener> e) { errorListener = e; }

  protected:
    virtual void setState(const std::string& sessionName, bool force);
    virtual qpid::SessionState* getState();
    virtual framing::FrameHandler* getInHandler();
    virtual void connectionException(framing::connection::CloseCode code, const std::string& msg);
    virtual void channelException(framing::session::DetachCode, const std::string& msg);
    virtual void executionException(framing::execution::ErrorCode, const std::string& msg);
    virtual void detaching();
    virtual void readyToSend();
=======
    QPID_BROKER_EXTERN void setErrorListener(boost::shared_ptr<ErrorListener> e) { errorListener = e; }

    // Called by SessionAdapter
    void incomingExecutionException(framing::execution::ErrorCode, const std::string& msg);

  protected:
    void setState(const std::string& sessionName, bool force);
    qpid::SessionState* getState();
    framing::FrameHandler* getInHandler();
    void connectionException(framing::connection::CloseCode code, const std::string& msg);
    void channelException(framing::session::DetachCode, const std::string& msg);
    void executionException(framing::execution::ErrorCode, const std::string& msg);
    void detaching();
    void readyToSend();
>>>>>>> 3bbfc42... Imported Upstream version 0.32

  private:
    struct SetChannelProxy : public framing::AMQP_ClientProxy { // Proxy that sets the channel.
        framing::ChannelHandler setChannel;
        SetChannelProxy(uint16_t ch, framing::FrameHandler* out)
            : framing::AMQP_ClientProxy(setChannel), setChannel(ch, out) {}
    };

<<<<<<< HEAD
    Connection& connection;
    framing::AMQP_ClientProxy proxy;
    std::auto_ptr<SessionState> session;
    std::auto_ptr<SetChannelProxy> clusterOrderProxy;
=======
    amqp_0_10::Connection& connection;
    framing::AMQP_ClientProxy proxy;
    std::auto_ptr<SessionState> session;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    boost::shared_ptr<ErrorListener> errorListener;
};

}} // namespace qpid::broker



#endif  /*!QPID_BROKER_SESSIONHANDLER_H*/
