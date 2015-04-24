#ifndef _broker_Message_h
#define _broker_Message_h

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
#include "qpid/broker/BrokerImportExport.h"
#include "qpid/broker/PersistableMessage.h"
#include "qpid/broker/MessageAdapter.h"
#include "qpid/framing/amqp_types.h"
#include "qpid/sys/Monitor.h"
#include "qpid/sys/Time.h"
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

namespace qpid {

namespace framing {
class AMQBody;
class AMQHeaderBody;
class FieldTable;
class SequenceNumber;
}

namespace broker {
class ConnectionToken;
class Exchange;
class ExchangeRegistry;
class MessageStore;
class Queue;
class ExpiryPolicy;

class Message : public PersistableMessage {
public:
    typedef boost::function<void (const boost::intrusive_ptr<Message>&)> MessageCallback;

    QPID_BROKER_EXTERN Message(const framing::SequenceNumber& id = framing::SequenceNumber());
    QPID_BROKER_EXTERN ~Message();

    uint64_t getPersistenceId() const { return persistenceId; }
    void setPersistenceId(uint64_t _persistenceId) const { persistenceId = _persistenceId; }

    bool getRedelivered() const { return redelivered; }
    void redeliver() { redelivered = true; }

    const ConnectionToken* getPublisher() const {  return publisher; }
    void setPublisher(ConnectionToken* p) {  publisher = p; }

    const framing::SequenceNumber& getCommandId() { return frames.getId(); }

    QPID_BROKER_EXTERN uint64_t contentSize() const;

    QPID_BROKER_EXTERN std::string getRoutingKey() const;
    const boost::shared_ptr<Exchange> getExchange(ExchangeRegistry&) const;
    QPID_BROKER_EXTERN std::string getExchangeName() const;
    bool isImmediate() const;
    QPID_BROKER_EXTERN const framing::FieldTable* getApplicationHeaders() const;
    QPID_BROKER_EXTERN std::string getAppId() const;
    QPID_BROKER_EXTERN bool isPersistent() const;
    bool requiresAccept();

    /** determine msg expiration time using the TTL value if present */
    QPID_BROKER_EXTERN void computeExpiration(const boost::intrusive_ptr<ExpiryPolicy>& e);
    void setExpiryPolicy(const boost::intrusive_ptr<ExpiryPolicy>& e);
    bool hasExpired();
    sys::AbsTime getExpiration() const { return expiration; }
    void setExpiration(sys::AbsTime exp) { expiration = exp; }
    void adjustTtl();
    void setRedelivered();
    QPID_BROKER_EXTERN void insertCustomProperty(const std::string& key, int64_t value);
    QPID_BROKER_EXTERN void insertCustomProperty(const std::string& key, const std::string& value);
    QPID_BROKER_EXTERN void removeCustomProperty(const std::string& key);
    void setExchange(const std::string&);
    void clearApplicationHeadersFlag();
    /** set the timestamp delivery property to the current time-of-day */
    QPID_BROKER_EXTERN void setTimestamp();

    framing::FrameSet& getFrames() { return frames; }
    const framing::FrameSet& getFrames() const { return frames; }

    template <class T> const T* getProperties() const {
        const qpid::framing::AMQHeaderBody* p = frames.getHeaders();
        return p->get<T>();
    }

    template <class T> const T* hasProperties() const {
        const qpid::framing::AMQHeaderBody* p = frames.getHeaders();
        return p->get<T>();
    }

    template <class T> void eraseProperties() {
        qpid::framing::AMQHeaderBody* p = frames.getHeaders();
        p->erase<T>();
    }

    template <class T> const T* getMethod() const {
        return frames.as<T>();
    }

    template <class T> T* getMethod() {
        return frames.as<T>();
    }

    template <class T> bool isA() const {
        return frames.isA<T>();
    }

    uint32_t getRequiredCredit();

    void encode(framing::Buffer& buffer) const;
    void encodeContent(framing::Buffer& buffer) const;

    /**
     * @returns the size of the buffer needed to encode this
     * message in its entirety
     */
    uint32_t encodedSize() const;
    /**
     * @returns the size of the buffer needed to encode the
     * 'header' of this message (not just the header frame,
     * but other meta data e.g.routing key and exchange)
     */
    uint32_t encodedHeaderSize() const;
    uint32_t encodedContentSize() const;

    QPID_BROKER_EXTERN void decodeHeader(framing::Buffer& buffer);
    QPID_BROKER_EXTERN void decodeContent(framing::Buffer& buffer);

    void QPID_BROKER_EXTERN tryReleaseContent();
    void releaseContent();
    void releaseContent(MessageStore* s);//deprecated, use 'setStore(store); releaseContent();' instead
    void destroy();

