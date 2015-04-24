#ifndef QPID_BROKER_REPLICATINGSUBSCRIPTION_H
#define QPID_BROKER_REPLICATINGSUBSCRIPTION_H

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

#include "BrokerInfo.h"
<<<<<<< HEAD
#include "qpid/broker/SemanticState.h"
#include "qpid/broker/QueueObserver.h"
#include "qpid/broker/ConsumerFactory.h"
#include "qpid/types/Uuid.h"
=======
#include "LogPrefix.h"
#include "qpid/broker/SemanticState.h"
#include "qpid/broker/ConsumerFactory.h"
#include "qpid/broker/QueueObserver.h"
#include <boost/enable_shared_from_this.hpp>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <iosfwd>

namespace qpid {

namespace broker {
class Message;
class Queue;
struct QueuedMessage;
class OwnershipToken;
}

namespace framing {
class Buffer;
}

namespace ha {
class QueueGuard;
<<<<<<< HEAD
=======
class HaBroker;
class Event;
class Primary;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

/**
 * A susbcription that replicates to a remote backup.
 *
 * Runs on the primary. In conjunction with a QueueGuard, delays completion of
 * messages till the backup has acknowledged, informs backup of locally dequeued
 * messages.
 *
 * A ReplicatingSubscription is "ready" when all the messages on the queue have
 * either been acknowledged by the backup, or are protected by the queue guard.
 * On a primary broker the ReplicatingSubscription calls Primary::readyReplica
 * when it is ready.
 *
 * THREAD SAFE: Called in subscription's connection thread but also in arbitrary
 * connection threads via dequeued.
 *
 * Lifecycle: broker::Queue holds shared_ptrs to this as a consumer.
 *
<<<<<<< HEAD
 */
class ReplicatingSubscription : public broker::SemanticState::ConsumerImpl
{
  public:
    struct Factory : public broker::ConsumerFactory {
=======
 *  ReplicatingSubscription makes calls on QueueGuard, but not vice-versa.
 */
class ReplicatingSubscription :
        public broker::SemanticState::ConsumerImpl,
        public broker::QueueObserver
{
  public:
    typedef broker::SemanticState::ConsumerImpl ConsumerImpl;

    class Factory : public broker::ConsumerFactory {
      public:
        Factory(HaBroker& hb) : haBroker(hb) {}

        HaBroker& getHaBroker() const { return haBroker; }

>>>>>>> 3bbfc42... Imported Upstream version 0.32
        boost::shared_ptr<broker::SemanticState::ConsumerImpl> create(
            broker::SemanticState* parent,
            const std::string& name, boost::shared_ptr<broker::Queue> ,
            bool ack, bool acquire, bool exclusive, const std::string& tag,
            const std::string& resumeId, uint64_t resumeTtl,
            const framing::FieldTable& arguments);
<<<<<<< HEAD
=======
      private:
        HaBroker& haBroker;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    };

    // Argument names for consume command.
    static const std::string QPID_REPLICATING_SUBSCRIPTION;
<<<<<<< HEAD
    static const std::string QPID_BACK;
    static const std::string QPID_FRONT;
    static const std::string QPID_BROKER_INFO;

    // TODO aconway 2012-05-23: these don't belong on ReplicatingSubscription
    /** Get position of front message on queue.
     *@return false if queue is empty.
     */
    static bool getFront(broker::Queue&, framing::SequenceNumber& result);
    /** Get next message after from in queue.
     *@return false if none found.
     */
    static bool getNext(broker::Queue&, framing::SequenceNumber from,
                        framing::SequenceNumber& result);

    ReplicatingSubscription(broker::SemanticState* parent,
=======
    static const std::string QPID_BROKER_INFO;
    static const std::string QPID_ID_SET;
    // Replicator types: argument values for QPID_REPLICATING_SUBSCRIPTION argument.
    static const std::string QPID_QUEUE_REPLICATOR;
    static const std::string QPID_TX_REPLICATOR;

    ReplicatingSubscription(HaBroker& haBroker,
                            broker::SemanticState* parent,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                            const std::string& name, boost::shared_ptr<broker::Queue> ,
                            bool ack, bool acquire, bool exclusive, const std::string& tag,
                            const std::string& resumeId, uint64_t resumeTtl,
                            const framing::FieldTable& arguments);

    ~ReplicatingSubscription();

<<<<<<< HEAD
    // Called via QueueGuard::dequeued.
    //@return true if the message requires completion.
    void dequeued(const broker::QueuedMessage& qm);

    // Called during initial scan for dequeues.
    void dequeued(framing::SequenceNumber first, framing::SequenceNumber last);

    // Consumer overrides.
    bool deliver(broker::QueuedMessage& msg);
    void cancel();
    void acknowledged(const broker::QueuedMessage&);
    bool browseAcquired() const { return true; }
=======

    // Consumer overrides.
    bool deliver(const broker::QueueCursor& cursor, const broker::Message& msg);
    void cancel();
    void acknowledged(const broker::DeliveryRecord&);
    bool browseAcquired() const { return true; }
    void stopped();

>>>>>>> 3bbfc42... Imported Upstream version 0.32
    // Hide the "queue deleted" error for a ReplicatingSubscription when a
    // queue is deleted, this is normal and not an error.
    bool hideDeletedError() { return true; }

<<<<<<< HEAD
=======
    // QueueObserver overrides
    void enqueued(const broker::Message&) {}
    void dequeued(const broker::Message&);
    void acquired(const broker::Message&) {}
    void requeued(const broker::Message&) {}

    /** A ReplicatingSubscription is a passive observer, not counted for auto
     * deletion and immediate message purposes.
     */
    bool isCounted() { return false; }

>>>>>>> 3bbfc42... Imported Upstream version 0.32
    /** Initialization that must be done separately from construction
     * because it requires a shared_ptr to this to exist.
     */
    void initialize();

    BrokerInfo getBrokerInfo() const { return info; }

<<<<<<< HEAD
=======
    void skipEnqueues(const ReplicationIdSet& ids);
    void skipDequeues(const ReplicationIdSet& ids);

>>>>>>> 3bbfc42... Imported Upstream version 0.32
  protected:
    bool doDispatch();

  private:
<<<<<<< HEAD
    std::string logPrefix;
    boost::shared_ptr<broker::Queue> dummy; // Used to send event messages
    framing::SequenceSet dequeues;
    framing::SequenceNumber backupPosition;
    bool ready;
    BrokerInfo info;
    boost::shared_ptr<QueueGuard> guard;

    void sendDequeueEvent(sys::Mutex::ScopedLock&);
    void sendPositionEvent(framing::SequenceNumber, sys::Mutex::ScopedLock&);
    void setReady();
    void sendEvent(const std::string& key, framing::Buffer&);
  friend struct Factory;
=======
    LogPrefix2 logPrefix;
    QueuePosition position;
    ReplicationIdSet dequeues;  // Dequeues to be sent in next dequeue event.
    ReplicationIdSet skipEnqueue; // Enqueues to skip: messages already on backup and tx enqueues.
    ReplicationIdSet skipDequeue; // Dequeues to skip: tx dequeues.
    ReplicationIdSet unready;   // Unguarded, replicated and un-acknowledged.
    bool wasStopped;
    bool ready;
    bool cancelled;
    BrokerInfo info;
    boost::shared_ptr<QueueGuard> guard;
    HaBroker& haBroker;
    boost::shared_ptr<Primary> primary;

    bool isGuarded(sys::Mutex::ScopedLock&);
    void dequeued(ReplicationId);
    void sendDequeueEvent(sys::Mutex::ScopedLock&);
    void sendIdEvent(ReplicationId, sys::Mutex::ScopedLock&);
    void sendEvent(const Event&, sys::Mutex::ScopedLock&);
    void checkReady(sys::Mutex::ScopedLock&);
  friend class Factory;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};


}} // namespace qpid::broker

#endif  /*!QPID_BROKER_REPLICATINGSUBSCRIPTION_H*/
