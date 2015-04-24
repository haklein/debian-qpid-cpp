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

#include "qpid/broker/Message.h"
<<<<<<< HEAD
#include "qpid/broker/Queue.h"
#include "qpid/broker/ExchangeRegistry.h"
#include "qpid/broker/ExpiryPolicy.h"
#include "qpid/StringUtils.h"
#include "qpid/framing/frame_functors.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/MessageTransferBody.h"
#include "qpid/framing/SendContent.h"
#include "qpid/framing/SequenceNumber.h"
#include "qpid/framing/TypeFilter.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/log/Statement.h"

=======

#include "qpid/amqp/CharSequence.h"
#include "qpid/amqp/MapHandler.h"
#include "qpid/broker/Connection.h"
#include "qpid/broker/OwnershipToken.h"
#include "qpid/management/ManagementObject.h"
#include "qpid/management/Manageable.h"
#include "qpid/StringUtils.h"
#include "qpid/log/Statement.h"
#include "qpid/assert.h"

#include <algorithm>
#include <string.h>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <time.h>

using boost::intrusive_ptr;
using qpid::sys::AbsTime;
using qpid::sys::Duration;
using qpid::sys::TIME_MSEC;
using qpid::sys::FAR_FUTURE;
<<<<<<< HEAD
using std::string;
using namespace qpid::framing;
=======
using qpid::amqp::CharSequence;
using qpid::amqp::MapHandler;
using std::string;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

