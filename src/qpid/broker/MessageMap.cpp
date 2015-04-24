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
#include "qpid/broker/MessageMap.h"
<<<<<<< HEAD
#include "qpid/broker/QueuedMessage.h"
=======
#include "qpid/broker/Message.h"
#include "qpid/broker/QueueCursor.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/log/Statement.h"
#include <algorithm>

namespace qpid {
namespace broker {
namespace {
const std::string EMPTY;
}

<<<<<<< HEAD
bool MessageMap::deleted(const QueuedMessage& message)
{
    Ordering::iterator i = messages.find(message.position);
    if (i != messages.end()) {
        erase(i);
        return true;
    } else {
        return false;
    }
}

std::string MessageMap::getKey(const QueuedMessage& message)
{
    const framing::FieldTable* ft = message.payload->getApplicationHeaders();
    if (ft) return ft->getAsString(key);
    else return EMPTY;
=======

std::string MessageMap::getKey(const Message& message)
{
    return message.getPropertyAsString(key);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

size_t MessageMap::size()
{
    size_t count(0);
    for (Ordering::iterator i = messages.begin(); i != messages.end(); ++i) {
<<<<<<< HEAD
        if (i->second.status == QueuedMessage::AVAILABLE) ++count;
=======
        if (i->second.getState() == AVAILABLE) ++count;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    return count;
}

bool MessageMap::empty()
{
    return size() == 0;//TODO: more efficient implementation
}

<<<<<<< HEAD
void MessageMap::release(const QueuedMessage& message)
{
    Ordering::iterator i = messages.find(message.position);
    if (i != messages.end() && i->second.status == QueuedMessage::ACQUIRED) {
        i->second.status = QueuedMessage::AVAILABLE;
    }
}

bool MessageMap::acquire(const framing::SequenceNumber& position, QueuedMessage& message)
{
    Ordering::iterator i = messages.find(position);
    if (i != messages.end() && i->second.status == QueuedMessage::AVAILABLE) {
        i->second.status = QueuedMessage::ACQUIRED;
        message = i->second;
=======
bool MessageMap::deleted(const QueueCursor& cursor)
{
    Ordering::iterator i = messages.find(cursor.position);
    if (i != messages.end()) {
        erase(i);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return true;
    } else {
        return false;
    }
}

<<<<<<< HEAD
bool MessageMap::find(const framing::SequenceNumber& position, QueuedMessage& message)
{
    Ordering::iterator i = messages.find(position);
    if (i != messages.end() && i->second.status == QueuedMessage::AVAILABLE) {
        message = i->second;
        return true;
    } else {
        return false;
    }
}

bool MessageMap::browse(const framing::SequenceNumber& position, QueuedMessage& message, bool unacquired)
{
    Ordering::iterator i = messages.lower_bound(position+1);
    if (i != messages.end() && (i->second.status == QueuedMessage::AVAILABLE  || (!unacquired && i->second.status == QueuedMessage::ACQUIRED))) {
        message = i->second;
        return true;
    } else {
        return false;
    }
}

bool MessageMap::consume(QueuedMessage& message)
{
    for (Ordering::iterator i = messages.begin(); i != messages.end(); ++i) {
        if (i->second.status == QueuedMessage::AVAILABLE) {
            i->second.status = QueuedMessage::ACQUIRED;
            message = i->second;
            return true;
        }
    }
    return false;
}

const QueuedMessage& MessageMap::replace(const QueuedMessage& original, const QueuedMessage& update)
{
    messages.erase(original.position);
    messages[update.position] = update;
    return update;
}

bool MessageMap::push(const QueuedMessage& added, QueuedMessage& removed)
=======
Message* MessageMap::find(const QueueCursor& cursor)
{
    if (cursor.valid) return find(cursor.position, 0);
    else return 0;
}

Message* MessageMap::find(const framing::SequenceNumber& position, QueueCursor* cursor)
{
    Ordering::iterator i = messages.lower_bound(position);
    if (i != messages.end()) {
        if (cursor) cursor->setPosition(i->first, version);
        if (i->first == position) return &(i->second);
        else return 0;
    } else {
        //there is no message whose sequence is greater than position,
        //i.e. haven't got there yet
        if (cursor) cursor->setPosition(position, version);
        return 0;
    }
}

Message* MessageMap::next(QueueCursor& cursor)
{
    Ordering::iterator i;
    if (!cursor.valid) i = messages.begin(); //start with oldest message
    else i = messages.upper_bound(cursor.position); //get first message that is greater than position

    while (i != messages.end()) {
        Message& m = i->second;
        cursor.setPosition(m.getSequence(), version);
        if (cursor.check(m)) {
            return &m;
        } else {
            ++i;
        }
    }
    return 0;
}

const Message& MessageMap::replace(const Message& original, const Message& update)
{
    messages.erase(original.getSequence());
    std::pair<Ordering::iterator, bool> i = messages.insert(Ordering::value_type(update.getSequence(), update));
    i.first->second.setState(AVAILABLE);
    return i.first->second;
}

void MessageMap::publish(const Message& added)
{
    Message dummy;
    update(added, dummy);
}

bool MessageMap::update(const Message& added, Message& removed)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::pair<Index::iterator, bool> result = index.insert(Index::value_type(getKey(added), added));
    if (result.second) {
        //there was no previous message for this key; nothing needs to
        //be removed, just add the message into its correct position
<<<<<<< HEAD
        QueuedMessage& a = messages[added.position];
        a = added;
        a.status = QueuedMessage::AVAILABLE;
        QPID_LOG(debug, "Added message " << a);
=======
        messages.insert(Ordering::value_type(added.getSequence(), added)).first->second.setState(AVAILABLE);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return false;
    } else {
        //there is already a message with that key which needs to be replaced
        removed = result.first->second;
        result.first->second = replace(result.first->second, added);
<<<<<<< HEAD
        result.first->second.status = QueuedMessage::AVAILABLE;
        QPID_LOG(debug, "Displaced message " << removed << " with " << result.first->second << ": " << result.first->first);
=======
        result.first->second.setState(AVAILABLE);
        QPID_LOG(debug, "Displaced message at " << removed.getSequence() << " with " << result.first->second.getSequence() << ": " << result.first->first);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return true;
    }
}

<<<<<<< HEAD
void MessageMap::setPosition(const framing::SequenceNumber& seq) {
    // Nothing to do, just assert that the precondition is respected and there
    // are no undeleted messages after seq.
    (void) seq; assert(messages.empty() || (--messages.end())->first <= seq);
}

void MessageMap::foreach(Functor f)
{
    for (Ordering::iterator i = messages.begin(); i != messages.end(); ++i) {
        if (i->second.status == QueuedMessage::AVAILABLE) f(i->second);
    }
}

void MessageMap::removeIf(Predicate p)
{
    for (Ordering::iterator i = messages.begin(); i != messages.end();) {
        if (i->second.status == QueuedMessage::AVAILABLE && p(i->second)) {
            index.erase(getKey(i->second));
            //Note: Removing from messages means that the subsequent
            //call to deleted() for the same message will return
            //false. At present that is not a problem. If this were
            //changed to hold onto the message until dequeued
            //(e.g. with REMOVED state), then the erase() below would
            //need to take that into account.
            messages.erase(i++);
        } else {
            ++i;
        }
=======
Message* MessageMap::release(const QueueCursor& cursor)
{
    Ordering::iterator i = messages.find(cursor.position);
    if (i != messages.end()) {
        i->second.setState(AVAILABLE);
        return &i->second;
    } else {
        return 0;
    }
}

void MessageMap::foreach(Functor f)
{
    for (Ordering::iterator i = messages.begin(); i != messages.end(); ++i) {
        if (i->second.getState() == AVAILABLE) f(i->second);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

void MessageMap::erase(Ordering::iterator i)
{
    index.erase(getKey(i->second));
    messages.erase(i);
}

<<<<<<< HEAD
MessageMap::MessageMap(const std::string& k) : key(k) {}
=======
MessageMap::MessageMap(const std::string& k) : key(k), version(0) {}
>>>>>>> 3bbfc42... Imported Upstream version 0.32

}} // namespace qpid::broker