    bool getContentFrame(const Queue& queue, framing::AMQFrame& frame, uint16_t maxContentSize, uint64_t offset) const;
    QPID_BROKER_EXTERN void sendContent(const Queue& queue, framing::FrameHandler& out, uint16_t maxFrameSize) const;
    void sendHeader(framing::FrameHandler& out, uint16_t maxFrameSize) const;

    QPID_BROKER_EXTERN bool isContentLoaded() const;

    bool isExcluded(const std::vector<std::string>& excludes) const;
    void addTraceId(const std::string& id);
    void clearTrace();

    void forcePersistent();
    bool isForcedPersistent();

    /** Call cb when dequeue is complete, may call immediately. Holds cb by reference. */
    void setDequeueCompleteCallback(MessageCallback& cb);
    void resetDequeueCompleteCallback();

    uint8_t getPriority() const;
    bool getIsManagementMessage() const;
    void setIsManagementMessage(bool b);
  private:
    MessageAdapter& getAdapter() const;
    void allDequeuesComplete();

    mutable sys::Mutex lock;
    framing::FrameSet frames;
    mutable boost::shared_ptr<Exchange> exchange;
    mutable uint64_t persistenceId;
    bool redelivered;
    bool loaded;
    bool staged;
    bool forcePersistentPolicy; // used to force message as durable, via a broker policy
    ConnectionToken* publisher;
    mutable MessageAdapter* adapter;
    qpid::sys::AbsTime expiration;
    boost::intrusive_ptr<ExpiryPolicy> expiryPolicy;

    static TransferAdapter TRANSFER;

    mutable boost::intrusive_ptr<Message> empty;

    sys::Monitor callbackLock;
    MessageCallback* dequeueCallback;
    bool inCallback;

    uint32_t requiredCredit;
    bool isManagementMessage;
      mutable bool copyHeaderOnWrite;

    /**
     * Expects lock to be held
     */
    template <class T> T* getModifiableProperties() {
        return getHeaderBody()->get<T>(true);
    }
    qpid::framing::AMQHeaderBody* getHeaderBody();
=======
#include "qpid/RefCounted.h"
#include "qpid/broker/PersistableMessage.h"
//TODO: move the following out of framing or replace it
#include "qpid/framing/SequenceNumber.h"
#include "qpid/sys/Time.h"
#include "qpid/types/Variant.h"

#include "qpid/broker/BrokerImportExport.h"

#include <string>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace qpid {
namespace amqp {
class MapHandler;
struct MessageId;
}

namespace management {
class ObjectId;
class Manageable;
}

namespace broker {
class OwnershipToken;
class Connection;

enum MessageState
{
    AVAILABLE=1,
    ACQUIRED=2,
    DELETED=4,
    UNAVAILABLE=8
};

class Message {
public:
    class Encoding : public IngressCompletion
    {
      public:
        virtual ~Encoding() {}
        virtual std::string getRoutingKey() const = 0;
        virtual bool isPersistent() const = 0;
        virtual uint8_t getPriority() const = 0;
        virtual uint64_t getMessageSize() const = 0;
        virtual qpid::amqp::MessageId getMessageId() const = 0;
        virtual qpid::amqp::MessageId getCorrelationId() const = 0;
        virtual std::string getPropertyAsString(const std::string& key) const = 0;
        virtual std::string getAnnotationAsString(const std::string& key) const = 0;
        virtual bool getTtl(uint64_t&) const = 0;
        virtual std::string getContent() const = 0;
        virtual void processProperties(qpid::amqp::MapHandler&) const = 0;
        virtual std::string getUserId() const = 0;
        virtual uint64_t getTimestamp() const = 0;
    };

    class SharedState : public Encoding
    {
      public:
        virtual ~SharedState() {}
        virtual const Connection* getPublisher() const = 0;
        virtual void setPublisher(const Connection* p) = 0;

        virtual void setExpiration(sys::AbsTime e) = 0;
        virtual sys::AbsTime getExpiration() const = 0;
        virtual sys::Duration getTimeToExpiration() const = 0;
        virtual void computeExpiration() = 0;

        virtual bool getIsManagementMessage() const = 0;
        virtual void setIsManagementMessage(bool b) = 0;
    };

    class SharedStateImpl : public SharedState
    {
        const Connection* publisher;
        qpid::sys::AbsTime expiration;
        bool isManagementMessage;
      public:
        QPID_BROKER_EXTERN SharedStateImpl();
        virtual ~SharedStateImpl() {}
        QPID_BROKER_EXTERN const Connection* getPublisher() const;
        QPID_BROKER_EXTERN void setPublisher(const Connection* p);
        QPID_BROKER_EXTERN void setExpiration(sys::AbsTime e);
        QPID_BROKER_EXTERN sys::AbsTime getExpiration() const;
        QPID_BROKER_EXTERN sys::Duration getTimeToExpiration() const;
        QPID_BROKER_EXTERN void computeExpiration();
        QPID_BROKER_EXTERN bool getIsManagementMessage() const;
        QPID_BROKER_EXTERN void setIsManagementMessage(bool b);
    };

