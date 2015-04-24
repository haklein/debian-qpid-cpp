#ifndef QPID_HA_QUEUEREPLICATOR_H
#define QPID_HA_QUEUEREPLICATOR_H

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
#include "BrokerInfo.h"
#include "qpid/broker/Exchange.h"
#include "qpid/framing/SequenceSet.h"
#include <boost/enable_shared_from_this.hpp>
=======


#include "BrokerInfo.h"
#include "LogPrefix.h"
#include "hash.h"
#include "qpid/broker/Exchange.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <iosfwd>

namespace qpid {

namespace broker {
class Bridge;
class Link;
class Queue;
class QueueRegistry;
class SessionHandler;
class Deliverable;
<<<<<<< HEAD
}

namespace ha {

/**
 * Exchange created on a backup broker to replicate a queue on the primary.
 *
 * Puts replicated messages on the local queue, handles dequeue events.
 * Creates a ReplicatingSubscription on the primary by passing special
 * arguments to the consume command.
=======
class ExchangeRegistry;
}

namespace ha {
class HaBroker;
class Settings;

/**
 * Exchange created on a backup broker to receive replicated messages and
 * replication events from a queue on the primary. It subscribes to the primary
 * queue via a ReplicatingSubscription on the primary by passing special
 * arguments to the subscribe command.
 *
 * It puts replicated messages on the local replica queue and handles dequeue
 * events by removing local messages.
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 *
 * THREAD SAFE: Called in different connection threads.
 */
class QueueReplicator : public broker::Exchange,
                        public boost::enable_shared_from_this<QueueReplicator>
{
  public:
<<<<<<< HEAD
    static const std::string DEQUEUE_EVENT_KEY;
    static const std::string POSITION_EVENT_KEY;
    static std::string replicatorName(const std::string& queueName);
    /** Test if a string is an event key */
    static bool isEventKey(const std::string key);

    QueueReplicator(const BrokerInfo&,
                    boost::shared_ptr<broker::Queue> q,
                    boost::shared_ptr<broker::Link> l);

    ~QueueReplicator();

    void activate();            // Call after ctor
    void deactivate();          // Call before dtor

    std::string getType() const;
    bool bind(boost::shared_ptr<broker::Queue
              >, const std::string&, const framing::FieldTable*);
    bool unbind(boost::shared_ptr<broker::Queue>, const std::string&, const framing::FieldTable*);
    void route(broker::Deliverable&);
    bool isBound(boost::shared_ptr<broker::Queue>, const std::string* const, const framing::FieldTable* const);

  private:
    void initializeBridge(broker::Bridge& bridge, broker::SessionHandler& sessionHandler);
    void dequeue(framing::SequenceNumber, sys::Mutex::ScopedLock&);

    std::string logPrefix;
    std::string bridgeName;
    sys::Mutex lock;
    boost::shared_ptr<broker::Queue> queue;
    boost::shared_ptr<broker::Link> link;
    boost::shared_ptr<broker::Bridge> bridge;
    BrokerInfo brokerInfo;
};

=======
    static const std::string QPID_SYNC_FREQUENCY;
    static const std::string REPLICATOR_PREFIX;
    typedef std::vector<boost::shared_ptr<QueueReplicator> > Vector;

    static std::string replicatorName(const std::string& queueName);
    static bool isReplicatorName(const std::string&);
    /*** Copy QueueReplicators from the registry */
    static void copy(broker::ExchangeRegistry&, Vector& result);

    static boost::shared_ptr<QueueReplicator> create(
        HaBroker&, boost::shared_ptr<broker::Queue> q, boost::shared_ptr<broker::Link> l);

    ~QueueReplicator();

    void disconnect();      // Called when we are disconnected from the primary.

    virtual std::string getType() const;

    void route(broker::Deliverable&);

    // Called via QueueObserver
    void enqueued(const broker::Message&);
    void dequeued(const broker::Message&);

    // Set if the queue has ever been subscribed to, used for auto-delete cleanup.
    void setSubscribed() { subscribed = true; }

    boost::shared_ptr<broker::Queue> getQueue() const { return queue; }

    // No-op unused Exchange virtual functions.
    bool bind(boost::shared_ptr<broker::Queue>, const std::string&, const framing::FieldTable*);
    bool unbind(boost::shared_ptr<broker::Queue>, const std::string&, const framing::FieldTable*);
    bool isBound(boost::shared_ptr<broker::Queue>, const std::string* const, const framing::FieldTable* const);
    bool hasBindings();

    void promoted();

  protected:
    typedef boost::function<void(const std::string&, sys::Mutex::ScopedLock&)> DispatchFn;
    typedef qpid::sys::unordered_map<std::string, DispatchFn> DispatchMap;

    QueueReplicator(
        HaBroker&, boost::shared_ptr<broker::Queue>, boost::shared_ptr<broker::Link>);

    void initialize();          // Called as part of create()

    virtual void deliver(const broker::Message&);

    virtual void destroy();             // Called when the queue is destroyed.
    virtual void destroy(sys::Mutex::ScopedLock&);

    sys::Mutex lock;
    HaBroker& haBroker;
    const BrokerInfo brokerInfo;
    DispatchMap dispatch;
    boost::shared_ptr<broker::Link> link;
    boost::weak_ptr<broker::Bridge> bridge;
    boost::shared_ptr<broker::Queue> queue;
    broker::SessionHandler* sessionHandler;

  private:
    typedef qpid::sys::unordered_map<
      ReplicationId, QueuePosition, Hasher<ReplicationId> > PositionMap;
    class ErrorListener;
    class QueueObserver;

    void initializeBridge(broker::Bridge& bridge, broker::SessionHandler& sessionHandler);

    // Dispatch functions
    void dequeueEvent(const std::string& data, sys::Mutex::ScopedLock&);
    void idEvent(const std::string& data, sys::Mutex::ScopedLock&);

    bool deletedOnPrimary(framing::execution::ErrorCode e, const std::string& msg);

    LogPrefix2 logPrefix;
    std::string bridgeName;

    bool subscribed;
    const Settings& settings;
    PositionMap positions;
    ReplicationIdSet idSet; // Set of replicationIds on the queue.
    ReplicationId nextId;   // ID for next message to arrive.
    ReplicationId maxId;    // Max ID used so far.

  friend class ErrorListener;
};


>>>>>>> 3bbfc42... Imported Upstream version 0.32
}} // namespace qpid::ha

#endif  /*!QPID_HA_QUEUEREPLICATOR_H*/