namespace qpid {
namespace broker {

<<<<<<< HEAD
TransferAdapter Message::TRANSFER;

Message::Message(const framing::SequenceNumber& id) :
    frames(id), persistenceId(0), redelivered(false), loaded(false),
    staged(false), forcePersistentPolicy(false), publisher(0), adapter(0),
    expiration(FAR_FUTURE), dequeueCallback(0),
    inCallback(false), requiredCredit(0), isManagementMessage(false), copyHeaderOnWrite(false)
{}

Message::~Message() {}

void Message::forcePersistent()
{
    sys::Mutex::ScopedLock l(lock);
    // only set forced bit if we actually need to force.
    if (! getAdapter().isPersistent(frames) ){
        forcePersistentPolicy = true;
    }
}

bool Message::isForcedPersistent()
{
    return forcePersistentPolicy;
}

std::string Message::getRoutingKey() const
{
    return getAdapter().getRoutingKey(frames);
}

std::string Message::getExchangeName() const
{
    return getAdapter().getExchange(frames);
}

const boost::shared_ptr<Exchange> Message::getExchange(ExchangeRegistry& registry) const
{
    if (!exchange) {
        exchange = registry.get(getExchangeName());
    }
    return exchange;
}

bool Message::isImmediate() const
{
    return getAdapter().isImmediate(frames);
}

const FieldTable* Message::getApplicationHeaders() const
{
    sys::Mutex::ScopedLock l(lock);
    return getAdapter().getApplicationHeaders(frames);
}

std::string Message::getAppId() const
{
    sys::Mutex::ScopedLock l(lock);
    return getAdapter().getAppId(frames);
}

bool Message::isPersistent() const
{
    sys::Mutex::ScopedLock l(lock);
    return (getAdapter().isPersistent(frames) || forcePersistentPolicy);
}

bool Message::requiresAccept()
{
    return getAdapter().requiresAccept(frames);
}

uint32_t Message::getRequiredCredit()
{
    sys::Mutex::ScopedLock l(lock);
    if (!requiredCredit) {
        //add up payload for all header and content frames in the frameset
        SumBodySize sum;
        frames.map_if(sum, TypeFilter2<HEADER_BODY, CONTENT_BODY>());
        requiredCredit = sum.getSize();
    }
    return requiredCredit;
}

void Message::encode(framing::Buffer& buffer) const
{
    sys::Mutex::ScopedLock l(lock);
    //encode method and header frames
    EncodeFrame f1(buffer);
    frames.map_if(f1, TypeFilter2<METHOD_BODY, HEADER_BODY>());

    //then encode the payload of each content frame
    framing::EncodeBody f2(buffer);
    frames.map_if(f2, TypeFilter<CONTENT_BODY>());
}

void Message::encodeContent(framing::Buffer& buffer) const
{
    sys::Mutex::ScopedLock l(lock);
    //encode the payload of each content frame
    EncodeBody f2(buffer);
    frames.map_if(f2, TypeFilter<CONTENT_BODY>());
}

uint32_t Message::encodedSize() const
{
    return encodedHeaderSize() + encodedContentSize();
}

uint32_t Message::encodedContentSize() const
{
    sys::Mutex::ScopedLock l(lock);
    return  frames.getContentSize();
}

uint32_t Message::encodedHeaderSize() const
{
    sys::Mutex::ScopedLock l(lock);   // prevent modifications while computing size
    //add up the size for all method and header frames in the frameset
    SumFrameSize sum;
    frames.map_if(sum, TypeFilter2<METHOD_BODY, HEADER_BODY>());
    return sum.getSize();
}

void Message::decodeHeader(framing::Buffer& buffer)
{
    AMQFrame method;
    method.decode(buffer);
    frames.append(method);

    AMQFrame header;
    header.decode(buffer);
    frames.append(header);
}

void Message::decodeContent(framing::Buffer& buffer)
{
    if (buffer.available()) {
        //get the data as a string and set that as the content
        //body on a frame then add that frame to the frameset
        AMQFrame frame((AMQContentBody()));
        frame.castBody<AMQContentBody>()->decode(buffer, buffer.available());
        frame.setFirstSegment(false);
        frames.append(frame);
    } else {
        //adjust header flags
        MarkLastSegment f;
        frames.map_if(f, TypeFilter<HEADER_BODY>());
    }
    //mark content loaded
    loaded = true;
}

// Used for testing only
void Message::tryReleaseContent()
{
    if (checkContentReleasable()) {
        releaseContent();
    }
}

void Message::releaseContent(MessageStore* s)
{
    //deprecated, use setStore(store); releaseContent(); instead
    if (!store) setStore(s);
    releaseContent();
}

void Message::releaseContent()
{
    sys::Mutex::ScopedLock l(lock);
    if (store) {
        if (!getPersistenceId()) {
            intrusive_ptr<PersistableMessage> pmsg(this);
            store->stage(pmsg);
            staged = true;
        }
        //ensure required credit and size is cached before content frames are released
        getRequiredCredit();
        contentSize();
        //remove any content frames from the frameset
        frames.remove(TypeFilter<CONTENT_BODY>());
        setContentReleased();
    }
}

void Message::destroy()
{
    if (staged) {
        if (store) {
            store->destroy(*this);
        } else {
            QPID_LOG(error, "Message content was staged but no store is set so it can't be destroyed");
        }
    }
}

bool Message::getContentFrame(const Queue& queue, AMQFrame& frame, uint16_t maxContentSize, uint64_t offset) const
{
    intrusive_ptr<const PersistableMessage> pmsg(this);

    bool done = false;
    string& data = frame.castBody<AMQContentBody>()->getData();
    store->loadContent(queue, pmsg, data, offset, maxContentSize);
    done = data.size() < maxContentSize;
    frame.setBof(false);
    frame.setEof(true);
    QPID_LOG(debug, "loaded frame" << frame);
    if (offset > 0) {
        frame.setBos(false);
    }
    if (!done) {
        frame.setEos(false);
    } else return false;
    return true;
}

void Message::sendContent(const Queue& queue, framing::FrameHandler& out, uint16_t maxFrameSize) const
{
    sys::Mutex::ScopedLock l(lock);
    if (isContentReleased() && !frames.isComplete()) {
        sys::Mutex::ScopedUnlock u(lock);
        uint16_t maxContentSize = maxFrameSize - AMQFrame::frameOverhead();
        bool morecontent = true;
        for (uint64_t offset = 0; morecontent; offset += maxContentSize)
        {
            AMQFrame frame((AMQContentBody()));
            morecontent = getContentFrame(queue, frame, maxContentSize, offset);
            out.handle(frame);
        }
        queue.countLoadedFromDisk(contentSize());
    } else {
        Count c;
        frames.map_if(c, TypeFilter<CONTENT_BODY>());

        SendContent f(out, maxFrameSize, c.getCount());
        frames.map_if(f, TypeFilter<CONTENT_BODY>());
    }
}

void Message::sendHeader(framing::FrameHandler& out, uint16_t /*maxFrameSize*/) const
{
    sys::Mutex::ScopedLock l(lock);
    Relay f(out);
    frames.map_if(f, TypeFilter<HEADER_BODY>());
    //as frame (and pointer to body) has now been passed to handler,
    //subsequent modifications should use a copy
    copyHeaderOnWrite = true;
}

// TODO aconway 2007-11-09: Obsolete, remove. Was used to cover over
// 0-8/0-9 message differences.
MessageAdapter& Message::getAdapter() const
{
    if (!adapter) {
        if(frames.isA<MessageTransferBody>()) {
            adapter = &TRANSFER;
        } else {
            const AMQMethodBody* method = frames.getMethod();
            if (!method) throw Exception("Can't adapt message with no method");
            else throw Exception(QPID_MSG("Can't adapt message based on " << *method));
        }
    }
    return *adapter;
}

uint64_t Message::contentSize() const
{
    return frames.getContentSize();
}

bool Message::isContentLoaded() const
{
    return loaded;
}


namespace
{
const std::string X_QPID_TRACE("x-qpid.trace");
}

bool Message::isExcluded(const std::vector<std::string>& excludes) const
{
    sys::Mutex::ScopedLock l(lock);
    const FieldTable* headers = getApplicationHeaders();
    if (headers) {
        std::string traceStr = headers->getAsString(X_QPID_TRACE);
        if (traceStr.size()) {
            std::vector<std::string> trace = split(traceStr, ", ");

            for (std::vector<std::string>::const_iterator i = excludes.begin(); i != excludes.end(); i++) {
                for (std::vector<std::string>::const_iterator j = trace.begin(); j != trace.end(); j++) {
                    if (*i == *j) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

class CloneHeaderBody
{
public:
    void operator()(AMQFrame& f)
    {
        f.cloneBody();
    }
};

AMQHeaderBody* Message::getHeaderBody()
{
    // expects lock to be held
    if (copyHeaderOnWrite) {
        CloneHeaderBody f;
        frames.map_if(f, TypeFilter<HEADER_BODY>());
        copyHeaderOnWrite = false;
    }
    return frames.getHeaders();
}

void Message::addTraceId(const std::string& id)
{
    sys::Mutex::ScopedLock l(lock);
    if (isA<MessageTransferBody>()) {
        FieldTable& headers = getModifiableProperties<MessageProperties>()->getApplicationHeaders();
        std::string trace = headers.getAsString(X_QPID_TRACE);
        if (trace.empty()) {
            headers.setString(X_QPID_TRACE, id);
        } else if (trace.find(id) == std::string::npos) {
            trace += ",";
            trace += id;
            headers.setString(X_QPID_TRACE, trace);
        }
    }
}

void Message::clearTrace()
{
    sys::Mutex::ScopedLock l(lock);
    if (isA<MessageTransferBody>()) {
        FieldTable& headers = getModifiableProperties<MessageProperties>()->getApplicationHeaders();
        std::string trace = headers.getAsString(X_QPID_TRACE);
        if (!trace.empty()) {
            headers.setString(X_QPID_TRACE, "");
        }
    }
}

void Message::setTimestamp()
{
    sys::Mutex::ScopedLock l(lock);
    DeliveryProperties* props = getModifiableProperties<DeliveryProperties>();
    time_t now = ::time(0);
    props->setTimestamp(now);   // AMQP-0.10: posix time_t - secs since Epoch
}

void Message::computeExpiration(const boost::intrusive_ptr<ExpiryPolicy>& e)
{
    sys::Mutex::ScopedLock l(lock);
    DeliveryProperties* props = getModifiableProperties<DeliveryProperties>();
    if (props->getTtl()) {
        // AMQP requires setting the expiration property to be posix
        // time_t in seconds. TTL is in milliseconds
        if (!props->getExpiration()) {
            //only set expiration in delivery properties if not already set
            time_t now = ::time(0);
            props->setExpiration(now + (props->getTtl()/1000));
        }
        if (e) {
            // Use higher resolution time for the internal expiry calculation.
            // Prevent overflow as a signed int64_t
            Duration ttl(std::min(props->getTtl() * TIME_MSEC,
                                  (uint64_t) std::numeric_limits<int64_t>::max()));
            expiration = AbsTime(e->getCurrentTime(), ttl);
            setExpiryPolicy(e);
        }
    }
}

void Message::adjustTtl()
{
    sys::Mutex::ScopedLock l(lock);
    DeliveryProperties* props = getModifiableProperties<DeliveryProperties>();
    if (props->getTtl()) {
        if (expiration < FAR_FUTURE) {
            sys::AbsTime current(
                expiryPolicy ? expiryPolicy->getCurrentTime() : sys::AbsTime::now());
            sys::Duration ttl(current, getExpiration());
            // convert from ns to ms; set to 1 if expired
            props->setTtl(int64_t(ttl) >= 1000000 ? int64_t(ttl)/1000000 : 1);
        }
    }
}

void Message::setRedelivered()
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<framing::DeliveryProperties>()->setRedelivered(true);
}

void Message::insertCustomProperty(const std::string& key, int64_t value)
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<MessageProperties>()->getApplicationHeaders().setInt64(key,value);
}

void Message::insertCustomProperty(const std::string& key, const std::string& value)
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<MessageProperties>()->getApplicationHeaders().setString(key,value);
}

void Message::removeCustomProperty(const std::string& key)
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<MessageProperties>()->getApplicationHeaders().erase(key);
}

void Message::setExchange(const std::string& exchange)
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<DeliveryProperties>()->setExchange(exchange);
}

void Message::clearApplicationHeadersFlag()
{
    sys::Mutex::ScopedLock l(lock);
    getModifiableProperties<MessageProperties>()->clearApplicationHeadersFlag();
}

void Message::setExpiryPolicy(const boost::intrusive_ptr<ExpiryPolicy>& e) {
    expiryPolicy = e;
}

bool Message::hasExpired()
{
    return expiryPolicy && expiryPolicy->hasExpired(*this);
}

namespace {
struct ScopedSet {
    sys::Monitor& lock;
    bool& flag;
    ScopedSet(sys::Monitor& l, bool& f) : lock(l), flag(f) {
        sys::Monitor::ScopedLock sl(lock);
        flag = true;
    }
    ~ScopedSet(){
        sys::Monitor::ScopedLock sl(lock);
        flag = false;
        lock.notifyAll();
    }
};
}

void Message::allDequeuesComplete() {
    ScopedSet ss(callbackLock, inCallback);
    MessageCallback* cb = dequeueCallback;
    if (cb && *cb) (*cb)(intrusive_ptr<Message>(this));
}

void Message::setDequeueCompleteCallback(MessageCallback& cb) {
    sys::Mutex::ScopedLock l(callbackLock);
    while (inCallback) callbackLock.wait();
    dequeueCallback = &cb;
}

void Message::resetDequeueCompleteCallback() {
    sys::Mutex::ScopedLock l(callbackLock);
    while (inCallback) callbackLock.wait();
    dequeueCallback = 0;
}

uint8_t Message::getPriority() const {
    sys::Mutex::ScopedLock l(lock);
    return getAdapter().getPriority(frames);
}

bool Message::getIsManagementMessage() const { return isManagementMessage; }
void Message::setIsManagementMessage(bool b) { isManagementMessage = b; }

=======
Message::Message() : deliveryCount(-1), alreadyAcquired(false), replicationId(0), isReplicationIdSet(false)
{}

Message::Message(boost::intrusive_ptr<SharedState> e, boost::intrusive_ptr<PersistableMessage> p)
    : sharedState(e), persistentContext(p), deliveryCount(-1), alreadyAcquired(false), replicationId(0), isReplicationIdSet(false)
{
    if (persistentContext) persistentContext->setIngressCompletion(e);
}

Message::~Message() {}


std::string Message::getRoutingKey() const
{
    return getEncoding().getRoutingKey();
}

bool Message::isPersistent() const
{
    return getEncoding().isPersistent();
}

uint64_t Message::getMessageSize() const
{
    return getEncoding().getMessageSize();
}

boost::intrusive_ptr<AsyncCompletion> Message::getIngressCompletion() const
{
    return sharedState;
}

namespace
{
const std::string X_QPID_TRACE("x-qpid.trace");
}

bool Message::isExcluded(const std::vector<std::string>& excludes) const
{
    std::string traceStr = getEncoding().getAnnotationAsString(X_QPID_TRACE);
    if (traceStr.size()) {
        std::vector<std::string> trace = split(traceStr, ", ");
        for (std::vector<std::string>::const_iterator i = excludes.begin(); i != excludes.end(); i++) {
            for (std::vector<std::string>::const_iterator j = trace.begin(); j != trace.end(); j++) {
                if (*i == *j) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Message::addTraceId(const std::string& id)
{
    std::string trace = getEncoding().getAnnotationAsString(X_QPID_TRACE);
    if (trace.empty()) {
        addAnnotation(X_QPID_TRACE, id);
    } else if (trace.find(id) == std::string::npos) {
        trace += ",";
        trace += id;
        addAnnotation(X_QPID_TRACE, trace);
    }
}

void Message::clearTrace()
{
    addAnnotation(X_QPID_TRACE, std::string());
}

uint64_t Message::getTimestamp() const
{
    return sharedState ? sharedState->getTimestamp() : 0;
}

uint64_t Message::getTtl() const
{
    uint64_t ttl;
    if (getTtl(ttl, 1)/*set to 1 if expired*/) {
        return ttl;
    } else {
        return 0;
    }
}

bool Message::getTtl(uint64_t& ttl) const
{
    return getTtl(ttl, 0); //set to 0 if expired
}

bool Message::getTtl(uint64_t& ttl, uint64_t expiredValue) const
{
    if (sharedState->getTtl(ttl) && sharedState->getExpiration() < FAR_FUTURE) {
        sys::Duration remaining = sharedState->getTimeToExpiration();
        // convert from ns to ms
        ttl = (int64_t(remaining) >= 1000000 ? int64_t(remaining)/1000000 : expiredValue);
        return true;
    } else {
        return false;
    }
}

void Message::addAnnotation(const std::string& key, const qpid::types::Variant& value)
{
    annotations.get()[key] = value;
    annotationsChanged();
}

void Message::annotationsChanged()
{
    if (persistentContext) {
        uint64_t id = persistentContext->getPersistenceId();
        persistentContext = persistentContext->merge(getAnnotations());
        persistentContext->setIngressCompletion(sharedState);
        persistentContext->setPersistenceId(id);
    }
}

uint8_t Message::getPriority() const
{
    return getEncoding().getPriority();
}

bool Message::getIsManagementMessage() const { return sharedState->getIsManagementMessage(); }

const Connection* Message::getPublisher() const { return sharedState->getPublisher(); }
bool Message::isLocalTo(const OwnershipToken* token) const {
    return token && sharedState->getPublisher() && token->isLocal(sharedState->getPublisher());
}


qpid::framing::SequenceNumber Message::getSequence() const
{
    return sequence;
}
void Message::setSequence(const qpid::framing::SequenceNumber& s)
{
    sequence = s;
}

MessageState Message::getState() const
{
    return state;
}
void Message::setState(MessageState s)
{
    state = s;
}
namespace {
const qpid::types::Variant::Map EMPTY_MAP;
}

const qpid::types::Variant::Map& Message::getAnnotations() const
{
    return annotations ? *annotations : EMPTY_MAP;
}

qpid::types::Variant Message::getAnnotation(const std::string& key) const
{
    const qpid::types::Variant::Map& a = getAnnotations();
    qpid::types::Variant::Map::const_iterator i = a.find(key);
    if (i != a.end()) return i->second;
    //FIXME: modify Encoding interface to allow retrieval of
    //annotations of different types from the message data as received
    //off the wire
    return qpid::types::Variant(getEncoding().getAnnotationAsString(key));
}

std::string Message::getUserId() const
{
    return sharedState->getUserId();
}

Message::SharedState& Message::getSharedState()
{
    return *sharedState;
}
const Message::Encoding& Message::getEncoding() const
{
    return *sharedState;
}
Message::operator bool() const
{
    return !!sharedState;
}

std::string Message::getContent() const
{
    return sharedState->getContent();
}

std::string Message::getPropertyAsString(const std::string& key) const
{
    return sharedState->getPropertyAsString(key);
}
namespace {
class PropertyRetriever : public MapHandler
{
  public:
    PropertyRetriever(const std::string& key) : name(key) {}
    void handleVoid(const CharSequence&) {}
    void handleBool(const CharSequence& key, bool value) { handle(key, value); }
    void handleUint8(const CharSequence& key, uint8_t value) { handle(key, value); }
    void handleUint16(const CharSequence& key, uint16_t value) { handle(key, value); }
    void handleUint32(const CharSequence& key, uint32_t value) { handle(key, value); }
    void handleUint64(const CharSequence& key, uint64_t value) { handle(key, value); }
    void handleInt8(const CharSequence& key, int8_t value) { handle(key, value); }
    void handleInt16(const CharSequence& key, int16_t value) { handle(key, value); }
    void handleInt32(const CharSequence& key, int32_t value) { handle(key, value); }
    void handleInt64(const CharSequence& key, int64_t value) { handle(key, value); }
    void handleFloat(const CharSequence& key, float value) { handle(key, value); }
    void handleDouble(const CharSequence& key, double value) { handle(key, value); }
    void handleString(const CharSequence& key, const CharSequence& value, const CharSequence& /*encoding*/)
    {
        if (matches(key)) result = std::string(value.data, value.size);
    }
    qpid::types::Variant getResult() { return result; }

  private:
    std::string name;
    qpid::types::Variant result;

    bool matches(const CharSequence& key)
    {
        return name.size()==key.size &&
            ::strncmp(key.data, name.data(), key.size) == 0;
    }

    template <typename T> void handle(const CharSequence& key, T value)
    {
        if (matches(key)) result = value;
    }
};
}
qpid::types::Variant Message::getProperty(const std::string& key) const
{
    PropertyRetriever r(key);
    sharedState->processProperties(r);
    return r.getResult();
}

boost::intrusive_ptr<PersistableMessage> Message::getPersistentContext() const
{
    return persistentContext;
}

void Message::processProperties(MapHandler& handler) const
{
    sharedState->processProperties(handler);
}

bool Message::hasReplicationId() const {
    return isReplicationIdSet;
}

uint64_t Message::getReplicationId() const {
    return replicationId;
}

void Message::setReplicationId(framing::SequenceNumber id) {
    replicationId = id;
    isReplicationIdSet = true;
}

sys::AbsTime Message::getExpiration() const
{
    return sharedState->getExpiration();
}

Message::SharedStateImpl::SharedStateImpl() : publisher(0), expiration(qpid::sys::FAR_FUTURE), isManagementMessage(false) {}

const Connection* Message::SharedStateImpl::getPublisher() const
{
    return publisher;
}

void Message::SharedStateImpl::setPublisher(const Connection* p)
{
    publisher = p;
}

sys::AbsTime Message::SharedStateImpl::getExpiration() const
{
    return expiration;
}

void Message::SharedStateImpl::setExpiration(sys::AbsTime e)
{
    expiration = e;
}

sys::Duration Message::SharedStateImpl::getTimeToExpiration() const
{
    return sys::Duration(sys::AbsTime::now(), expiration);
}

void Message::SharedStateImpl::computeExpiration()
{
    //TODO: this is still quite 0-10 specific...
    uint64_t ttl;
    if (getTtl(ttl)) {
        // Use higher resolution time for the internal expiry calculation.
        // Prevent overflow as a signed int64_t
        Duration duration(std::min(ttl * TIME_MSEC,
                                   (uint64_t) std::numeric_limits<int64_t>::max()));
        expiration = AbsTime(sys::AbsTime::now(), duration);
    }
}

bool Message::SharedStateImpl::getIsManagementMessage() const
{
    return isManagementMessage;
}
void Message::SharedStateImpl::setIsManagementMessage(bool b)
{
    isManagementMessage = b;
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
}} // namespace qpid::broker
