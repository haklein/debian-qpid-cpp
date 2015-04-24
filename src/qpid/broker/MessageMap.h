#ifndef QPID_BROKER_MESSAGEMAP_H
#define QPID_BROKER_MESSAGEMAP_H

/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
<<<<<<< HEAD
o * regarding copyright ownership.  The ASF licenses this file
=======
 * regarding copyright ownership.  The ASF licenses this file
>>>>>>> 3bbfc42... Imported Upstream version 0.32
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
#include "qpid/broker/Messages.h"
<<<<<<< HEAD
=======
#include "qpid/broker/Message.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/framing/SequenceNumber.h"
#include <map>
#include <string>

namespace qpid {
namespace broker {

/**
 * Provides a last value queue behaviour, whereby a messages replace
 * any previous message with the same value for a defined property
 * (i.e. the key).
 */
class MessageMap : public Messages
{
  public:
    MessageMap(const std::string& key);
<<<<<<< HEAD
    virtual ~MessageMap() {}
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    size_t size();
    bool empty();

<<<<<<< HEAD
    virtual bool deleted(const QueuedMessage&);
    void release(const QueuedMessage&);
    virtual bool acquire(const framing::SequenceNumber&, QueuedMessage&);
    bool find(const framing::SequenceNumber&, QueuedMessage&);
    virtual bool browse(const framing::SequenceNumber&, QueuedMessage&, bool);
    bool consume(QueuedMessage&);
    virtual bool push(const QueuedMessage& added, QueuedMessage& removed);
    void setPosition(const framing::SequenceNumber&);

    void foreach(Functor);
    virtual void removeIf(Predicate);

  protected:
    typedef std::map<std::string, QueuedMessage> Index;
    typedef std::map<framing::SequenceNumber, QueuedMessage> Ordering;
    const std::string key;
    Index index;
    Ordering messages;

    std::string getKey(const QueuedMessage&);
    virtual const QueuedMessage& replace(const QueuedMessage&, const QueuedMessage&);
=======
    bool deleted(const QueueCursor&);
    void publish(const Message& added);//use update instead to get replaced message
    Message* next(QueueCursor&);
    Message* release(const QueueCursor& cursor);
    Message* find(const QueueCursor&);
    Message* find(const framing::SequenceNumber&, QueueCursor*);

    void foreach(Functor);

    bool update(const Message& added, Message& removed);

  protected:
    typedef std::map<std::string, Message> Index;
    typedef std::map<framing::SequenceNumber, Message> Ordering;
    const std::string key;
    Index index;
    Ordering messages;
    int32_t version;

    std::string getKey(const Message&);
    virtual const Message& replace(const Message&, const Message&);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    void erase(Ordering::iterator);
};
}} // namespace qpid::broker

#endif  /*!QPID_BROKER_MESSAGEMAP_H*/
