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
#include "qpid/amqp_0_10/Connection.h"
#include "qpid/log/Statement.h"
<<<<<<< HEAD
#include "qpid/amqp_0_10/exceptions.h"
=======
#include "qpid/framing/reply_exceptions.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/Buffer.h"
#include "qpid/framing/ProtocolInitiation.h"

namespace qpid {
namespace amqp_0_10 {

<<<<<<< HEAD
=======
using framing::InternalErrorException;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
using sys::Mutex;

Connection::Connection(sys::OutputControl& o, const std::string& id, bool _isClient)
    : pushClosed(false), popClosed(false), output(o), identifier(id), initialized(false),
      isClient(_isClient), buffered(0), version(0,10)
{}

void Connection::setInputHandler(std::auto_ptr<sys::ConnectionInputHandler> c) {
    connection = c;
}

size_t  Connection::decode(const char* buffer, size_t size) {
    framing::Buffer in(const_cast<char*>(buffer), size);
    if (isClient && !initialized) {
        //read in protocol header
        framing::ProtocolInitiation pi;
        if (pi.decode(in)) {
            if(!(pi==version))
                throw Exception(QPID_MSG("Unsupported version: " << pi
                                         << " supported version " << version));
            QPID_LOG(trace, "RECV [" << identifier << "]: INIT(" << pi << ")");
<<<<<<< HEAD
        }
        initialized = true;
    }
    framing::AMQFrame frame;
    while(frame.decode(in)) {
=======
            initialized = true;
        }
    }
    framing::AMQFrame frame;
    while(!pushClosed && frame.decode(in)) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        QPID_LOG(trace, "RECV [" << identifier << "]: " << frame);
         connection->received(frame);
    }
    return in.getPosition();
}

bool Connection::canEncode() {
    Mutex::ScopedLock l(frameQueueLock);
    if (!popClosed)  {
        Mutex::ScopedUnlock u(frameQueueLock);
        connection->doOutput();
    }
    return !popClosed && ((!isClient && !initialized) || !frameQueue.empty());
}

bool Connection::isClosed() const {
    Mutex::ScopedLock l(frameQueueLock);
    return pushClosed && popClosed;
}

<<<<<<< HEAD
size_t  Connection::encode(const char* buffer, size_t size) {
=======
size_t  Connection::encode(char* buffer, size_t size) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    {   // Swap frameQueue data into workQueue to avoid holding lock while we encode.
        Mutex::ScopedLock l(frameQueueLock);
        if (popClosed) return 0; // Can't pop any more frames.
        assert(workQueue.empty());
        workQueue.swap(frameQueue);
    }
<<<<<<< HEAD
    framing::Buffer out(const_cast<char*>(buffer), size);
=======
    framing::Buffer out(buffer, size);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (!isClient && !initialized) {
        framing::ProtocolInitiation pi(getVersion());
        pi.encode(out);
        initialized = true;
        QPID_LOG(trace, "SENT [" << identifier << "]: INIT(" << pi << ")");
    }
    size_t frameSize=0;
    size_t encoded=0;
    while (!workQueue.empty() && ((frameSize=workQueue.front().encodedSize()) <= out.available())) {
        workQueue.front().encode(out);
        QPID_LOG(trace, "SENT [" << identifier << "]: " << workQueue.front());
        workQueue.pop_front();
        encoded += frameSize;
        if (workQueue.empty() && out.available() > 0) {
            // try to get more output
            connection->doOutput();
            Mutex::ScopedLock l(frameQueueLock);
            workQueue.swap(frameQueue);  // Need to get any new frames into the work queue
        }
    }
    assert(workQueue.empty() || workQueue.front().encodedSize() <= size);
    if (!workQueue.empty() && workQueue.front().encodedSize() > size)
        throw InternalErrorException(QPID_MSG("Frame too large for buffer."));
    {
        Mutex::ScopedLock l(frameQueueLock);
        buffered -= encoded;
        // Put back any frames we did not encode.
        frameQueue.insert(frameQueue.begin(), workQueue.begin(), workQueue.end());
        workQueue.clear();
        if (frameQueue.empty() && pushClosed)
            popClosed = true;
    }
    return out.getPosition();
}

void Connection::abort() { output.abort(); }
<<<<<<< HEAD
void Connection::activateOutput() { output.activateOutput(); }
void Connection::giveReadCredit(int32_t credit) { output.giveReadCredit(credit); }
=======
void Connection::connectionEstablished() { output.connectionEstablished(); }
void Connection::activateOutput() { output.activateOutput(); }
>>>>>>> 3bbfc42... Imported Upstream version 0.32

void  Connection::close() {
    // No more frames can be pushed onto the queue.
    // Frames aleady on the queue can be popped.
    Mutex::ScopedLock l(frameQueueLock);
    pushClosed = true;
}

void  Connection::closed() {
    connection->closed();
}

<<<<<<< HEAD
void Connection::send(framing::AMQFrame& f) {
=======
void Connection::handle(framing::AMQFrame& f) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    {
        Mutex::ScopedLock l(frameQueueLock);
	if (!pushClosed)
            frameQueue.push_back(f);
        buffered += f.encodedSize();
    }
    activateOutput();
}

framing::ProtocolVersion Connection::getVersion() const {
    return version;
}

<<<<<<< HEAD
void Connection::setVersion(const framing::ProtocolVersion& v)  {
    version = v;
}

size_t Connection::getBuffered() const {
    Mutex::ScopedLock l(frameQueueLock);
    return buffered;
}

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}} // namespace qpid::amqp_0_10