    QPID_BROKER_EXTERN Message(boost::intrusive_ptr<SharedState>, boost::intrusive_ptr<PersistableMessage>);
    QPID_BROKER_EXTERN Message();
    QPID_BROKER_EXTERN ~Message();

    bool isRedelivered() const { return deliveryCount > 0; }
    bool hasBeenAcquired() const { return alreadyAcquired; }
    void deliver() { ++deliveryCount; alreadyAcquired |= (deliveryCount>0); }
    void undeliver() { --deliveryCount; }
    int getDeliveryCount() const { return deliveryCount; }
    void resetDeliveryCount() { deliveryCount = -1; alreadyAcquired = false; }

    const Connection* getPublisher() const;
    bool isLocalTo(const OwnershipToken*) const;

    QPID_BROKER_EXTERN std::string getRoutingKey() const;
    QPID_BROKER_EXTERN bool isPersistent() const;

    QPID_BROKER_EXTERN sys::AbsTime getExpiration() const;
    uint64_t getTtl() const;
    QPID_BROKER_EXTERN bool getTtl(uint64_t&) const;

    QPID_BROKER_EXTERN uint64_t getTimestamp() const;

    QPID_BROKER_EXTERN void addAnnotation(const std::string& key, const qpid::types::Variant& value);
    QPID_BROKER_EXTERN bool isExcluded(const std::vector<std::string>& excludes) const;
    QPID_BROKER_EXTERN void addTraceId(const std::string& id);
    QPID_BROKER_EXTERN void clearTrace();
    QPID_BROKER_EXTERN uint8_t getPriority() const;
    QPID_BROKER_EXTERN std::string getPropertyAsString(const std::string& key) const;
    QPID_BROKER_EXTERN qpid::types::Variant getProperty(const std::string& key) const;
    void processProperties(qpid::amqp::MapHandler&) const;

    QPID_BROKER_EXTERN uint64_t getMessageSize() const;

    QPID_BROKER_EXTERN const Encoding& getEncoding() const;
    QPID_BROKER_EXTERN operator bool() const;
    QPID_BROKER_EXTERN SharedState& getSharedState();

    bool getIsManagementMessage() const;

    QPID_BROKER_EXTERN qpid::framing::SequenceNumber getSequence() const;
    QPID_BROKER_EXTERN void setSequence(const qpid::framing::SequenceNumber&);

    MessageState getState() const;
    void setState(MessageState);

    QPID_BROKER_EXTERN qpid::types::Variant getAnnotation(const std::string& key) const;
    QPID_BROKER_EXTERN const qpid::types::Variant::Map& getAnnotations() const;
    QPID_BROKER_EXTERN std::string getUserId() const;

    QPID_BROKER_EXTERN std::string getContent() const;//Used for ha, management, when content needs to be decoded

    QPID_BROKER_EXTERN boost::intrusive_ptr<AsyncCompletion> getIngressCompletion() const;
    QPID_BROKER_EXTERN boost::intrusive_ptr<PersistableMessage> getPersistentContext() const;
    QPID_BROKER_EXTERN bool hasReplicationId() const;
    QPID_BROKER_EXTERN uint64_t getReplicationId() const;
    QPID_BROKER_EXTERN void setReplicationId(framing::SequenceNumber id);

  private:
    /**
     * Template for optional members that are only constructed when
     * if/when needed, to conserve memory. (Boost::optional doesn't
     * help here).
     */
    template <typename T> class Optional
    {
        boost::scoped_ptr<T> value;
      public:
        Optional() : value(0) {}
        Optional(const Optional<T>& o) : value(o.value ? new T(*o.value) : 0) {}
        T& get()
        {
            if (!value) value.reset(new T);
            return *value;
        }
        const T& operator*() const
        {
            return *value;
        }
        Optional<T>& operator=(const Optional<T>& o)
        {
            if (o.value) {
                if (!value) value.reset(new T(*o.value));
            }
            return *this;
        }
        operator bool() const
        {
            return !!value;
        }
    };


    boost::intrusive_ptr<SharedState> sharedState;
    boost::intrusive_ptr<PersistableMessage> persistentContext;
    int deliveryCount;
    bool alreadyAcquired;
    Optional<qpid::types::Variant::Map> annotations;
    MessageState state;
    qpid::framing::SequenceNumber sequence;
    framing::SequenceNumber replicationId;
    bool isReplicationIdSet:1;

    void annotationsChanged();
    bool getTtl(uint64_t&, uint64_t expiredValue) const;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}}


#endif
