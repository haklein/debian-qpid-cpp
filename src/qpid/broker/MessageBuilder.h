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
#ifndef _MessageBuilder_
#define _MessageBuilder_

#include "qpid/broker/BrokerImportExport.h"
#include "qpid/framing/FrameHandler.h"
#include "qpid/framing/SequenceNumber.h"
#include "qpid/RefCounted.h"

<<<<<<< HEAD
=======
#include <string>

>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <boost/intrusive_ptr.hpp>

namespace qpid {
    namespace broker {
<<<<<<< HEAD
        class Message;
        class MessageStore;

        class QPID_BROKER_CLASS_EXTERN MessageBuilder : public framing::FrameHandler{
        public:
            QPID_BROKER_EXTERN MessageBuilder(MessageStore* const store);
            QPID_BROKER_EXTERN void handle(framing::AMQFrame& frame);
            boost::intrusive_ptr<Message> getMessage() { return message; }
=======
        namespace amqp_0_10 {
        class MessageTransfer;
        }

        class QPID_BROKER_CLASS_EXTERN MessageBuilder : public framing::FrameHandler{
        public:
            QPID_BROKER_EXTERN MessageBuilder();
            QPID_BROKER_EXTERN void handle(framing::AMQFrame& frame);
            boost::intrusive_ptr<qpid::broker::amqp_0_10::MessageTransfer> getMessage();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            QPID_BROKER_EXTERN void start(const framing::SequenceNumber& id);
            void end();
        private:
            enum State {DORMANT, METHOD, HEADER, CONTENT};
            State state;
<<<<<<< HEAD
            boost::intrusive_ptr<Message> message;
            MessageStore* const store;
=======
            boost::intrusive_ptr<qpid::broker::amqp_0_10::MessageTransfer> message;
            std::string exchange;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

            void checkType(uint8_t expected, uint8_t actual);
        };
    }
}


#endif

