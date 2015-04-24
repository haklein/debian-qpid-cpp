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

#include "qpid/broker/Queue.h"
<<<<<<< HEAD

#include "qpid/broker/Broker.h"
#include "qpid/broker/QueueEvents.h"
#include "qpid/broker/Exchange.h"
#include "qpid/broker/Fairshare.h"
#include "qpid/broker/DeliverableMessage.h"
#include "qpid/broker/LegacyLVQ.h"
#include "qpid/broker/MessageDeque.h"
#include "qpid/broker/MessageMap.h"
#include "qpid/broker/MessageStore.h"
#include "qpid/broker/NullMessageStore.h"
#include "qpid/broker/QueueRegistry.h"
#include "qpid/broker/QueueFlowLimit.h"
#include "qpid/broker/ThresholdAlerts.h"
#include "qpid/broker/FifoDistributor.h"
#include "qpid/broker/MessageGroupManager.h"

=======
#include "qpid/broker/Broker.h"
#include "qpid/broker/Connection.h"
#include "qpid/broker/AclModule.h"
#include "qpid/broker/QueueCursor.h"
#include "qpid/broker/QueueDepth.h"
#include "qpid/broker/QueueSettings.h"
#include "qpid/broker/Exchange.h"
#include "qpid/broker/DeliverableMessage.h"
#include "qpid/broker/MessageStore.h"
#include "qpid/broker/MessageDeque.h"
#include "qpid/broker/MessageDistributor.h"
#include "qpid/broker/FifoDistributor.h"
#include "qpid/broker/NullMessageStore.h"
#include "qpid/broker/QueueRegistry.h"
#include "qpid/broker/Selector.h"
#include "qpid/broker/TransactionObserver.h"
#include "qpid/broker/TxDequeue.h"

//TODO: get rid of this
#include "qpid/broker/amqp_0_10/MessageTransfer.h"

#include "qpid/amqp_0_10/Codecs.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/StringUtils.h"
#include "qpid/log/Statement.h"
#include "qpid/management/ManagementAgent.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/FieldValue.h"
<<<<<<< HEAD
#include "qpid/sys/ClusterSafe.h"
#include "qpid/sys/Monitor.h"
#include "qpid/sys/Time.h"
=======
#include "qpid/sys/Monitor.h"
#include "qpid/sys/Time.h"
#include "qpid/sys/Timer.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/types/Variant.h"
#include "qmf/org/apache/qpid/broker/ArgsQueuePurge.h"
#include "qmf/org/apache/qpid/broker/ArgsQueueReroute.h"
#include "qmf/org/apache/qpid/broker/EventQueueDelete.h"
<<<<<<< HEAD
=======
#include "qmf/org/apache/qpid/broker/EventSubscribe.h"
#include "qmf/org/apache/qpid/broker/EventUnsubscribe.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/bind.hpp>
#include <boost/intrusive_ptr.hpp>


namespace qpid {
namespace broker {

using namespace qpid::sys;
using namespace qpid::framing;
using qpid::management::ManagementAgent;
using qpid::management::ManagementObject;
using qpid::management::Manageable;
using qpid::management::Args;
<<<<<<< HEAD
=======
using qpid::management::getCurrentPublisher;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
using std::string;
using std::for_each;
using std::mem_fun;
namespace _qmf = qmf::org::apache::qpid::broker;


namespace
{
<<<<<<< HEAD
const std::string qpidMaxSize("qpid.max_size");
const std::string qpidMaxCount("qpid.max_count");
const std::string qpidNoLocal("no-local");
const std::string qpidTraceIdentity("qpid.trace.id");
const std::string qpidTraceExclude("qpid.trace.exclude");
const std::string qpidLastValueQueueKey("qpid.last_value_queue_key");
const std::string qpidLastValueQueue("qpid.last_value_queue");
const std::string qpidLastValueQueueNoBrowse("qpid.last_value_queue_no_browse");
const std::string qpidPersistLastNode("qpid.persist_last_node");
const std::string qpidVQMatchProperty("qpid.LVQ_key");
const std::string qpidQueueEventGeneration("qpid.queue_event_generation");
const std::string qpidAutoDeleteTimeout("qpid.auto_delete_timeout");
//following feature is not ready for general use as it doesn't handle
//the case where a message is enqueued on more than one queue well enough:
const std::string qpidInsertSequenceNumbers("qpid.insert_sequence_numbers");

const int ENQUEUE_ONLY=1;
const int ENQUEUE_AND_DEQUEUE=2;

inline void mgntEnqStats(const boost::intrusive_ptr<Message>& msg,
			 _qmf::Queue* mgmtObject,
			 _qmf::Broker* brokerMgmtObject)
=======

inline void mgntEnqStats(const Message& msg,
			 _qmf::Queue::shared_ptr mgmtObject,
			 _qmf::Broker::shared_ptr brokerMgmtObject)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    if (mgmtObject != 0) {
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();

<<<<<<< HEAD
        uint64_t contentSize = msg->contentSize();
=======
        uint64_t contentSize = msg.getMessageSize();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        qStats->msgTotalEnqueues +=1;
        bStats->msgTotalEnqueues += 1;
        qStats->byteTotalEnqueues += contentSize;
        bStats->byteTotalEnqueues += contentSize;
<<<<<<< HEAD
        if (msg->isPersistent ()) {
=======
        if (msg.isPersistent ()) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            qStats->msgPersistEnqueues += 1;
            bStats->msgPersistEnqueues += 1;
            qStats->bytePersistEnqueues += contentSize;
            bStats->bytePersistEnqueues += contentSize;
        }
        mgmtObject->statisticsUpdated();
        brokerMgmtObject->statisticsUpdated();
    }
}

<<<<<<< HEAD
inline void mgntDeqStats(const boost::intrusive_ptr<Message>& msg,
			 _qmf::Queue* mgmtObject,
			 _qmf::Broker* brokerMgmtObject)
=======
inline void mgntDeqStats(const Message& msg,
			 _qmf::Queue::shared_ptr mgmtObject,
			 _qmf::Broker::shared_ptr brokerMgmtObject)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    if (mgmtObject != 0){
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();
<<<<<<< HEAD
        uint64_t contentSize = msg->contentSize();
=======
        uint64_t contentSize = msg.getMessageSize();
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        qStats->msgTotalDequeues += 1;
        bStats->msgTotalDequeues += 1;
        qStats->byteTotalDequeues += contentSize;
        bStats->byteTotalDequeues += contentSize;
<<<<<<< HEAD
        if (msg->isPersistent ()){
=======
        if (msg.isPersistent ()){
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            qStats->msgPersistDequeues += 1;
            bStats->msgPersistDequeues += 1;
            qStats->bytePersistDequeues += contentSize;
            bStats->bytePersistDequeues += contentSize;
        }
        mgmtObject->statisticsUpdated();
        brokerMgmtObject->statisticsUpdated();
    }
}

<<<<<<< HEAD
} // namespace

Queue::Queue(const string& _name, bool _autodelete,
             MessageStore* const _store,
             const OwnershipToken* const _owner,
=======
QueueSettings merge(const QueueSettings& inputs, const Broker& broker)
{
    QueueSettings settings(inputs);
    settings.maxDepth = QueueDepth();
    if (inputs.maxDepth.hasCount() && inputs.maxDepth.getCount()) {
        settings.maxDepth.setCount(inputs.maxDepth.getCount());
    }
    if (inputs.maxDepth.hasSize()) {
        if (inputs.maxDepth.getSize()) {
            settings.maxDepth.setSize(inputs.maxDepth.getSize());
        }
    } else if (broker.getQueueLimit()) {
        settings.maxDepth.setSize(broker.getQueueLimit());
    }
    return settings;
}

}


Queue::TxPublish::TxPublish(const Message& m, boost::shared_ptr<Queue> q) : message(m), queue(q), prepared(false) {}
bool Queue::TxPublish::prepare(TransactionContext* ctxt) throw()
{
    try {
        prepared = queue->enqueue(ctxt, message);
        return true;
    } catch (const std::exception& e) {
        QPID_LOG(error, "Failed to prepare: " << e.what());
        return false;
    }
}
void Queue::TxPublish::commit() throw()
{
    try {
        if (prepared) queue->process(message);
    } catch (const std::exception& e) {
        QPID_LOG(error, "Failed to commit: " << e.what());
    }
}
void Queue::TxPublish::rollback() throw()
{
    try {
        if (prepared) queue->enqueueAborted(message);
    } catch (const std::exception& e) {
        QPID_LOG(error, "Failed to rollback: " << e.what());
    }
}

void Queue::TxPublish::callObserver(
    const boost::shared_ptr<TransactionObserver>& observer)
{
    observer->enqueue(queue, message);
}

Queue::Queue(const string& _name, const QueueSettings& _settings,
             MessageStore* const _store,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
             Manageable* parent,
             Broker* b) :

