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
#include "qpid/broker/DeliveryRecord.h"
#include "qpid/broker/DeliverableMessage.h"
#include "qpid/broker/SemanticState.h"
#include "qpid/broker/Consumer.h"
#include "qpid/broker/Exchange.h"
#include "qpid/broker/Queue.h"
<<<<<<< HEAD
=======
#include "qpid/broker/amqp_0_10/MessageTransfer.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/log/Statement.h"
#include "qpid/framing/FrameHandler.h"
#include "qpid/framing/MessageTransferBody.h"

using namespace qpid;
using namespace qpid::broker;
using std::string;

<<<<<<< HEAD
DeliveryRecord::DeliveryRecord(const QueuedMessage& _msg,
=======
DeliveryRecord::DeliveryRecord(const QueueCursor& _msg,
                               framing::SequenceNumber _msgId,
                               framing::SequenceNumber _replicationId,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                               const Queue::shared_ptr& _queue,
                               const std::string& _tag,
                               const boost::shared_ptr<Consumer>& _consumer,
                               bool _acquired,
                               bool accepted,
                               bool _windowing,
<<<<<<< HEAD
                               uint32_t _credit):
    msg(_msg),
    queue(_queue),
    tag(_tag),
    consumer(_consumer),
    acquired(_acquired),
    acceptExpected(!accepted),
    cancelled(false),
    completed(false),
    ended(accepted && acquired),
    windowing(_windowing),
    credit(msg.payload ? msg.payload->getRequiredCredit() : _credit)
=======
                               uint32_t _credit) : msg(_msg),
                                                   queue(_queue),
                                                   tag(_tag),
                                                   consumer(_consumer),
                                                   acquired(_acquired),
                                                   acceptExpected(!accepted),
                                                   cancelled(false),
                                                   completed(false),
                                                   ended(accepted && acquired),
                                                   windowing(_windowing),
                                                   credit(_credit),
                                                   msgId(_msgId),
                                                   replicationId(_replicationId)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{}

bool DeliveryRecord::setEnded()
{
    ended = true;
<<<<<<< HEAD
    //reset msg pointer, don't need to hold on to it anymore
    msg.payload = boost::intrusive_ptr<Message>();
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    QPID_LOG(debug, "DeliveryRecord::setEnded() id=" << id);
    return isRedundant();
}

<<<<<<< HEAD
void DeliveryRecord::redeliver(SemanticState* const session) {
    if (!ended) {
        if(cancelled){
            //if subscription was cancelled, requeue it (waiting for
            //final confirmation for AMQP WG on this case)
            requeue();
        }else{
            msg.payload->redeliver();//mark as redelivered
            session->deliver(*this, false);
        }
    }
}

void DeliveryRecord::deliver(framing::FrameHandler& h, DeliveryId deliveryId, uint16_t framesize)
{
    id = deliveryId;
    if (msg.payload->getRedelivered()){
        msg.payload->setRedelivered();
    }
    msg.payload->adjustTtl();

    framing::AMQFrame method((framing::MessageTransferBody(framing::ProtocolVersion(), tag, acceptExpected ? 0 : 1, acquired ? 0 : 1)));
    method.setEof(false);
    h.handle(method);
    msg.payload->sendHeader(h, framesize);
    msg.payload->sendContent(*queue, h, framesize);
}

void DeliveryRecord::requeue() const
{
    if (acquired && !ended) {
        msg.payload->redeliver();
        queue->requeue(msg);
=======
void DeliveryRecord::requeue()
{
    if (acquired && !ended) {
        queue->release(msg);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

void DeliveryRecord::release(bool setRedelivered)
{
    if (acquired && !ended) {
<<<<<<< HEAD
        if (setRedelivered) msg.payload->redeliver();
        queue->requeue(msg);
=======
        queue->release(msg, setRedelivered);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        acquired = false;
        setEnded();
    } else {
        QPID_LOG(debug, "Ignoring release for " << id << " acquired=" << acquired << ", ended =" << ended);
    }
}

<<<<<<< HEAD
void DeliveryRecord::complete()  {
=======
void DeliveryRecord::complete()
{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    completed = true;
}

bool DeliveryRecord::accept(TransactionContext* ctxt) {
    if (!ended) {
<<<<<<< HEAD
        if (consumer) consumer->acknowledged(getMessage());
=======
        if (consumer) consumer->acknowledged(*this);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (acquired) queue->dequeue(ctxt, msg);
        setEnded();
        QPID_LOG(debug, "Accepted " << id);
    }
    return isRedundant();
}

<<<<<<< HEAD
void DeliveryRecord::dequeue(TransactionContext* ctxt) const{
=======
void DeliveryRecord::dequeue(TransactionContext* ctxt) const
{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (acquired && !ended) {
        queue->dequeue(ctxt, msg);
    }
}

<<<<<<< HEAD
void DeliveryRecord::committed() const{
    queue->dequeueCommitted(msg);
=======
void DeliveryRecord::committed() const
{
    if (acquired && !ended) {
        queue->dequeueCommitted(msg);
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void DeliveryRecord::reject()
{
    if (acquired && !ended) {
<<<<<<< HEAD
        Exchange::shared_ptr alternate = queue->getAlternateExchange();
        if (alternate) {
            DeliverableMessage delivery(msg.payload);
            alternate->routeWithAlternate(delivery);
            QPID_LOG(info, "Routed rejected message from " << queue->getName() << " to "
                     << alternate->getName());
        } else {
            //just drop it
            QPID_LOG(info, "Dropping rejected message from " << queue->getName());
        }
        queue->countRejected();
        dequeue();
=======
        queue->reject(msg);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        setEnded();
    }
}

uint32_t DeliveryRecord::getCredit() const
{
    return credit;
}

void DeliveryRecord::acquire(DeliveryIds& results) {
    if (queue->acquire(msg, tag)) {
        acquired = true;
        results.push_back(id);
        if (!acceptExpected) {
            if (ended) { QPID_LOG(error, "Can't dequeue ended message"); }
            else { queue->dequeue(0, msg); setEnded(); }
        }
    } else {
        QPID_LOG(info, "Message already acquired " << id.getValue());
    }
}

void DeliveryRecord::cancel(const std::string& cancelledTag)
{
    if (tag == cancelledTag)
        cancelled = true;
}

AckRange DeliveryRecord::findRange(DeliveryRecords& records, DeliveryId first, DeliveryId last)
{
    DeliveryRecords::iterator start = lower_bound(records.begin(), records.end(), first);
    // Find end - position it just after the last record in range
    DeliveryRecords::iterator end = lower_bound(records.begin(), records.end(), last);
    if (end != records.end() && end->getId() == last) ++end;
    return AckRange(start, end);
}


namespace qpid {
namespace broker {

std::ostream& operator<<(std::ostream& out, const DeliveryRecord& r)
{
    out << "{" << "id=" << r.id.getValue();
    out << ", tag=" << r.tag << "}";
    out << ", queue=" << r.queue->getName() << "}";
    return out;
}


}}
