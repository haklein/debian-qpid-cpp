<<<<<<< HEAD
=======

>>>>>>> 3bbfc42... Imported Upstream version 0.32
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


#include "qpid/broker/PersistableMessage.h"
<<<<<<< HEAD
#include "qpid/broker/MessageStore.h"
=======
#include "qpid/broker/Queue.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <iostream>

using namespace qpid::broker;

namespace qpid {
namespace broker {

<<<<<<< HEAD
class MessageStore;

PersistableMessage::~PersistableMessage() {}

PersistableMessage::PersistableMessage() :
    asyncDequeueCounter(0),
    store(0)
{}

void PersistableMessage::flush()
{
    syncList copy;
    {
        sys::ScopedLock<sys::Mutex> l(storeLock);
	if (store) {
	    copy = synclist;
	} else {
            return;//early exit as nothing to do
	}
    }
    for (syncList::iterator i = copy.begin(); i != copy.end(); ++i) {
        PersistableQueue::shared_ptr q(i->lock());
        if (q) {
            q->flush();
        }
    } 
}

void PersistableMessage::setContentReleased()
{
    contentReleaseState.released = true;
}

bool PersistableMessage::isContentReleased() const
{ 
    return contentReleaseState.released;
}
       

bool PersistableMessage::isStoredOnQueue(PersistableQueue::shared_ptr queue){
    if (store && (queue->getPersistenceId()!=0)) {
        for (syncList::iterator i = synclist.begin(); i != synclist.end(); ++i) {
            PersistableQueue::shared_ptr q(i->lock());
            if (q && q->getPersistenceId() == queue->getPersistenceId())  return true;
        } 
    }            
    return false;
}


void PersistableMessage::addToSyncList(PersistableQueue::shared_ptr queue, MessageStore* _store) { 
    if (_store){
        sys::ScopedLock<sys::Mutex> l(storeLock);
        store = _store;
        boost::weak_ptr<PersistableQueue> q(queue);
        synclist.push_back(q);
    }
}

void PersistableMessage::enqueueAsync(PersistableQueue::shared_ptr queue, MessageStore* _store) { 
    addToSyncList(queue, _store);
    enqueueStart();
}

bool PersistableMessage::isDequeueComplete() { 
    sys::ScopedLock<sys::Mutex> l(asyncDequeueLock);
    return asyncDequeueCounter == 0;
}
    
void PersistableMessage::dequeueComplete() { 
    bool notify = false;
    {
        sys::ScopedLock<sys::Mutex> l(asyncDequeueLock);
        if (asyncDequeueCounter > 0) {
            if (--asyncDequeueCounter == 0) {
                notify = true;
            }
        }
    }
    if (notify) allDequeuesComplete();
}

void PersistableMessage::dequeueAsync(PersistableQueue::shared_ptr queue, MessageStore* _store) { 
    if (_store){
        sys::ScopedLock<sys::Mutex> l(storeLock);
        store = _store;
        boost::weak_ptr<PersistableQueue> q(queue);
        synclist.push_back(q);
    }
    dequeueAsync();
}

void PersistableMessage::dequeueAsync() { 
    sys::ScopedLock<sys::Mutex> l(asyncDequeueLock);
    asyncDequeueCounter++; 
}

PersistableMessage::ContentReleaseState::ContentReleaseState() : blocked(false), requested(false), released(false) {}

void PersistableMessage::setStore(MessageStore* s)
{
    store = s;
}

void PersistableMessage::requestContentRelease()
{
    contentReleaseState.requested = true;
}
void PersistableMessage::blockContentRelease()
{ 
    contentReleaseState.blocked = true;
}
bool PersistableMessage::checkContentReleasable()
{ 
    return contentReleaseState.requested && !contentReleaseState.blocked;
}

bool PersistableMessage::isContentReleaseBlocked()
{
    return contentReleaseState.blocked;
}

bool PersistableMessage::isContentReleaseRequested()
{
    return contentReleaseState.requested;
}
=======
PersistableMessage::~PersistableMessage() {}
PersistableMessage::PersistableMessage() : ingressCompletion(0), persistenceId(0) {}

void PersistableMessage::setIngressCompletion(boost::intrusive_ptr<IngressCompletion> i)
{
    ingressCompletion = i.get();
    /**
     * What follows is a hack to account for the fact that the
     * AsyncCompletion to use may be, but is not always, this same
     * object.
     *
     * This is hopefully temporary, and allows the store interface to
     * remain unchanged without requiring another object to be allocated
     * for every message.
     *
     * The case in question is where a message previously passed to
     * the store is modified by some other queue onto which it is
     * pushed, and then again persisted to the store. These will be
     * two separate PersistableMessage instances (since the latter now
     * has different content), but need to share the same
     * AsyncCompletion (since they refer to the same incoming transfer
     * command).
     */
    if (static_cast<RefCounted*>(ingressCompletion) != static_cast<RefCounted*>(this)) {
        holder = i;
    }
}

void PersistableMessage::enqueueAsync(boost::shared_ptr<Queue> q)
{
    enqueueStart();
    ingressCompletion->enqueueAsync(q);
}

void PersistableMessage::dequeueComplete() {}
>>>>>>> 3bbfc42... Imported Upstream version 0.32

}}