    name(_name),
<<<<<<< HEAD
    autodelete(_autodelete),
    store(_store),
    owner(_owner),
    consumerCount(0),
    browserCount(0),
    exclusive(0),
    noLocal(false),
    persistLastNode(false),
    inLastNodeFailure(false),
    messages(new MessageDeque()),
    persistenceId(0),
    policyExceeded(false),
    mgmtObject(0),
    brokerMgmtObject(0),
    eventMode(0),
    insertSeqNo(0),
    broker(b),
    deleted(false),
    barrier(*this),
    autoDeleteTimeout(0),
    allocator(new FifoDistributor( *messages ))
{
    if (parent != 0 && broker != 0) {
        ManagementAgent* agent = broker->getManagementAgent();

        if (agent != 0) {
            mgmtObject = new _qmf::Queue(agent, this, parent, _name, _store != 0, _autodelete, _owner != 0);
            agent->addObject(mgmtObject, 0, store != 0);
            brokerMgmtObject = (qmf::org::apache::qpid::broker::Broker*) broker->GetManagementObject();
=======
    store(_store),
    owner(0),
    exclusive(0),
    messages(new MessageDeque()),
    persistenceId(0),
    settings(b ? merge(_settings, *b) : _settings),
    eventMode(0),
    observers(name, messageLock),
    broker(b),
    deleted(false),
    barrier(*this),
    allocator(new FifoDistributor( *messages )),
    redirectSource(false)
{
    current.setCount(0);//always track depth in messages
    if (settings.maxDepth.getSize()) current.setSize(0);//track depth in bytes only if policy requires it
    if (settings.traceExcludes.size()) {
        split(traceExclude, settings.traceExcludes, ", ");
    }
    qpid::amqp_0_10::translate(settings.asMap(), encodableSettings);
    if (parent != 0 && broker != 0) {
        ManagementAgent* agent = broker->getManagementAgent();
        if (agent != 0) {
            mgmtObject = _qmf::Queue::shared_ptr(
                new _qmf::Queue(agent, this, parent, _name, _store != 0, settings.autodelete));
            mgmtObject->set_arguments(settings.asMap());
            agent->addObject(mgmtObject, 0, store != 0);
            brokerMgmtObject = boost::dynamic_pointer_cast<_qmf::Broker>(broker->GetManagementObject());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            if (brokerMgmtObject)
                brokerMgmtObject->inc_queueCount();
        }
    }
<<<<<<< HEAD
}

Queue::~Queue()
{
    if (mgmtObject != 0) {
        mgmtObject->resourceDestroy();
        if (brokerMgmtObject)
            brokerMgmtObject->dec_queueCount();
    }
}

bool isLocalTo(const OwnershipToken* token, boost::intrusive_ptr<Message>& msg)
{
    return token && token->isLocal(msg->getPublisher());
}

bool Queue::isLocal(boost::intrusive_ptr<Message>& msg)
=======

    if ( settings.isBrowseOnly ) {
        QPID_LOG ( info, "Queue " << name << " is browse-only." );
    }
    if (settings.filter.size()) {
        selector.reset(new Selector(settings.filter));
        QPID_LOG (info, "Queue " << name << " using filter: " << settings.filter);
    }
}

Queue::~Queue()
{
    if (mgmtObject != 0)
        mgmtObject->debugStats("destroying");
}

bool Queue::isLocal(const Message& msg)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    //message is considered local if it was published on the same
    //connection as that of the session which declared this queue
    //exclusive (owner) or which has an exclusive subscription
    //(exclusive)
<<<<<<< HEAD
    return noLocal && (isLocalTo(owner, msg) || isLocalTo(exclusive, msg));
}

bool Queue::isExcluded(boost::intrusive_ptr<Message>& msg)
{
    return traceExclude.size() && msg->isExcluded(traceExclude);
}

void Queue::deliver(boost::intrusive_ptr<Message> msg){
    // Check for deferred delivery in a cluster.
    if (broker && broker->deferDelivery(name, msg))
        return;
    if (msg->isImmediate() && getConsumerCount() == 0) {
        if (alternateExchange) {
            DeliverableMessage deliverable(msg);
            alternateExchange->route(deliverable);
        }
    } else if (isLocal(msg)) {
        //drop message
        QPID_LOG(info, "Dropping 'local' message from " << getName());
    } else if (isExcluded(msg)) {
        //drop message
        QPID_LOG(info, "Dropping excluded message from " << getName());
    } else {
        enqueue(0, msg);
        push(msg);
        QPID_LOG(debug, "Message " << msg << " enqueued on " << name);
    }
}

void Queue::recoverPrepared(boost::intrusive_ptr<Message>& msg)
{
    Mutex::ScopedLock locker(messageLock);
    if (policy.get()) policy->recoverEnqueued(msg);
}

void Queue::recover(boost::intrusive_ptr<Message>& msg)
{
    {
        Mutex::ScopedLock locker(messageLock);
        if (policy.get()) policy->recoverEnqueued(msg);
    }

    push(msg, true);
    if (store){
        // setup synclist for recovered messages, so they don't get re-stored on lastNodeFailure
        msg->addToSyncList(shared_from_this(), store);
    }

    if (store && (!msg->isContentLoaded() || msg->checkContentReleasable())) {
        //content has not been loaded, need to ensure that lazy loading mode is set:
        //TODO: find a nicer way to do this
        msg->releaseContent(store);
        // NOTE: The log message in this section are used for flow-to-disk testing (which checks the log for the
        // presence of this message). Do not change this without also checking these tests.
        QPID_LOG(debug, "Message id=\"" << msg->getProperties<MessageProperties>()->getMessageId() << "\"; pid=0x" <<
                        std::hex << msg->getPersistenceId() << std::dec << ": Content released after recovery");
    }
}

void Queue::process(boost::intrusive_ptr<Message>& msg){
    push(msg);
    if (mgmtObject != 0){
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        const uint64_t contentSize = msg->contentSize();
=======
    return settings.noLocal && (msg.isLocalTo(owner) || msg.isLocalTo(exclusive));
}

bool Queue::isExcluded(const Message& msg)
{
    return traceExclude.size() && msg.isExcluded(traceExclude);
}

bool Queue::accept(const Message& msg)
{
    //TODO: move some of this out of the queue and into the publishing
    //'link' for whatever protocol is used; that would let protocol
    //specific stuff be kept out the queue
    if (broker::amqp_0_10::MessageTransfer::isImmediateDeliveryRequired(msg) && getConsumerCount() == 0) {
        if (alternateExchange) {
            DeliverableMessage deliverable(msg, 0);
            alternateExchange->route(deliverable);
        }
        return false;
    } else if (isLocal(msg)) {
        //drop message
        QPID_LOG(info, "Dropping 'local' message from " << getName());
        return false;
    } else if (isExcluded(msg)) {
        //drop message
        QPID_LOG(info, "Dropping excluded message from " << getName());
        return false;
    } else if (selector) {
        return selector->filter(msg);
    } else {
        return true;
    }
}

void Queue::deliver(Message msg, TxBuffer* txn)
{
    if (redirectPeer) {
        redirectPeer->deliverTo(msg, txn);
    } else {
        deliverTo(msg, txn);
    }
}

void Queue::deliverTo(Message msg, TxBuffer* txn)
{
    if (accept(msg)) {
        interceptors.record(msg);
        if (txn) {
            TxOp::shared_ptr op(new TxPublish(msg, shared_from_this()));
            txn->enlist(op);
        } else {
            if (enqueue(0, msg)) {
                push(msg);
                QPID_LOG(debug, "Message " << msg.getSequence() << " enqueued on " << name);
            } else {
                QPID_LOG(debug, "Message " << msg.getSequence() << " dropped from " << name);
            }
        }
    }
}

void Queue::recoverPrepared(const Message& msg)
{
    Mutex::ScopedLock locker(messageLock);
    current += QueueDepth(1, msg.getMessageSize());
}

void Queue::recover(Message& msg)
{
    recoverPrepared(msg);
    push(msg, true);
}

void Queue::process(Message& msg)
{
    push(msg);
    if (mgmtObject != 0){
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        const uint64_t contentSize = msg.getMessageSize();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        qStats->msgTxnEnqueues  += 1;
        qStats->byteTxnEnqueues += contentSize;
        mgmtObject->statisticsUpdated();
        if (brokerMgmtObject) {
            _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();
            bStats->msgTxnEnqueues += 1;
            bStats->byteTxnEnqueues += contentSize;
            brokerMgmtObject->statisticsUpdated();
        }
    }
}

<<<<<<< HEAD
void Queue::requeue(const QueuedMessage& msg){
    assertClusterSafe();
    QueueListeners::NotificationSet copy;
    {
        if (!isEnqueued(msg)) return;
        if (deleted) {
            //
            // If the queue has been deleted, requeued messages must be sent to the alternate exchange
            // if one is configured.
            //
            if (alternateExchange.get()) {
                DeliverableMessage dmsg(msg.payload);
                alternateExchange->routeWithAlternate(dmsg);
                if (brokerMgmtObject)
                    brokerMgmtObject->inc_abandonedViaAlt();
            } else {
                if (brokerMgmtObject)
                    brokerMgmtObject->inc_abandoned();
            }
            mgntDeqStats(msg.payload, mgmtObject, brokerMgmtObject);
        } else {
            {
                Mutex::ScopedLock locker(messageLock);
                messages->release(msg);
                observeRequeue(msg, locker);
                listeners.populate(copy);
            }

            if (mgmtObject) {
                mgmtObject->inc_releases();
                if (brokerMgmtObject)
                    brokerMgmtObject->inc_releases();
            }

            // for persistLastNode - don't force a message twice to disk, but force it if no force before
            if(inLastNodeFailure && persistLastNode && !msg.payload->isStoredOnQueue(shared_from_this())) {
                msg.payload->forcePersistent();
                if (msg.payload->isForcedPersistent() ){
                    boost::intrusive_ptr<Message> payload = msg.payload;
                    enqueue(0, payload);
=======
void Queue::release(const QueueCursor& position, bool markRedelivered)
{
    QueueListeners::NotificationSet copy;
    {
        Mutex::ScopedLock locker(messageLock);
        if (!deleted) {
            Message* message = messages->release(position);
            if (message) {
                if (!markRedelivered) message->undeliver();
                listeners.populate(copy);
                observeRequeue(*message, locker);
                if (mgmtObject) {
                    mgmtObject->inc_releases();
                    if (brokerMgmtObject)
                        brokerMgmtObject->inc_releases();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                }
            }
        }
    }
    copy.notify();
}

<<<<<<< HEAD
bool Queue::acquireMessageAt(const SequenceNumber& position, QueuedMessage& message)
{
    assertClusterSafe();
    QPID_LOG(debug, "Attempting to acquire message at " << position);
    if (acquire(position, message)) {
        QPID_LOG(debug, "Acquired message at " << position << " from " << name);
        return true;
    } else {
        QPID_LOG(debug, "Could not acquire message at " << position << " from " << name << "; no message at that position");
        return false;
    }
}

bool Queue::acquire(const QueuedMessage& msg, const std::string& consumer)
{
    assertClusterSafe();
    QPID_LOG(debug, consumer << " attempting to acquire message at " << msg.position);
    bool ok;
    {
        Mutex::ScopedLock locker(messageLock);
        ok = allocator->allocate( consumer, msg );
    }
    if (!ok) {
        QPID_LOG(debug, "Not permitted to acquire msg at " << msg.position << " from '" << name);
        return false;
    }

    QueuedMessage copy(msg);
    if (acquire( msg.position, copy)) {
        QPID_LOG(debug, "Acquired message at " << msg.position << " from " << name);
        return true;
    }
    QPID_LOG(debug, "Could not acquire message at " << msg.position << " from " << name << "; no message at that position");
    return false;
}

void Queue::notifyListener()
{
    assertClusterSafe();
    QueueListeners::NotificationSet set;
    {
        Mutex::ScopedLock locker(messageLock);
        if (messages->size()) {
            listeners.populate(set);
        }
    }
    set.notify();
}

bool Queue::getNextMessage(QueuedMessage& m, Consumer::shared_ptr& c)
{
    checkNotDeleted(c);
    if (c->preAcquires()) {
        switch (consumeNextMessage(m, c)) {
          case CONSUMED:
            return true;
          case CANT_CONSUME:
            notifyListener();//let someone else try
          case NO_MESSAGES:
          default:
            return false;
        }
    } else {
        return browseNextMessage(m, c);
    }
}

Queue::ConsumeCode Queue::consumeNextMessage(QueuedMessage& m, Consumer::shared_ptr& c)
{
    while (true) {
        QueuedMessage msg;
        bool found;
        {
            Mutex::ScopedLock locker(messageLock);
            found = allocator->nextConsumableMessage(c, msg);
            if (!found) listeners.addListener(c);
        }
        if (!found) {
            QPID_LOG(debug, "No messages to dispatch on queue '" << name << "'");
            return NO_MESSAGES;
        }

        if (msg.payload->hasExpired()) {
            QPID_LOG(debug, "Message expired from queue '" << name << "'");
            c->setPosition(msg.position);
            dequeue(0, msg);
            if (mgmtObject) {
                mgmtObject->inc_discardsTtl();
                if (brokerMgmtObject)
                    brokerMgmtObject->inc_discardsTtl();
            }
            continue;
        }

        if (c->filter(msg.payload)) {
            if (c->accept(msg.payload)) {
                {
                    Mutex::ScopedLock locker(messageLock);
                    bool ok = allocator->allocate( c->getName(), msg );  // inform allocator
                    (void) ok; assert(ok);
                    observeAcquire(msg, locker);
                }
                if (mgmtObject) {
                    mgmtObject->inc_acquires();
                    if (brokerMgmtObject)
                        brokerMgmtObject->inc_acquires();
                }
                m = msg;
                return CONSUMED;
            } else {
                //message(s) are available but consumer hasn't got enough credit
                QPID_LOG(debug, "Consumer can't currently accept message from '" << name << "'");
            }
        } else {
            //consumer will never want this message
            QPID_LOG(debug, "Consumer doesn't want message from '" << name << "'");
        }

        Mutex::ScopedLock locker(messageLock);
        messages->release(msg);
        return CANT_CONSUME;
    }
}

bool Queue::browseNextMessage(QueuedMessage& m, Consumer::shared_ptr& c)
{
    while (true) {
        QueuedMessage msg;
        bool found;
        {
            Mutex::ScopedLock locker(messageLock);
            found = allocator->nextBrowsableMessage(c, msg);
            if (!found) listeners.addListener(c);
        }
        if (!found) { // no next available
            QPID_LOG(debug, "No browsable messages available for consumer " <<
                     c->getName() << " on queue '" << name << "'");
            return false;
        }

        if (c->filter(msg.payload) && !msg.payload->hasExpired()) {
            if (c->accept(msg.payload)) {
                //consumer wants the message
                c->setPosition(msg.position);
                m = msg;
                return true;
            } else {
                //browser hasn't got enough credit for the message
                QPID_LOG(debug, "Browser can't currently accept message from '" << name << "'");
                return false;
            }
        } else {
            //consumer will never want this message, continue seeking
            QPID_LOG(debug, "Browser skipping message from '" << name << "'");
            c->setPosition(msg.position);
        }
    }
    return false;
=======
bool Queue::dequeueMessageAt(const SequenceNumber& position)
{
    ScopedAutoDelete autodelete(*this);
    boost::intrusive_ptr<PersistableMessage> pmsg;
    {
        Mutex::ScopedLock locker(messageLock);
        QPID_LOG(debug, "Attempting to dequeue message at " << position);
        QueueCursor cursor;
        Message* msg = messages->find(position, &cursor);
        if (msg) {
            if (msg->isPersistent()) pmsg = msg->getPersistentContext();
            observeDequeue(*msg, locker, settings.autodelete ? &autodelete : 0);
            messages->deleted(cursor);
        } else {
            QPID_LOG(debug, "Could not dequeue message at " << position << "; no such message");
            return false;
        }
    }
    dequeueFromStore(pmsg);
    return true;
}

bool Queue::acquire(const QueueCursor& position, const std::string& consumer)
{
    Mutex::ScopedLock locker(messageLock);
    Message* msg;

    msg = messages->find(position);
    if (msg) {
        QPID_LOG(debug, consumer << " attempting to acquire message at " << msg->getSequence());
        if (!allocator->acquire(consumer, *msg)) {
            QPID_LOG(debug, "Not permitted to acquire msg at " << msg->getSequence() << " from '" << name);
            return false;
        } else {
            observeAcquire(*msg, locker);
            QPID_LOG(debug, "Acquired message at " << msg->getSequence() << " from " << name);
            return true;
        }
    } else {
        QPID_LOG(debug, "Failed to acquire message which no longer exists on " << name);
        return false;
    }
}

bool Queue::getNextMessage(Message& m, Consumer::shared_ptr& c)
{
    if (!checkNotDeleted(c)) return false;
    QueueListeners::NotificationSet set;
    ScopedAutoDelete autodelete(*this);
    bool messageFound(false);
    while (true) {
        //TODO: reduce lock scope
        Mutex::ScopedLock locker(messageLock);
        QueueCursor cursor = c->getCursor(); // Save current position.
        Message* msg = messages->next(*c);   // Advances c.
        if (msg) {
            if (msg->getExpiration() < sys::AbsTime::now()) {
                QPID_LOG(debug, "Message expired from queue '" << name << "'");
                observeDequeue(*msg, locker, settings.autodelete ? &autodelete : 0);
                //ERROR: don't hold lock across call to store!!
                if (msg->isPersistent()) dequeueFromStore(msg->getPersistentContext());
                if (mgmtObject) {
                    mgmtObject->inc_discardsTtl();
                    if (brokerMgmtObject)
                        brokerMgmtObject->inc_discardsTtl();
                }
                messages->deleted(*c);
                continue;
            }

            if (c->filter(*msg)) {
                if (c->accept(*msg)) {
                    if (c->preAcquires()) {
                        QPID_LOG(debug, "Attempting to acquire message " << msg->getSequence()
                                 << " from '" << name << "' with state " << msg->getState());
                        if (allocator->acquire(c->getName(), *msg)) {
                            if (mgmtObject) {
                                mgmtObject->inc_acquires();
                                if (brokerMgmtObject)
                                    brokerMgmtObject->inc_acquires();
                            }
                            observeAcquire(*msg, locker);
                            msg->deliver();
                        } else {
                            QPID_LOG(debug, "Could not acquire message from '" << name << "'");
                            continue; //try another message
                        }
                    }
                    QPID_LOG(debug, "Message " << msg->getSequence() << " retrieved from '"
                             << name << "'");
                    m = *msg;
                    messageFound = true;
                    break;
                } else {
                    //message(s) are available but consumer hasn't got enough credit
                    QPID_LOG(debug, "Consumer can't currently accept message from '" << name << "'");
                    c->setCursor(cursor); // Restore cursor, will try again with credit
                    if (c->preAcquires()) {
                        //let someone else try
                        listeners.populate(set);
                    }
                    break;
                }
            } else {
                //consumer will never want this message, try another one
                QPID_LOG(debug, "Consumer doesn't want message from '" << name << "'");
                if (c->preAcquires()) {
                    //let someone else try to take this one
                    listeners.populate(set);
                }
            }
        } else {
            QPID_LOG(debug, "No messages to dispatch on queue '" << name << "'");
            c->stopped();
            listeners.addListener(c);
            break;
        }

    }
    set.notify();
    return messageFound;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Queue::removeListener(Consumer::shared_ptr c)
{
    QueueListeners::NotificationSet set;
    {
        Mutex::ScopedLock locker(messageLock);
        listeners.removeListener(c);
        if (messages->size()) {
            listeners.populate(set);
        }
    }
    set.notify();
}

bool Queue::dispatch(Consumer::shared_ptr c)
{
<<<<<<< HEAD
    QueuedMessage msg(this);
    if (getNextMessage(msg, c)) {
        c->deliver(msg);
=======
    Message msg;
    if (getNextMessage(msg, c)) {
        c->deliver(*c, msg);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return true;
    } else {
        return false;
    }
}

<<<<<<< HEAD
bool Queue::find(SequenceNumber pos, QueuedMessage& msg) const {
    Mutex::ScopedLock locker(messageLock);
    if (messages->find(pos, msg))
        return true;
    return false;
}

void Queue::consume(Consumer::shared_ptr c, bool requestExclusive){
    assertClusterSafe();
    {
        Mutex::ScopedLock locker(messageLock);
        // NOTE: consumerCount is actually a count of all
        // subscriptions, both acquiring and non-acquiring (browsers).
        // Check for exclusivity of acquiring consumers.
        size_t acquiringConsumers = consumerCount - browserCount;
        if (c->preAcquires()) {
=======
bool Queue::find(SequenceNumber pos, Message& msg) const
{
    Mutex::ScopedLock locker(messageLock);
    Message* ptr = messages->find(pos, 0);
    if (ptr) {
        msg = *ptr;
        return true;
    }
    return false;
}

void Queue::markInUse(bool controlling)
{
    Mutex::ScopedLock locker(messageLock);
    if (controlling) users.addLifecycleController();
    else users.addOther();
}

void Queue::releaseFromUse(bool controlling, bool doDelete)
{
    bool trydelete;
    if (controlling) {
        Mutex::ScopedLock locker(messageLock);
        users.removeLifecycleController();
        trydelete = true;
    } else {
        Mutex::ScopedLock locker(messageLock);
        users.removeOther();
        trydelete = isUnused(locker);
    }
    if (trydelete && doDelete) scheduleAutoDelete();
}

void Queue::consume(Consumer::shared_ptr c, bool requestExclusive,
                    const framing::FieldTable& arguments,
                    const std::string& connectionId, const std::string& userId)
{
    boost::intrusive_ptr<qpid::sys::TimerTask> t;
    {
        Mutex::ScopedLock locker(messageLock);
        if (c->preAcquires()) {
            if(settings.isBrowseOnly) {
                throw NotAllowedException(
                    QPID_MSG("Queue " << name << " is browse only.  Refusing acquiring consumer."));
            }

>>>>>>> 3bbfc42... Imported Upstream version 0.32
            if(exclusive) {
                throw ResourceLockedException(
                    QPID_MSG("Queue " << getName()
                             << " has an exclusive consumer. No more consumers allowed."));
            } else if(requestExclusive) {
<<<<<<< HEAD
                if(acquiringConsumers) {
=======
                if(users.hasConsumers()) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                    throw ResourceLockedException(
                        QPID_MSG("Queue " << getName()
                                 << " already has consumers. Exclusive access denied."));
                } else {
                    exclusive = c->getSession();
                }
            }
<<<<<<< HEAD
        }
        else
            browserCount++;
        consumerCount++;
        //reset auto deletion timer if necessary
        if (autoDeleteTimeout && autoDeleteTask) {
            autoDeleteTask->cancel();
        }
        observeConsumerAdd(*c, locker);
    }
    if (mgmtObject != 0)
        mgmtObject->inc_consumerCount ();
}

void Queue::cancel(Consumer::shared_ptr c){
    removeListener(c);
    {
        Mutex::ScopedLock locker(messageLock);
        consumerCount--;
        if (!c->preAcquires()) browserCount--;
        if(exclusive) exclusive = 0;
        observeConsumerRemove(*c, locker);
    }
    if (mgmtObject != 0)
        mgmtObject->dec_consumerCount ();
}

QueuedMessage Queue::get(){
    QueuedMessage msg(this);
    bool ok;
    {
        Mutex::ScopedLock locker(messageLock);
        ok = messages->consume(msg);
        if (ok) observeAcquire(msg, locker);
    }

    if (ok && mgmtObject) {
        mgmtObject->inc_acquires();
        if (brokerMgmtObject)
            brokerMgmtObject->inc_acquires();
    }

    return msg;
}

namespace {
bool collectIf(QueuedMessage& qm, Messages::Predicate predicate,
               std::deque<QueuedMessage>& collection)
{
    if (predicate(qm)) {
        collection.push_back(qm);
        return true;
    } else {
        return false;
    }
}

bool isExpired(const QueuedMessage& qm) { return qm.payload->hasExpired(); }
} // namespace

void Queue::dequeueIf(Messages::Predicate predicate,
                      std::deque<QueuedMessage>& dequeued)
{
    {
        Mutex::ScopedLock locker(messageLock);
        messages->removeIf(boost::bind(&collectIf, _1, predicate, boost::ref(dequeued)));
    }
    if (!dequeued.empty()) {
        if (mgmtObject) {
            mgmtObject->inc_acquires(dequeued.size());
            if (brokerMgmtObject)
                brokerMgmtObject->inc_acquires(dequeued.size());
        }
        for (std::deque<QueuedMessage>::const_iterator i = dequeued.begin();
             i != dequeued.end(); ++i) {
            {
                // KAG: should be safe to retake lock after the removeIf, since
                // no other thread can touch these messages after the removeIf() call
                Mutex::ScopedLock locker(messageLock);
                observeAcquire(*i, locker);
            }
            dequeue( 0, *i );
        }
    }
=======
            users.addConsumer();
        } else if(c->isCounted()) {
            users.addBrowser();
        }
        if(c->isCounted()) {
            //reset auto deletion timer if necessary
            if (settings.autoDeleteDelay && autoDeleteTask) {
                t = autoDeleteTask;
            }

            observeConsumerAdd(*c, locker);
        }
    }
    if (t) t->cancel();
    if (mgmtObject != 0 && c->isCounted()) {
        mgmtObject->inc_consumerCount();
    }
    if (broker) {
        ManagementAgent* agent = broker->getManagementAgent();
        if (agent) {
            agent->raiseEvent(
                _qmf::EventSubscribe(connectionId, userId, name,
                                     c->getTag(), requestExclusive, ManagementAgent::toMap(arguments)));
        }
    }
}

void Queue::cancel(Consumer::shared_ptr c, const std::string& connectionId, const std::string& userId)
{
    removeListener(c);
    if(c->isCounted())

    {
        bool unused;
        {
            Mutex::ScopedLock locker(messageLock);
            if (c->preAcquires()) {
                users.removeConsumer();
                if (exclusive) exclusive = 0;
            } else {
                users.removeBrowser();
            }
            observeConsumerRemove(*c, locker);
            unused = !users.isUsed();
        }
        if (mgmtObject != 0) {
            mgmtObject->dec_consumerCount();
        }
        if (unused && settings.autodelete) scheduleAutoDelete();
    }
    if (broker) {
        ManagementAgent* agent = broker->getManagementAgent();
        if (agent) agent->raiseEvent(_qmf::EventUnsubscribe(connectionId, userId, c->getTag()));
    }
}

namespace{
bool hasExpired(const Message& m, AbsTime now)
{
    return m.getExpiration() < now;
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/**
 *@param lapse: time since the last purgeExpired
 */
void Queue::purgeExpired(sys::Duration lapse) {
    //As expired messages are discarded during dequeue also, only
    //bother explicitly expiring if the rate of dequeues since last
    //attempt is less than one per second.
    int count = dequeueSincePurge.get();
    dequeueSincePurge -= count;
    int seconds = int64_t(lapse)/qpid::sys::TIME_SEC;
    if (seconds == 0 || count / seconds < 1) {
<<<<<<< HEAD
        std::deque<QueuedMessage> dequeued;
        dequeueIf(boost::bind(&isExpired, _1), dequeued);
        if (dequeued.size()) {
            if (mgmtObject) {
                mgmtObject->inc_discardsTtl(dequeued.size());
                if (brokerMgmtObject)
                    brokerMgmtObject->inc_discardsTtl(dequeued.size());
=======
        sys::AbsTime time = sys::AbsTime::now();
        uint32_t count = remove(0, boost::bind(&hasExpired, _1, time), 0, CONSUMER, settings.autodelete);
        QPID_LOG(debug, "Purged " << count << " expired messages from " << getName());
        //
        // Report the count of discarded-by-ttl messages
        //
        if (mgmtObject && count) {
            mgmtObject->inc_discardsTtl(count);
            if (brokerMgmtObject) {
                brokerMgmtObject->inc_discardsTtl(count);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            }
        }
    }
}

namespace {
    // for use with purge/move below - collect messages that match a given filter
    //
    class MessageFilter
    {
    public:
        static const std::string typeKey;
        static const std::string paramsKey;
        static MessageFilter *create( const ::qpid::types::Variant::Map *filter );
<<<<<<< HEAD
        virtual bool match( const QueuedMessage& ) const { return true; }
=======
        virtual bool match( const Message& ) const { return true; }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        virtual ~MessageFilter() {}
    protected:
        MessageFilter() {};
    };
    const std::string MessageFilter::typeKey("filter_type");
    const std::string MessageFilter::paramsKey("filter_params");

    // filter by message header string value exact match
    class HeaderMatchFilter : public MessageFilter
    {
    public:
        /* Config:
           { 'filter_type' : 'header_match_str',
             'filter_params' : { 'header_key' : "<header name>",
                                 'header_value' : "<value to match>"
                               }
           }
        */
        static const std::string typeKey;
        static const std::string headerKey;
        static const std::string valueKey;
        HeaderMatchFilter( const std::string& _header, const std::string& _value )
            : MessageFilter (), header(_header), value(_value) {}
<<<<<<< HEAD
        bool match( const QueuedMessage& msg ) const
        {
            const qpid::framing::FieldTable* headers = msg.payload->getApplicationHeaders();
            if (!headers) return false;
            FieldTable::ValuePtr h = headers->get(header);
            if (!h || !h->convertsTo<std::string>()) return false;
            return h->get<std::string>() == value;
=======
        bool match( const Message& msg ) const
        {
            return msg.getPropertyAsString(header) == value;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        }
    private:
        const std::string header;
        const std::string value;
    };
    const std::string HeaderMatchFilter::typeKey("header_match_str");
    const std::string HeaderMatchFilter::headerKey("header_key");
    const std::string HeaderMatchFilter::valueKey("header_value");

    // factory to create correct filter based on map
    MessageFilter* MessageFilter::create( const ::qpid::types::Variant::Map *filter )
    {
        using namespace qpid::types;
        if (filter && !filter->empty()) {
            Variant::Map::const_iterator i = filter->find(MessageFilter::typeKey);
            if (i != filter->end()) {

                if (i->second.asString() == HeaderMatchFilter::typeKey) {
                    Variant::Map::const_iterator p = filter->find(MessageFilter::paramsKey);
                    if (p != filter->end() && p->second.getType() == VAR_MAP) {
                        Variant::Map::const_iterator k = p->second.asMap().find(HeaderMatchFilter::headerKey);
                        Variant::Map::const_iterator v = p->second.asMap().find(HeaderMatchFilter::valueKey);
                        if (k != p->second.asMap().end() && v != p->second.asMap().end()) {
                            std::string headerKey(k->second.asString());
                            std::string value(v->second.asString());
                            QPID_LOG(debug, "Message filtering by header value configured.  key: " << headerKey << " value: " << value );
                            return new HeaderMatchFilter( headerKey, value );
                        }
                    }
                }
            }
<<<<<<< HEAD
            QPID_LOG(error, "Ignoring unrecognized message filter: '" << *filter << "'");
=======
            QPID_LOG(error, "Unrecognized message filter: '" << *filter << "'");
            throw qpid::Exception(QPID_MSG("Unrecognized message filter: '" << *filter << "'"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        }
        return new MessageFilter();
    }

<<<<<<< HEAD
    // used by removeIf() to collect all messages matching a filter, maximum match count is
    // optional.
    struct Collector {
        const uint32_t maxMatches;
        MessageFilter& filter;
        std::deque<QueuedMessage> matches;
        Collector(MessageFilter& filter, uint32_t max)
            : maxMatches(max), filter(filter) {}
        bool operator() (QueuedMessage& qm)
        {
            if (maxMatches == 0 || matches.size() < maxMatches) {
                if (filter.match( qm )) {
                    matches.push_back(qm);
                    return true;
                }
            }
            return false;
        }
    };

} // end namespace
=======
    void moveTo(boost::shared_ptr<Queue> q, Message& m)
    {
        if (q) {
            q->deliver(m);
        }
    }
} // end namespace

uint32_t Queue::remove(const uint32_t maxCount, MessagePredicate p, MessageFunctor f,
                       SubscriptionType type, bool triggerAutoDelete, uint32_t maxTests)
{
    ScopedAutoDelete autodelete(*this);
    std::deque<Message> removed;
    {
        QueueCursor c(type);
        uint32_t count(0), tests(0);
        Mutex::ScopedLock locker(messageLock);
        Message* m = messages->next(c);
        while (m){
            if (maxTests && tests++ >= maxTests) break;
            if (!p || p(*m)) {
                if (maxCount && count++ >= maxCount) break;
                if (m->getState() == AVAILABLE) {
                    //don't actually acquire, just act as if we did
                    observeAcquire(*m, locker);
                }
                observeDequeue(*m, locker, triggerAutoDelete ? &autodelete : 0);
                removed.push_back(*m);//takes a copy of the message
                if (!messages->deleted(c)) {
                    QPID_LOG(warning, "Failed to correctly remove message from " << name << "; state is not consistent!");
                    assert(false);
                }
            }
            m = messages->next(c);
        }
    }
    for (std::deque<Message>::iterator i = removed.begin(); i != removed.end(); ++i) {
        if (f) f(*i);//ERROR? need to clear old persistent context?
        if (i->isPersistent()) dequeueFromStore(i->getPersistentContext());//do this outside of lock and after any re-routing
    }
    return removed.size();
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32


/**
 * purge - for purging all or some messages on a queue
 *         depending on the purge_request
 *
<<<<<<< HEAD
 * purge_request == 0 then purge all messages
 *               == N then purge N messages from queue
 * Sometimes purge_request == 1 to unblock the top of queue
=======
 * qty == 0 then purge all messages
 *     == N then purge N messages from queue
 * Sometimes qty == 1 to unblock the top of queue
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 *
 * The dest exchange may be supplied to re-route messages through the exchange.
 * It is safe to re-route messages such that they arrive back on the same queue,
 * even if the queue is ordered by priority.
 *
 * An optional filter can be supplied that will be applied against each message.  The
 * message is purged only if the filter matches.  See MessageDistributor for more detail.
 */
<<<<<<< HEAD
uint32_t Queue::purge(const uint32_t purge_request, boost::shared_ptr<Exchange> dest,
                      const qpid::types::Variant::Map *filter)
{
    std::auto_ptr<MessageFilter> mf(MessageFilter::create(filter));
    Collector c(*mf.get(), purge_request);

    {
        Mutex::ScopedLock locker(messageLock);
        messages->removeIf( boost::bind<bool>(boost::ref(c), _1) );
    }

    if (!c.matches.empty()) {
        if (mgmtObject) {
            mgmtObject->inc_acquires(c.matches.size());
            if (dest.get()) {
                mgmtObject->inc_reroutes(c.matches.size());
                if (brokerMgmtObject) {
                    brokerMgmtObject->inc_acquires(c.matches.size());
                    brokerMgmtObject->inc_reroutes(c.matches.size());
                }
            } else {
                mgmtObject->inc_discardsPurge(c.matches.size());
                if (brokerMgmtObject) {
                    brokerMgmtObject->inc_acquires(c.matches.size());
                    brokerMgmtObject->inc_discardsPurge(c.matches.size());
                }
            }
        }

        for (std::deque<QueuedMessage>::iterator qmsg = c.matches.begin();
             qmsg != c.matches.end(); ++qmsg) {

            {
                // KAG: should be safe to retake lock after the removeIf, since
                // no other thread can touch these messages after the removeIf call
                Mutex::ScopedLock locker(messageLock);
                observeAcquire(*qmsg, locker);
            }
            dequeue(0, *qmsg);
            QPID_LOG(debug, "Purged message at " << qmsg->position << " from " << getName());
            // now reroute if necessary
            if (dest.get()) {
                assert(qmsg->payload);
                qmsg->payload->clearTrace();
                DeliverableMessage dmsg(qmsg->payload);
                dest->routeWithAlternate(dmsg);
            }
        }
    }
    return c.matches.size();
=======
uint32_t Queue::purge(const uint32_t qty, boost::shared_ptr<Exchange> dest,
                      const qpid::types::Variant::Map *filter)
{
    std::auto_ptr<MessageFilter> mf(MessageFilter::create(filter));
    uint32_t count = remove(qty, boost::bind(&MessageFilter::match, mf.get(), _1), boost::bind(&reroute, dest, _1), CONSUMER/*?*/, settings.autodelete);

    if (mgmtObject && count) {
        mgmtObject->inc_acquires(count);
        if (dest.get()) {
            mgmtObject->inc_reroutes(count);
            if (brokerMgmtObject) {
                brokerMgmtObject->inc_acquires(count);
                brokerMgmtObject->inc_reroutes(count);
            }
        } else {
            mgmtObject->inc_discardsPurge(count);
            if (brokerMgmtObject) {
                brokerMgmtObject->inc_acquires(count);
                brokerMgmtObject->inc_discardsPurge(count);
            }
        }
    }

    return count;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

uint32_t Queue::move(const Queue::shared_ptr destq, uint32_t qty,
                     const qpid::types::Variant::Map *filter)
{
    std::auto_ptr<MessageFilter> mf(MessageFilter::create(filter));
<<<<<<< HEAD
    Collector c(*mf.get(), qty);

    {
        Mutex::ScopedLock locker(messageLock);
        messages->removeIf( boost::bind<bool>(boost::ref(c), _1) );
    }


    if (!c.matches.empty()) {
        // Update observers and message state:

        if (mgmtObject) {
            mgmtObject->inc_acquires(c.matches.size());
            if (brokerMgmtObject)
                brokerMgmtObject->inc_acquires(c.matches.size());
        }

        for (std::deque<QueuedMessage>::iterator qmsg = c.matches.begin();
             qmsg != c.matches.end(); ++qmsg) {
            {
                Mutex::ScopedLock locker(messageLock);
                observeAcquire(*qmsg, locker);
            }
            dequeue(0, *qmsg);
            // and move to destination Queue.
            assert(qmsg->payload);
            destq->deliver(qmsg->payload);
        }
    }
    return c.matches.size();
}

/** Acquire the message at the given position, return true and msg if acquire succeeds */
bool Queue::acquire(const qpid::framing::SequenceNumber& position, QueuedMessage& msg)
{
    bool ok;
    {
        Mutex::ScopedLock locker(messageLock);
        ok = messages->acquire(position, msg);
        if (ok) observeAcquire(msg, locker);
    }
    if (ok) {
        if (mgmtObject) {
            mgmtObject->inc_acquires();
            if (brokerMgmtObject)
                brokerMgmtObject->inc_acquires();
        }
        ++dequeueSincePurge;
        return true;
    }
    return false;
}

void Queue::push(boost::intrusive_ptr<Message>& msg, bool isRecovery){
    assertClusterSafe();
    QueueListeners::NotificationSet copy;
    QueuedMessage removed, qm(this, msg);
    bool dequeueRequired = false;
    {
        Mutex::ScopedLock locker(messageLock);
        qm.position = ++sequence;
        if (messages->push(qm, removed)) {
            dequeueRequired = true;
            observeAcquire(removed, locker);
        }
        observeEnqueue(qm, locker);
        if (policy.get()) {
            policy->enqueued(qm);
        }
        listeners.populate(copy);
    }
    if (insertSeqNo) msg->insertCustomProperty(seqNoKey, qm.position);

    mgntEnqStats(msg, mgmtObject, brokerMgmtObject);

    if (dequeueRequired) {
        if (mgmtObject) {
            mgmtObject->inc_acquires();
            mgmtObject->inc_discardsLvq();
            if (brokerMgmtObject) {
                brokerMgmtObject->inc_acquires();
                brokerMgmtObject->inc_discardsLvq();
            }
        }
        if (isRecovery) {
            //can't issue new requests for the store until
            //recovery is complete
            Mutex::ScopedLock locker(messageLock);
            pendingDequeues.push_back(removed);
        } else {
            dequeue(0, removed);
        }
=======
    return remove(qty, boost::bind(&MessageFilter::match, mf.get(), _1), boost::bind(&moveTo, destq, _1), CONSUMER/*?*/, settings.autodelete);
}

void Queue::push(Message& message, bool /*isRecovery*/)
{
    QueueListeners::NotificationSet copy;
    {
        Mutex::ScopedLock locker(messageLock);
        message.setSequence(++sequence);
        if (settings.sequencing) message.addAnnotation(settings.sequenceKey, (uint32_t)sequence);
        interceptors.publish(message);
        messages->publish(message);
        listeners.populate(copy);
        observeEnqueue(message, locker);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    copy.notify();
}

<<<<<<< HEAD
void isEnqueueComplete(uint32_t* result, const QueuedMessage& message)
{
    if (message.payload->isIngressComplete()) (*result)++;
}

/** function only provided for unit tests, or code not in critical message path */
uint32_t Queue::getEnqueueCompleteMessageCount() const
{
    uint32_t count = 0;
    Mutex::ScopedLock locker(messageLock);
    messages->foreach(boost::bind(&isEnqueueComplete, &count, _1));
    return count;
}

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
uint32_t Queue::getMessageCount() const
{
    Mutex::ScopedLock locker(messageLock);
    return messages->size();
}

uint32_t Queue::getConsumerCount() const
{
    Mutex::ScopedLock locker(messageLock);
<<<<<<< HEAD
    return consumerCount;
=======
    return users.getSubscriberCount();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

bool Queue::canAutoDelete() const
{
    Mutex::ScopedLock locker(messageLock);
<<<<<<< HEAD
    return autodelete && !consumerCount && !owner;
}

void Queue::clearLastNodeFailure()
{
    inLastNodeFailure = false;
}

void Queue::forcePersistent(QueuedMessage& message)
{
    if(!message.payload->isStoredOnQueue(shared_from_this())) {
        message.payload->forcePersistent();
        if (message.payload->isForcedPersistent() ){
            enqueue(0, message.payload);
        }
    }
}

void Queue::setLastNodeFailure()
{
    if (persistLastNode){
        Mutex::ScopedLock locker(messageLock);
        try {
            messages->foreach(boost::bind(&Queue::forcePersistent, this, _1));
        } catch (const std::exception& e) {
            // Could not go into last node standing (for example journal not large enough)
            QPID_LOG(error, "Unable to fail to last node standing for queue: " << name << " : " << e.what());
        }
        inLastNodeFailure = true;
    }
}


// return true if store exists,
bool Queue::enqueue(TransactionContext* ctxt, boost::intrusive_ptr<Message>& msg, bool suppressPolicyCheck)
=======
    return !deleted && checkAutoDelete(locker);
}

bool Queue::checkAutoDelete(const Mutex::ScopedLock& lock) const
{
    if (settings.autodelete) {
        switch (settings.lifetime) {
          case QueueSettings::DELETE_IF_UNUSED:
            return isUnused(lock);
          case QueueSettings::DELETE_IF_EMPTY:
            return !users.isInUseByController() && isEmpty(lock);
          case QueueSettings::DELETE_IF_UNUSED_AND_EMPTY:
            return isUnused(lock) && isEmpty(lock);
          case QueueSettings::DELETE_ON_CLOSE:
            return !users.isInUseByController();
        }
    }
    return false;
}

bool Queue::isUnused(const Mutex::ScopedLock&) const
{
    return !owner && !users.isUsed();;
}

bool Queue::isEmpty(const Mutex::ScopedLock&) const
{
    return current.getCount() == 0;
}
/*
 * return true if enqueue succeeded and message should be made
 * available; returning false will result in the message being dropped
 */
bool Queue::enqueue(TransactionContext* ctxt, Message& msg)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    ScopedUse u(barrier);
    if (!u.acquired) return false;

<<<<<<< HEAD
    if (policy.get() && !suppressPolicyCheck) {
        std::deque<QueuedMessage> dequeues;
        {
            Mutex::ScopedLock locker(messageLock);
            try {
                policy->tryEnqueue(msg);
            } catch(ResourceLimitExceededException&) {
                if (mgmtObject) {
                    mgmtObject->inc_discardsOverflow();
                    if (brokerMgmtObject)
                        brokerMgmtObject->inc_discardsOverflow();
                }
                throw;
            }
            policy->getPendingDequeues(dequeues);
        }
        //depending on policy, may have some dequeues that need to performed without holding the lock

        //
        // Count the dequeues as ring-discards.  We know that these aren't rejects because
        // policy->tryEnqueue would have thrown an exception.
        //
        if (mgmtObject && !dequeues.empty()) {
            mgmtObject->inc_discardsRing(dequeues.size());
            if (brokerMgmtObject)
                brokerMgmtObject->inc_discardsRing(dequeues.size());
        }

        for_each(dequeues.begin(), dequeues.end(), boost::bind(&Queue::dequeue, this, (TransactionContext*) 0, _1));
    }

    if (inLastNodeFailure && persistLastNode){
        msg->forcePersistent();
    }

    if (traceId.size()) {
        msg->addTraceId(traceId);
    }

    if ((msg->isPersistent() || msg->checkContentReleasable()) && store) {
        // mark the message as being enqueued - the store MUST CALL msg->enqueueComplete()
        // when it considers the message stored.
        msg->enqueueAsync(shared_from_this(), store);
        boost::intrusive_ptr<PersistableMessage> pmsg = boost::static_pointer_cast<PersistableMessage>(msg);
        store->enqueue(ctxt, pmsg, *this);
        return true;
    }
    if (!store) {
        //Messages enqueued on a transient queue should be prevented
        //from having their content released as it may not be
        //recoverable by these queue for delivery
        msg->blockContentRelease();
    }
    return false;
}

void Queue::enqueueAborted(boost::intrusive_ptr<Message> msg)
{
    Mutex::ScopedLock locker(messageLock);
    if (policy.get()) policy->enqueueAborted(msg);
}

// return true if store exists,
bool Queue::dequeue(TransactionContext* ctxt, const QueuedMessage& msg)
{
    ScopedUse u(barrier);
    if (!u.acquired) return false;
    {
        Mutex::ScopedLock locker(messageLock);
        if (!isEnqueued(msg)) return false;
        if (!ctxt) {
            if (policy.get()) policy->dequeued(msg);
            messages->deleted(msg);
            observeDequeue(msg, locker);
        }
    }

    if (!ctxt) {
        mgntDeqStats(msg.payload, mgmtObject, brokerMgmtObject);
    }

    // This check prevents messages which have been forced persistent on one queue from dequeuing
    // from another on which no forcing has taken place and thus causing a store error.
    bool fp = msg.payload->isForcedPersistent();
    if (!fp || (fp && msg.payload->isStoredOnQueue(shared_from_this()))) {
        if ((msg.payload->isPersistent() || msg.payload->checkContentReleasable()) && store) {
            msg.payload->dequeueAsync(shared_from_this(), store); //increment to async counter -- for message sent to more than one queue
            boost::intrusive_ptr<PersistableMessage> pmsg = boost::static_pointer_cast<PersistableMessage>(msg.payload);
            store->dequeue(ctxt, pmsg, *this);
            return true;
        }
    }
    return false;
}

void Queue::dequeueCommitted(const QueuedMessage& msg)
{
    {
        Mutex::ScopedLock locker(messageLock);
        if (policy.get()) policy->dequeued(msg);
        messages->deleted(msg);
        observeDequeue(msg, locker);
    }
    mgntDeqStats(msg.payload, mgmtObject, brokerMgmtObject);
    if (mgmtObject != 0) {
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        const uint64_t contentSize = msg.payload->contentSize();
        qStats->msgTxnDequeues  += 1;
        qStats->byteTxnDequeues += contentSize;
        mgmtObject->statisticsUpdated();
=======
    {
        Mutex::ScopedLock locker(messageLock);
        if (!checkDepth(QueueDepth(1, msg.getMessageSize()), msg)) {
            return false;
        }
    }

    if (settings.traceId.size()) {
        msg.addTraceId(settings.traceId);
    }

    if (msg.isPersistent() && store) {
        // mark the message as being enqueued - the store MUST CALL msg->enqueueComplete()
        // when it considers the message stored.
        boost::intrusive_ptr<PersistableMessage> pmsg = msg.getPersistentContext();
        assert(pmsg);
        pmsg->enqueueAsync(shared_from_this());
        try {
            store->enqueue(ctxt, pmsg, *this);
        } catch (...) {
            enqueueAborted(msg);
            throw;
        }
    }
    return true;
}

void Queue::enqueueAborted(const Message& msg)
{
    //Called when any transactional enqueue is aborted (including but
    //not limited to a recovered dtx transaction)
    Mutex::ScopedLock locker(messageLock);
    current -= QueueDepth(1, msg.getMessageSize());
}

void Queue::enqueueCommited(Message& msg)
{
    //called when a recovered dtx enqueue operation is committed; the
    //message is already on disk and space has been reserved in policy
    //but it should now be made available
    process(msg);
}
void Queue::dequeueAborted(Message& msg)
{
    //called when a recovered dtx dequeue operation is aborted; the
    //message should be added back to the queue
    push(msg);
}
void Queue::dequeueCommited(const Message& msg)
{
    //called when a recovered dtx dequeue operation is committed; the
    //message will at this point have already been removed from the
    //store and will not be available for delivery. The only action
    //required is to ensure the observers are notified and the
    //management stats are correctly decremented
    ScopedAutoDelete autodelete(*this);
    Mutex::ScopedLock locker(messageLock);
    observeDequeue(msg, locker, settings.autodelete ? &autodelete : 0);
    if (mgmtObject != 0) {
        mgmtObject->inc_msgTxnDequeues();
        mgmtObject->inc_byteTxnDequeues(msg.getMessageSize());
    }
}


void Queue::dequeueFromStore(boost::intrusive_ptr<PersistableMessage> msg)
{
    ScopedUse u(barrier);
    if (u.acquired && msg && store) {
        store->dequeue(0, msg, *this);
    }
}

void Queue::dequeue(const QueueCursor& cursor, TxBuffer* txn)
{
    if (txn) {
        TxOp::shared_ptr op;
        {
            Mutex::ScopedLock locker(messageLock);
            Message* msg = messages->find(cursor);
            if (msg) {
                op = TxOp::shared_ptr(new TxDequeue(cursor, shared_from_this(), msg->getSequence(), msg->getReplicationId()));
            }
        }
        if (op) txn->enlist(op);
    } else {
        dequeue(0, cursor);
    }
}


void Queue::dequeue(TransactionContext* ctxt, const QueueCursor& cursor)
{
    ScopedUse u(barrier);
    if (!u.acquired) return;
    ScopedAutoDelete autodelete(*this);
    boost::intrusive_ptr<PersistableMessage> pmsg;
    {
        Mutex::ScopedLock locker(messageLock);
        Message* msg = messages->find(cursor);
        if (msg) {
            if (msg->isPersistent()) pmsg = msg->getPersistentContext();
            if (!ctxt) {
                observeDequeue(*msg, locker, settings.autodelete ? &autodelete : 0);
                messages->deleted(cursor);//message pointer not valid after this
            }
        } else {
            return;
        }
    }
    if (store && pmsg) {
        store->dequeue(ctxt, pmsg, *this);
    }
}

void Queue::dequeueCommitted(const QueueCursor& cursor)
{
    ScopedAutoDelete autodelete(*this);
    Mutex::ScopedLock locker(messageLock);
    Message* msg = messages->find(cursor);
    if (msg) {
        const uint64_t contentSize = msg->getMessageSize();
        observeDequeue(*msg, locker, settings.autodelete ? &autodelete : 0);
        if (mgmtObject != 0) {
            mgmtObject->inc_msgTxnDequeues();
            mgmtObject->inc_byteTxnDequeues(contentSize);
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (brokerMgmtObject) {
            _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();
            bStats->msgTxnDequeues += 1;
            bStats->byteTxnDequeues += contentSize;
            brokerMgmtObject->statisticsUpdated();
        }
<<<<<<< HEAD
    }
}

/**
 * Removes the first (oldest) message from the in-memory delivery queue as well dequeing
 * it from the logical (and persistent if applicable) queue
 */
bool Queue::popAndDequeue(QueuedMessage& msg)
{
    bool popped;
    {
        Mutex::ScopedLock locker(messageLock);
        popped = messages->consume(msg);
        if (popped) observeAcquire(msg, locker);
    }
    if (popped) {
        if (mgmtObject) {
            mgmtObject->inc_acquires();
            if (brokerMgmtObject)
                brokerMgmtObject->inc_acquires();
        }
        dequeue(0, msg);
        return true;
    } else {
        return false;
=======
        messages->deleted(cursor);
    } else {
        QPID_LOG(error, "Could not find dequeued message on commit");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

/**
 * Updates policy and management when a message has been dequeued,
 * Requires messageLock be held by caller.
 */
<<<<<<< HEAD
void Queue::observeDequeue(const QueuedMessage& msg, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::const_iterator i = observers.begin(); i != observers.end(); ++i) {
        try{
            (*i)->dequeued(msg);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of dequeue for queue " << getName() << ": " << e.what());
        }
    }
=======
void Queue::observeDequeue(const Message& msg, const Mutex::ScopedLock& lock, ScopedAutoDelete* autodelete)
{
    current -= QueueDepth(1, msg.getMessageSize());
    mgntDeqStats(msg, mgmtObject, brokerMgmtObject);
    observers.dequeued(msg, lock);
    if (autodelete && isEmpty(lock)) autodelete->check(lock);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/** updates queue observers when a message has become unavailable for transfer.
 * Requires messageLock be held by caller.
 */
<<<<<<< HEAD
void Queue::observeAcquire(const QueuedMessage& msg, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::const_iterator i = observers.begin(); i != observers.end(); ++i) {
        try{
            (*i)->acquired(msg);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of message removal for queue " << getName() << ": " << e.what());
        }
    }
=======
void Queue::observeAcquire(const Message& msg, const Mutex::ScopedLock& l)
{
    observers.acquired(msg, l);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/** updates queue observers when a message has become re-available for transfer
 *  Requires messageLock be held by caller.
 */
<<<<<<< HEAD
void Queue::observeRequeue(const QueuedMessage& msg, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::const_iterator i = observers.begin(); i != observers.end(); ++i) {
        try{
            (*i)->requeued(msg);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of message requeue for queue " << getName() << ": " << e.what());
        }
    }
=======
void Queue::observeRequeue(const Message& msg, const Mutex::ScopedLock& l)
{
    observers.requeued(msg, l);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/** updates queue observers when a new consumer has subscribed to this queue.
 */
<<<<<<< HEAD
void Queue::observeConsumerAdd( const Consumer& c, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::const_iterator i = observers.begin(); i != observers.end(); ++i) {
        try{
            (*i)->consumerAdded(c);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of new consumer for queue " << getName() << ": " << e.what());
        }
    }
=======
void Queue::observeConsumerAdd( const Consumer& c, const qpid::sys::Mutex::ScopedLock& l)
{
    observers.consumerAdded(c, l);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/** updates queue observers when a consumer has unsubscribed from this queue.
 */
<<<<<<< HEAD
void Queue::observeConsumerRemove( const Consumer& c, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::const_iterator i = observers.begin(); i != observers.end(); ++i) {
        try{
            (*i)->consumerRemoved(c);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of removed consumer for queue " << getName() << ": " << e.what());
        }
    }
}


void Queue::create(const FieldTable& _settings)
{
    settings = _settings;
    if (store) {
        store->create(*this, _settings);
    }
    configureImpl(_settings);
=======
void Queue::observeConsumerRemove( const Consumer& c, const qpid::sys::Mutex::ScopedLock& l)
{
    observers.consumerRemoved(c, l);
}


void Queue::create()
{
    if (store) {
        store->create(*this, settings.storeSettings);
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


int getIntegerSetting(const qpid::framing::FieldTable& settings, const std::string& key)
{
    qpid::framing::FieldTable::ValuePtr v = settings.get(key);
    if (!v) {
        return 0;
    } else if (v->convertsTo<int>()) {
        return v->get<int>();
    } else if (v->convertsTo<std::string>()){
        std::string s = v->get<std::string>();
        try {
            return boost::lexical_cast<int>(s);
        } catch(const boost::bad_lexical_cast&) {
            QPID_LOG(warning, "Ignoring invalid integer value for " << key << ": " << s);
            return 0;
        }
    } else {
        QPID_LOG(warning, "Ignoring invalid integer value for " << key << ": " << *v);
        return 0;
    }
}

bool getBoolSetting(const qpid::framing::FieldTable& settings, const std::string& key)
{
    qpid::framing::FieldTable::ValuePtr v = settings.get(key);
    if (!v) {
        return false;
    } else if (v->convertsTo<int>()) {
        return v->get<int>() != 0;
    } else if (v->convertsTo<std::string>()){
        std::string s = v->get<std::string>();
        if (s == "True")  return true;
        if (s == "true")  return true;
        if (s == "False") return false;
        if (s == "false") return false;
        try {
            return boost::lexical_cast<bool>(s);
        } catch(const boost::bad_lexical_cast&) {
            QPID_LOG(warning, "Ignoring invalid boolean value for " << key << ": " << s);
            return false;
        }
    } else {
        QPID_LOG(warning, "Ignoring invalid boolean value for " << key << ": " << *v);
        return false;
    }
}

<<<<<<< HEAD
void Queue::configure(const FieldTable& _settings)
{
    settings = _settings;
    configureImpl(settings);
}

void Queue::configureImpl(const FieldTable& _settings)
{
    eventMode = _settings.getAsInt(qpidQueueEventGeneration);
    if (eventMode && broker) {
        broker->getQueueEvents().observe(*this, eventMode == ENQUEUE_ONLY);
    }

    if (QueuePolicy::getType(_settings) == QueuePolicy::FLOW_TO_DISK &&
        (!store || NullMessageStore::isNullStore(store) || (broker && !(broker->getQueueEvents().isSync())) )) {
        if ( NullMessageStore::isNullStore(store)) {
            QPID_LOG(warning, "Flow to disk not valid for non-persisted queue:" << getName());
        } else if (broker && !(broker->getQueueEvents().isSync()) ) {
            QPID_LOG(warning, "Flow to disk not valid with async Queue Events:" << getName());
        }
        FieldTable copy(_settings);
        copy.erase(QueuePolicy::typeKey);
        setPolicy(QueuePolicy::createQueuePolicy(getName(), copy));
    } else {
        setPolicy(QueuePolicy::createQueuePolicy(getName(), _settings));
    }
    if (broker && broker->getManagementAgent()) {
        ThresholdAlerts::observe(*this, *(broker->getManagementAgent()), _settings, broker->getOptions().queueThresholdEventRatio);
    }

    //set this regardless of owner to allow use of no-local with exclusive consumers also
    noLocal = getBoolSetting(_settings, qpidNoLocal);
    QPID_LOG(debug, "Configured queue " << getName() << " with no-local=" << noLocal);

    std::string lvqKey = _settings.getAsString(qpidLastValueQueueKey);
    if (lvqKey.size()) {
        QPID_LOG(debug, "Configured queue " <<  getName() << " as Last Value Queue with key " << lvqKey);
        messages = std::auto_ptr<Messages>(new MessageMap(lvqKey));
        allocator = boost::shared_ptr<MessageDistributor>(new FifoDistributor( *messages ));
    } else if (getBoolSetting(_settings, qpidLastValueQueueNoBrowse)) {
        QPID_LOG(debug, "Configured queue " <<  getName() << " as Legacy Last Value Queue with 'no-browse' on");
        messages = LegacyLVQ::updateOrReplace(messages, qpidVQMatchProperty, true, broker);
        allocator = boost::shared_ptr<MessageDistributor>(new FifoDistributor( *messages ));
    } else if (getBoolSetting(_settings, qpidLastValueQueue)) {
        QPID_LOG(debug, "Configured queue " <<  getName() << " as Legacy Last Value Queue");
        messages = LegacyLVQ::updateOrReplace(messages, qpidVQMatchProperty, false, broker);
        allocator = boost::shared_ptr<MessageDistributor>(new FifoDistributor( *messages ));
    } else {
        std::auto_ptr<Messages> m = Fairshare::create(_settings);
        if (m.get()) {
            messages = m;
            allocator = boost::shared_ptr<MessageDistributor>(new FifoDistributor( *messages ));
            QPID_LOG(debug, "Configured queue " <<  getName() << " as priority queue.");
        } else { // default (FIFO) queue type
            // override default message allocator if message groups configured.
            boost::shared_ptr<MessageGroupManager> mgm(MessageGroupManager::create( getName(), *messages, _settings));
            if (mgm) {
                allocator = mgm;
                addObserver(mgm);
            }
        }
    }

    persistLastNode = getBoolSetting(_settings, qpidPersistLastNode);
    if (persistLastNode) QPID_LOG(debug, "Configured queue to Persist data if cluster fails to one node for: " << getName());

    traceId = _settings.getAsString(qpidTraceIdentity);
    std::string excludeList = _settings.getAsString(qpidTraceExclude);
    if (excludeList.size()) {
        split(traceExclude, excludeList, ", ");
    }
    QPID_LOG(debug, "Configured queue " << getName() << " with qpid.trace.id='" << traceId
             << "' and qpid.trace.exclude='"<< excludeList << "' i.e. " << traceExclude.size() << " elements");

    FieldTable::ValuePtr p =_settings.get(qpidInsertSequenceNumbers);
    if (p && p->convertsTo<std::string>()) insertSequenceNumbers(p->get<std::string>());

    autoDeleteTimeout = getIntegerSetting(_settings, qpidAutoDeleteTimeout);
    if (autoDeleteTimeout)
        QPID_LOG(debug, "Configured queue " << getName() << " with qpid.auto_delete_timeout=" << autoDeleteTimeout);

    if (mgmtObject != 0) {
        mgmtObject->set_arguments(ManagementAgent::toMap(_settings));
    }

    QueueFlowLimit::observe(*this, _settings);
=======
void Queue::abandoned(const Message& message)
{
    if (reroute(alternateExchange, message) && brokerMgmtObject)
        brokerMgmtObject->inc_abandonedViaAlt();
    else if (brokerMgmtObject)
        brokerMgmtObject->inc_abandoned();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Queue::destroyed()
{
    unbind(broker->getExchanges());
<<<<<<< HEAD

    QueuedMessage m;
    while(popAndDequeue(m)) {
        DeliverableMessage msg(m.payload);
        if (alternateExchange.get()) {
            if (brokerMgmtObject)
                brokerMgmtObject->inc_abandonedViaAlt();
            alternateExchange->routeWithAlternate(msg);
        } else {
            if (brokerMgmtObject)
                brokerMgmtObject->inc_abandoned();
        }
    }
    if (alternateExchange.get())
        alternateExchange->decAlternateUsers();
=======
    remove(0, 0, boost::bind(&Queue::abandoned, this, _1), REPLICATOR/*even acquired message are treated as abandoned*/, false);
    if (alternateExchange.get()) {
        alternateExchange->decAlternateUsers();
        alternateExchange.reset();
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    if (store) {
        barrier.destroy();
        store->flush(*this);
        store->destroy(*this);
        store = 0;//ensure we make no more calls to the store for this queue
    }
<<<<<<< HEAD
    if (autoDeleteTask) autoDeleteTask = boost::intrusive_ptr<TimerTask>();
    notifyDeleted();
    {
        Mutex::ScopedLock lock(messageLock);
        observers.clear();
=======
    notifyDeleted();
    {
        Mutex::ScopedLock l(messageLock);
        if (autoDeleteTask) autoDeleteTask = boost::intrusive_ptr<TimerTask>();
        observers.destroy(l);
    }
    if (mgmtObject != 0) {
        mgmtObject->resourceDestroy();
        if (brokerMgmtObject)
            brokerMgmtObject->dec_queueCount();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

void Queue::notifyDeleted()
{
    QueueListeners::ListenerSet set;
    {
        Mutex::ScopedLock locker(messageLock);
        deleted = true;
        listeners.snapshot(set);
    }
    set.notifyAll();
}

void Queue::bound(const string& exchange, const string& key,
                  const FieldTable& args)
{
    bindings.add(exchange, key, args);
}

void Queue::unbind(ExchangeRegistry& exchanges)
{
    bindings.unbind(exchanges, shared_from_this());
}

<<<<<<< HEAD
void Queue::setPolicy(std::auto_ptr<QueuePolicy> _policy)
{
    Mutex::ScopedLock locker(messageLock);
    policy = _policy;
    if (policy.get())
        policy->setQueue(this);
}

const QueuePolicy* Queue::getPolicy()
{
    Mutex::ScopedLock locker(messageLock);
    return policy.get();
}
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32

uint64_t Queue::getPersistenceId() const
{
    return persistenceId;
}

void Queue::setPersistenceId(uint64_t _persistenceId) const
{
    if (mgmtObject != 0 && persistenceId == 0 && externalQueueStore)
    {
<<<<<<< HEAD
        ManagementObject* childObj = externalQueueStore->GetManagementObject();
=======
        ManagementObject::shared_ptr childObj = externalQueueStore->GetManagementObject();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (childObj != 0)
            childObj->setReference(mgmtObject->getObjectId());
    }
    persistenceId = _persistenceId;
}

void Queue::encode(Buffer& buffer) const
{
    buffer.putShortString(name);
<<<<<<< HEAD
    buffer.put(settings);
    if (policy.get()) {
        buffer.put(*policy);
    }
    buffer.putShortString(alternateExchange.get() ? alternateExchange->getName() : std::string(""));
=======
    buffer.put(encodableSettings);
    buffer.putShortString(alternateExchange.get() ? alternateExchange->getName() : std::string(""));
    buffer.putShortString(userId);
    buffer.putInt8(isAutoDelete());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

uint32_t Queue::encodedSize() const
{
    return name.size() + 1/*short string size octet*/
        + (alternateExchange.get() ? alternateExchange->getName().size() : 0) + 1 /* short string */
<<<<<<< HEAD
        + settings.encodedSize()
        + (policy.get() ? (*policy).encodedSize() : 0);
=======
        + userId.size() + 1 /* short string */
        + 1 /* autodelete flag */
        + encodableSettings.encodedSize();
}

void Queue::updateAclUserQueueCount()
{
  if (broker->getAcl())
    broker->getAcl()->approveCreateQueue(userId, name);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Queue::shared_ptr Queue::restore( QueueRegistry& queues, Buffer& buffer )
{
    string name;
<<<<<<< HEAD
    buffer.getShortString(name);
    FieldTable settings;
    buffer.get(settings);
    boost::shared_ptr<Exchange> alternate;
    std::pair<Queue::shared_ptr, bool> result = queues.declare(name, true, false, 0, alternate, settings, true);
    if (result.first->policy.get() && buffer.available() >= result.first->policy->encodedSize()) {
        buffer.get ( *(result.first->policy) );
    }
    if (buffer.available()) {
        string altExch;
        buffer.getShortString(altExch);
        result.first->alternateExchangeName.assign(altExch);
=======
    string _userId;
    FieldTable ft;
    boost::shared_ptr<Exchange> alternate;
    QueueSettings settings(true, false); // settings.autodelete might be overwritten
    string altExch;
    bool has_userId = false;
    bool has_altExch = false;

    buffer.getShortString(name);
    buffer.get(ft);
    settings.populate(ft, settings.storeSettings);
    //get alternate exchange
    if (buffer.available()) {
        buffer.getShortString(altExch);
        has_altExch = true;
    }
    //get userId of queue's creator; ACL counters for userId are done after ACL plugin is initialized
    if (buffer.available()) {
        buffer.getShortString(_userId);
        has_userId = true;
    }
    //get autodelete flag
    if (buffer.available()) {
        settings.autodelete = buffer.getInt8();
    }

    std::pair<Queue::shared_ptr, bool> result = queues.declare(name, settings, alternate, true);
    if (has_altExch)
        result.first->alternateExchangeName.assign(altExch);
    if (has_userId)
        result.first->setOwningUser(_userId);

    if (result.first->getSettings().autoDeleteDelay) {
        result.first->scheduleAutoDelete();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }

    return result.first;
}


void Queue::setAlternateExchange(boost::shared_ptr<Exchange> exchange)
{
    alternateExchange = exchange;
<<<<<<< HEAD
=======
    alternateExchange->incAlternateUsers();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (mgmtObject) {
        if (exchange.get() != 0)
            mgmtObject->set_altExchange(exchange->GetManagementObject()->getObjectId());
        else
            mgmtObject->clr_altExchange();
    }
}

boost::shared_ptr<Exchange> Queue::getAlternateExchange()
{
    return alternateExchange;
}

<<<<<<< HEAD
void tryAutoDeleteImpl(Broker& broker, Queue::shared_ptr queue, const std::string& connectionId, const std::string& userId)
{
    if (broker.getQueues().destroyIf(queue->getName(),
                                     boost::bind(boost::mem_fn(&Queue::canAutoDelete), queue))) {
        QPID_LOG(debug, "Auto-deleting " << queue->getName());
        queue->destroyed();

        if (broker.getManagementAgent())
            broker.getManagementAgent()->raiseEvent(_qmf::EventQueueDelete(connectionId, userId, queue->getName()));
    }
}

struct AutoDeleteTask : qpid::sys::TimerTask
{
    Broker& broker;
    Queue::shared_ptr queue;
    std::string connectionId;
    std::string userId;

    AutoDeleteTask(Broker& b, Queue::shared_ptr q, const std::string& cId, const std::string& uId, AbsTime fireTime)
        : qpid::sys::TimerTask(fireTime, "DelayedAutoDeletion:"+q->getName()), broker(b), queue(q), connectionId(cId), userId(uId) {}
=======
struct AutoDeleteTask : qpid::sys::TimerTask
{
    Queue::shared_ptr queue;

    AutoDeleteTask(Queue::shared_ptr q, AbsTime fireTime)
        : qpid::sys::TimerTask(fireTime, "DelayedAutoDeletion:"+q->getName()), queue(q) {}
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    void fire()
    {
        //need to detect case where queue was used after the task was
        //created, but then became unused again before the task fired;
        //in this case ignore this request as there will have already
        //been a later task added
<<<<<<< HEAD
        tryAutoDeleteImpl(broker, queue, connectionId, userId);
    }
};

void Queue::tryAutoDelete(Broker& broker, Queue::shared_ptr queue, const std::string& connectionId, const std::string& userId)
{
    if (queue->autoDeleteTimeout && queue->canAutoDelete()) {
        AbsTime time(now(), Duration(queue->autoDeleteTimeout * TIME_SEC));
        queue->autoDeleteTask = boost::intrusive_ptr<qpid::sys::TimerTask>(new AutoDeleteTask(broker, queue, connectionId, userId, time));
        broker.getClusterTimer().add(queue->autoDeleteTask);
        QPID_LOG(debug, "Timed auto-delete for " << queue->getName() << " initiated");
    } else {
        tryAutoDeleteImpl(broker, queue, connectionId, userId);
=======
        queue->tryAutoDelete();
    }
};

void Queue::scheduleAutoDelete(bool immediate)
{
    if (canAutoDelete()) {
        if (!immediate && settings.autoDeleteDelay) {
            AbsTime time(now(), Duration(settings.autoDeleteDelay * TIME_SEC));
            autoDeleteTask = boost::intrusive_ptr<qpid::sys::TimerTask>(new AutoDeleteTask(shared_from_this(), time));
            broker->getTimer().add(autoDeleteTask);
            QPID_LOG(debug, "Timed auto-delete for " << getName() << " initiated");
        } else {
            tryAutoDelete();
        }
    }
}

void Queue::tryAutoDelete()
{
    bool proceed(false);
    {
        Mutex::ScopedLock locker(messageLock);
        if (!deleted && checkAutoDelete(locker)) {
            proceed = true;
            deleted = true;
        }
    }

    if (proceed) {
        broker->getQueues().destroy(name);
        if (broker->getAcl())
            broker->getAcl()->recordDestroyQueue(name);

        QPID_LOG_CAT(debug, model, "Auto-delete queue deleted: " << name << " (" << deleted << ")");
        destroyed();
    } else {
        QPID_LOG_CAT(debug, model, "Auto-delete queue could not be deleted: " << name);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

bool Queue::isExclusiveOwner(const OwnershipToken* const o) const
{
<<<<<<< HEAD
    Mutex::ScopedLock locker(ownershipLock);
    return o == owner;
}

void Queue::releaseExclusiveOwnership()
{
    Mutex::ScopedLock locker(ownershipLock);
    owner = 0;
=======
    Mutex::ScopedLock locker(messageLock);
    return o == owner;
}

void Queue::releaseExclusiveOwnership(bool immediateExpiry)
{
    bool unused;
    {
        Mutex::ScopedLock locker(messageLock);
        owner = 0;
        if (mgmtObject) {
            mgmtObject->set_exclusive(false);
        }
        unused = !users.isUsed();
    }
    if (unused && settings.autodelete) {
        scheduleAutoDelete(immediateExpiry);
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

bool Queue::setExclusiveOwner(const OwnershipToken* const o)
{
    //reset auto deletion timer if necessary
<<<<<<< HEAD
    if (autoDeleteTimeout && autoDeleteTask) {
        autoDeleteTask->cancel();
    }
    Mutex::ScopedLock locker(ownershipLock);
    if (owner) {
        return false;
    } else {
        owner = o;
=======
    if (settings.autoDeleteDelay && autoDeleteTask) {
        autoDeleteTask->cancel();
    }
    Mutex::ScopedLock locker(messageLock);
    if (owner  || users.hasConsumers()) {
        return false;
    } else {
        owner = o;
        if (mgmtObject) {
            mgmtObject->set_exclusive(true);
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return true;
    }
}

bool Queue::hasExclusiveOwner() const
{
<<<<<<< HEAD
    Mutex::ScopedLock locker(ownershipLock);
=======
    Mutex::ScopedLock locker(messageLock);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    return owner != 0;
}

bool Queue::hasExclusiveConsumer() const
{
    return exclusive;
}

void Queue::setExternalQueueStore(ExternalQueueStore* inst) {
    if (externalQueueStore!=inst && externalQueueStore)
        delete externalQueueStore;
    externalQueueStore = inst;

    if (inst) {
<<<<<<< HEAD
        ManagementObject* childObj = inst->GetManagementObject();
=======
        ManagementObject::shared_ptr childObj = inst->GetManagementObject();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (childObj != 0 && mgmtObject != 0)
            childObj->setReference(mgmtObject->getObjectId());
    }
}

void Queue::countRejected() const
{
    if (mgmtObject) {
        mgmtObject->inc_discardsSubscriber();
        if (brokerMgmtObject)
            brokerMgmtObject->inc_discardsSubscriber();
    }
}

<<<<<<< HEAD
void Queue::countFlowedToDisk(uint64_t size) const
{
    if (mgmtObject) {
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        qStats->msgFtdEnqueues += 1;
        qStats->byteFtdEnqueues += size;
        mgmtObject->statisticsUpdated();
        if (brokerMgmtObject) {
            _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();
            bStats->msgFtdEnqueues += 1;
            bStats->byteFtdEnqueues += size;
            brokerMgmtObject->statisticsUpdated();
        }
    }
}

void Queue::countLoadedFromDisk(uint64_t size) const
{
    if (mgmtObject) {
        _qmf::Queue::PerThreadStats *qStats = mgmtObject->getStatistics();
        qStats->msgFtdDequeues += 1;
        qStats->byteFtdDequeues += size;
        mgmtObject->statisticsUpdated();
        if (brokerMgmtObject) {
            _qmf::Broker::PerThreadStats *bStats = brokerMgmtObject->getStatistics();
            bStats->msgFtdDequeues += 1;
            bStats->byteFtdDequeues += size;
            brokerMgmtObject->statisticsUpdated();
        }
    }
}


ManagementObject* Queue::GetManagementObject (void) const
{
    return (ManagementObject*) mgmtObject;
=======
ManagementObject::shared_ptr Queue::GetManagementObject(void) const
{
    return mgmtObject;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Manageable::status_t Queue::ManagementMethod (uint32_t methodId, Args& args, string& etext)
{
    Manageable::status_t status = Manageable::STATUS_UNKNOWN_METHOD;
<<<<<<< HEAD
=======
    AclModule* acl = broker->getAcl();
    std::string _userId = (getCurrentPublisher()?getCurrentPublisher()->getUserId():"");
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    QPID_LOG (debug, "Queue::ManagementMethod [id=" << methodId << "]");

    switch (methodId) {
    case _qmf::Queue::METHOD_PURGE :
        {
<<<<<<< HEAD
=======
            if ((acl)&&(!(acl->authorise(_userId, acl::ACT_PURGE, acl::OBJ_QUEUE, name, NULL)))) {
                throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied purge request from " << _userId));
            }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            _qmf::ArgsQueuePurge& purgeArgs = (_qmf::ArgsQueuePurge&) args;
            purge(purgeArgs.i_request, boost::shared_ptr<Exchange>(), &purgeArgs.i_filter);
            status = Manageable::STATUS_OK;
        }
        break;

    case _qmf::Queue::METHOD_REROUTE :
        {
            _qmf::ArgsQueueReroute& rerouteArgs = (_qmf::ArgsQueueReroute&) args;
            boost::shared_ptr<Exchange> dest;
            if (rerouteArgs.i_useAltExchange) {
                if (!alternateExchange) {
                    status = Manageable::STATUS_PARAMETER_INVALID;
                    etext = "No alternate-exchange defined";
                    break;
                }
                dest = alternateExchange;
            } else {
                try {
                    dest = broker->getExchanges().get(rerouteArgs.i_exchange);
                } catch(const std::exception&) {
                    status = Manageable::STATUS_PARAMETER_INVALID;
                    etext = "Exchange not found";
                    break;
                }
            }

<<<<<<< HEAD
=======
            if (acl) {
                std::map<acl::Property, std::string> params;
                params.insert(make_pair(acl::PROP_EXCHANGENAME, dest->getName()));
                if (!acl->authorise(_userId, acl::ACT_REROUTE, acl::OBJ_QUEUE, name, &params)) {
                    throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied reroute request from " << _userId));
                }
            }

>>>>>>> 3bbfc42... Imported Upstream version 0.32
            purge(rerouteArgs.i_request, dest, &rerouteArgs.i_filter);
            status = Manageable::STATUS_OK;
        }
        break;
    }

    return status;
}


void Queue::query(qpid::types::Variant::Map& results) const
{
    Mutex::ScopedLock locker(messageLock);
    /** @todo add any interesting queue state into results */
    if (allocator) allocator->query(results);
}

namespace {
struct After {
    framing::SequenceNumber seq;
    After(framing::SequenceNumber s) : seq(s) {}
<<<<<<< HEAD
    bool operator()(const QueuedMessage& qm) { return qm.position > seq; }
=======
    bool operator()(const Message& m) { return m.getSequence() > seq; }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};
} // namespace


void Queue::setPosition(SequenceNumber n) {
    Mutex::ScopedLock locker(messageLock);
    if (n < sequence) {
<<<<<<< HEAD
        std::deque<QueuedMessage> dequeued;
        dequeueIf(After(n), dequeued);
        messages->setPosition(n);
    }
    sequence = n;
    QPID_LOG(trace, "Set position to " << sequence << " on " << getName());
=======
        remove(0, After(n), MessagePredicate(), BROWSER, false);
    }
    sequence = n;
    QPID_LOG(debug, "Set position to " << sequence << " on " << getName());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

SequenceNumber Queue::getPosition() {
    Mutex::ScopedLock locker(messageLock);
    return sequence;
}

<<<<<<< HEAD
=======
void Queue::getRange(framing::SequenceNumber& front, framing::SequenceNumber& back,
                     SubscriptionType type)
{
    Mutex::ScopedLock locker(messageLock);
    QueueCursor cursor(type);
    back = sequence;
    Message* message = messages->next(cursor);
    front = message ? message->getSequence() : back+1;
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
int Queue::getEventMode() { return eventMode; }

void Queue::recoveryComplete(ExchangeRegistry& exchanges)
{
    // set the alternate exchange
    if (!alternateExchangeName.empty()) {
        Exchange::shared_ptr ae = exchanges.find(alternateExchangeName);
        if (ae) setAlternateExchange(ae);
        else QPID_LOG(warning, "Could not set alternate exchange \""
                      << alternateExchangeName << "\" on queue \"" << name
                      << "\": exchange does not exist.");
    }
    //process any pending dequeues
<<<<<<< HEAD
    std::deque<QueuedMessage> pd;
    {
        Mutex::ScopedLock locker(messageLock);
        pendingDequeues.swap(pd);
    }
    for_each(pd.begin(), pd.end(), boost::bind(&Queue::dequeue, this, (TransactionContext*) 0, _1));
}

void Queue::insertSequenceNumbers(const std::string& key)
{
    seqNoKey = key;
    insertSeqNo = !seqNoKey.empty();
    QPID_LOG(debug, "Inserting sequence numbers as " << key);
=======
    for (std::vector<Message>::iterator i = pendingDequeues.begin(); i != pendingDequeues.end(); ++i) {
        dequeueFromStore(i->getPersistentContext());
    }
    pendingDequeues.clear();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

/** updates queue observers and state when a message has become available for transfer
 *  Requires messageLock be held by caller.
 */
<<<<<<< HEAD
void Queue::observeEnqueue(const QueuedMessage& m, const qpid::sys::Mutex::ScopedLock&)
{
    for (Observers::iterator i = observers.begin(); i != observers.end(); ++i) {
        try {
            (*i)->enqueued(m);
        } catch (const std::exception& e) {
            QPID_LOG(warning, "Exception on notification of enqueue for queue " << getName() << ": " << e.what());
        }
    }
}

void Queue::updateEnqueued(const QueuedMessage& m)
{
    if (m.payload) {
        boost::intrusive_ptr<Message> payload = m.payload;
        enqueue(0, payload, true);
        {
            Mutex::ScopedLock locker(messageLock);
            messages->updateAcquired(m);
            observeEnqueue(m, locker);
            if (policy.get()) {
                policy->recoverEnqueued(payload);
                policy->enqueued(m);
            }
        }
        mgntEnqStats(m.payload, mgmtObject, brokerMgmtObject);
    } else {
        QPID_LOG(warning, "Queue informed of enqueued message that has no payload");
    }
}

bool Queue::isEnqueued(const QueuedMessage& msg)
{
    Mutex::ScopedLock locker(messageLock);
    return !policy.get() || policy->isEnqueued(msg);
}

// Note: accessing listeners outside of lock is dangerous.  Caller must ensure the queue's
// state is not changed while listeners is referenced.
QueueListeners& Queue::getListeners() { return listeners; }

// Note: accessing messages outside of lock is dangerous.  Caller must ensure the queue's
// state is not changed while messages is referenced.
Messages& Queue::getMessages() { return *messages; }
const Messages& Queue::getMessages() const { return *messages; }

void Queue::checkNotDeleted(const Consumer::shared_ptr& c)
{
    if (deleted && !c->hideDeletedError()) {
        throw ResourceDeletedException(QPID_MSG("Queue " << getName() << " has been deleted."));
    }
}

void Queue::addObserver(boost::shared_ptr<QueueObserver> observer)
{
    Mutex::ScopedLock lock(messageLock);
    observers.insert(observer);
}

void Queue::removeObserver(boost::shared_ptr<QueueObserver> observer)
{
    Mutex::ScopedLock lock(messageLock);
    observers.erase(observer);
=======
void Queue::observeEnqueue(const Message& m, const Mutex::ScopedLock& l)
{
    observers.enqueued(m, l);
    mgntEnqStats(m, mgmtObject, brokerMgmtObject);
}

bool Queue::checkNotDeleted(const Consumer::shared_ptr& c)
{
    if (deleted && !c->hideDeletedError())
        throw ResourceDeletedException(QPID_MSG("Queue " << getName() << " has been deleted."));
    return !deleted;
}

bool Queue::isDeleted() const
{
    Mutex::ScopedLock lock(messageLock);
    return deleted;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Queue::flush()
{
    ScopedUse u(barrier);
    if (u.acquired && store) store->flush(*this);
}


bool Queue::bind(boost::shared_ptr<Exchange> exchange, const std::string& key,
                 const qpid::framing::FieldTable& arguments)
{
<<<<<<< HEAD
    if (exchange->bind(shared_from_this(), key, &arguments)) {
=======
    if (!isDeleted() && exchange->bind(shared_from_this(), key, &arguments)) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        bound(exchange->getName(), key, arguments);
        if (exchange->isDurable() && isDurable()) {
            store->bind(*exchange, *this, key, arguments);
        }
        return true;
    } else {
        return false;
    }
}


Broker* Queue::getBroker()
{
    return broker;
}

void Queue::setDequeueSincePurge(uint32_t value) {
    dequeueSincePurge = value;
}

<<<<<<< HEAD
namespace{
class FindLowest
{
  public:
    FindLowest() : init(false) {}
    void process(const QueuedMessage& message) {
        QPID_LOG(debug, "FindLowest processing: " << message.position);
        if (!init || message.position < lowest) lowest = message.position;
        init = true;
    }
    bool getLowest(qpid::framing::SequenceNumber& result) {
        if (init) {
            result = lowest;
            return true;
        } else {
            return false;
        }
    }
  private:
    bool init;
    qpid::framing::SequenceNumber lowest;
};
=======
void Queue::reject(const QueueCursor& cursor)
{
    ScopedAutoDelete autodelete(*this);
    Exchange::shared_ptr alternate = getAlternateExchange();
    Message copy;
    boost::intrusive_ptr<PersistableMessage> pmsg;
    {
        Mutex::ScopedLock locker(messageLock);
        Message* message = messages->find(cursor);
        if (message) {
            if (alternate) copy = *message;
            if (message->isPersistent()) pmsg = message->getPersistentContext();
            countRejected();
            observeDequeue(*message, locker, settings.autodelete ? &autodelete : 0);
            messages->deleted(cursor);
        } else {
            return;
        }
    }
    if (alternate) {
        copy.resetDeliveryCount();
        DeliverableMessage delivery(copy, 0);
        alternate->routeWithAlternate(delivery);
        QPID_LOG(info, "Routed rejected message from " << getName() << " to "
                 << alternate->getName());
    } else {
        //just drop it
        QPID_LOG(info, "Dropping rejected message from " << getName());
    }
    dequeueFromStore(pmsg);
}

bool Queue::checkDepth(const QueueDepth& increment, const Message&)
{
    if (settings.maxDepth && (settings.maxDepth - current < increment)) {
        if (mgmtObject) {
            mgmtObject->inc_discardsOverflow();
            if (brokerMgmtObject)
                brokerMgmtObject->inc_discardsOverflow();
        }
        throw ResourceLimitExceededException(QPID_MSG("Maximum depth exceeded on " << name << ": current=[" << current << "], max=[" << settings.maxDepth << "]"));
    } else {
        current += increment;
        return true;
    }
}

bool Queue::seek(QueueCursor& cursor, MessagePredicate predicate)
{
    Mutex::ScopedLock locker(messageLock);
    //hold lock across calls to predicate, or take copy of message?
    //currently hold lock, may want to revise depending on any new use
    //cases
    Message* message = messages->next(cursor);
    while (message && (predicate && !predicate(*message))) {
        message = messages->next(cursor);
    }
    return message != 0;
}

bool Queue::seek(QueueCursor& cursor, MessagePredicate predicate, qpid::framing::SequenceNumber start)
{
    Mutex::ScopedLock locker(messageLock);
    //hold lock across calls to predicate, or take copy of message?
    //currently hold lock, may want to revise depending on any new use
    //cases
    Message* message;
    message = messages->find(start, &cursor);
    if (message && (!predicate || predicate(*message))) return true;

    return seek(cursor, predicate);
}

bool Queue::seek(QueueCursor& cursor, qpid::framing::SequenceNumber start)
{
    Mutex::ScopedLock locker(messageLock);
    return messages->find(start, &cursor);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Queue::UsageBarrier::UsageBarrier(Queue& q) : parent(q), count(0) {}

bool Queue::UsageBarrier::acquire()
{
<<<<<<< HEAD
    Monitor::ScopedLock l(parent.messageLock);  /** @todo: use a dedicated lock instead of messageLock */
=======
    Monitor::ScopedLock l(usageLock);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (parent.deleted) {
        return false;
    } else {
        ++count;
        return true;
    }
}

void Queue::UsageBarrier::release()
{
<<<<<<< HEAD
    Monitor::ScopedLock l(parent.messageLock);
    if (--count == 0) parent.messageLock.notifyAll();
=======
    Monitor::ScopedLock l(usageLock);
    if (--count == 0) usageLock.notifyAll();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Queue::UsageBarrier::destroy()
{
<<<<<<< HEAD
    Monitor::ScopedLock l(parent.messageLock);
    parent.deleted = true;
    while (count) parent.messageLock.wait();
=======
    Monitor::ScopedLock l(usageLock);
    parent.deleted = true;
    while (count) usageLock.wait();
}

void Queue::addArgument(const string& key, const types::Variant& value) {
    settings.original[key] = value;
    qpid::amqp_0_10::translate(settings.asMap(), encodableSettings);
    boost::shared_ptr<qpid::framing::FieldValue> v;
    qpid::amqp_0_10::translate(value, v);
    settings.storeSettings.set(key, v);
    if (mgmtObject != 0) mgmtObject->set_arguments(settings.asMap());
}


void Queue::setRedirectPeer ( Queue::shared_ptr peer, bool isSrc) {
    Mutex::ScopedLock locker(messageLock);
    redirectPeer = peer;
    redirectSource = isSrc;
}

void Queue::setMgmtRedirectState( std::string peer, bool enabled, bool isSrc ) {
    if (mgmtObject != 0) {
        mgmtObject->set_redirectPeer(enabled ? peer : "");
        mgmtObject->set_redirectSource(isSrc);
    }
}

void Queue::setOwningUser(std::string& _userId) {
    userId  = _userId;
    if (mgmtObject != 0)
       mgmtObject->set_creator(userId);
}

bool Queue::reroute(boost::shared_ptr<Exchange> e, const Message& m)
{
    if (e) {
        DeliverableMessage d(m, 0);
        d.getMessage().clearTrace();
        e->routeWithAlternate(d);
        return true;
    } else {
        return false;
    }
}

Queue::QueueUsers::QueueUsers() : consumers(0), browsers(0), others(0), controller(false) {}
void Queue::QueueUsers::addConsumer() { ++consumers; }
void Queue::QueueUsers::addBrowser() { ++browsers; }
void Queue::QueueUsers::addLifecycleController() { assert(!controller); controller = true; }
void Queue::QueueUsers::addOther(){ ++others; }
void Queue::QueueUsers::removeConsumer() { assert(consumers > 0); --consumers; }
void Queue::QueueUsers::removeBrowser() { assert(browsers > 0); --browsers; }
void Queue::QueueUsers::removeLifecycleController() { assert(controller); controller = false; }
void Queue::QueueUsers::removeOther() { assert(others > 0); --others; }
bool Queue::QueueUsers::isInUseByController() const { return controller; }
bool Queue::QueueUsers::isUsed() const { return controller || consumers || browsers || others; }
uint32_t Queue::QueueUsers::getSubscriberCount() const { return consumers + browsers; }
bool Queue::QueueUsers::hasConsumers() const { return consumers; }

Queue::ScopedAutoDelete::ScopedAutoDelete(Queue& q) : queue(q), eligible(false) {}
void Queue::ScopedAutoDelete::check(const sys::Mutex::ScopedLock& lock)
{
    eligible = queue.checkAutoDelete(lock);
}
Queue::ScopedAutoDelete::~ScopedAutoDelete()
{
    if (eligible)  queue.scheduleAutoDelete();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

}}

