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
=======
#include "qpid/broker/Protocol.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/framing/AMQP_HighestVersion.h"
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/MessageTransferBody.h"
#include "qpid/framing/FieldValue.h"
#include "qpid/framing/Uuid.h"
<<<<<<< HEAD
=======
#include "MessageUtils.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

#include "unit_test.h"

#include <iostream>

using namespace qpid::broker;
using namespace qpid::framing;

using std::string;

namespace qpid {
namespace tests {

QPID_AUTO_TEST_SUITE(MessageTestSuite)

QPID_AUTO_TEST_CASE(testEncodeDecode)
{
    string exchange = "MyExchange";
    string routingKey = "MyRoutingKey";
<<<<<<< HEAD
    Uuid messageId(true);
    string data1("abcdefg");
    string data2("hijklmn");

    boost::intrusive_ptr<Message> msg(new Message());

    AMQFrame method((MessageTransferBody(ProtocolVersion(), exchange, 0, 0)));
    AMQFrame header((AMQHeaderBody()));
    AMQFrame content1((AMQContentBody(data1)));
    AMQFrame content2((AMQContentBody(data2)));

    msg->getFrames().append(method);
    msg->getFrames().append(header);
    msg->getFrames().append(content1);
    msg->getFrames().append(content2);

    MessageProperties* mProps = msg->getFrames().getHeaders()->get<MessageProperties>(true);
    mProps->setContentLength(data1.size() + data2.size());
    mProps->setMessageId(messageId);
    FieldTable applicationHeaders;
    applicationHeaders.setString("abc", "xyz");
    mProps->setApplicationHeaders(applicationHeaders);
    DeliveryProperties* dProps = msg->getFrames().getHeaders()->get<DeliveryProperties>(true);
    dProps->setRoutingKey(routingKey);
    dProps->setDeliveryMode(PERSISTENT);
    BOOST_CHECK(msg->isPersistent());

    std::vector<char> buff(msg->encodedSize());
    Buffer wbuffer(&buff[0], msg->encodedSize());
    msg->encode(wbuffer);

    Buffer rbuffer(&buff[0], msg->encodedSize());
    msg = new Message();
    msg->decodeHeader(rbuffer);
    msg->decodeContent(rbuffer);
    BOOST_CHECK_EQUAL(exchange, msg->getExchangeName());
    BOOST_CHECK_EQUAL(routingKey, msg->getRoutingKey());
    BOOST_CHECK_EQUAL((uint64_t) data1.size() + data2.size(), msg->contentSize());
    BOOST_CHECK_EQUAL((uint64_t) data1.size() + data2.size(), msg->getProperties<MessageProperties>()->getContentLength());
    BOOST_CHECK_EQUAL(messageId, msg->getProperties<MessageProperties>()->getMessageId());
    BOOST_CHECK_EQUAL(string("xyz"), msg->getProperties<MessageProperties>()->getApplicationHeaders().getAsString("abc"));
    BOOST_CHECK_EQUAL((uint8_t) PERSISTENT, msg->getProperties<DeliveryProperties>()->getDeliveryMode());
    BOOST_CHECK(msg->isPersistent());
=======
    uint64_t ttl(60);
    Uuid messageId(true);
    string data("abcdefghijklmn");

    qpid::types::Variant::Map properties;
    properties["routing-key"] = routingKey;
    properties["ttl"] = ttl;
    properties["durable"] = true;
    properties["message-id"] = qpid::types::Uuid(messageId.data());
    properties["abc"] = "xyz";
    Message msg = MessageUtils::createMessage(properties, data);

    std::vector<char> bytes(msg.getPersistentContext()->encodedSize());
    qpid::framing::Buffer buffer(&bytes[0], bytes.size());
    msg.getPersistentContext()->encode(buffer);
    buffer.reset();
    ProtocolRegistry registry(std::set<std::string>(), 0);
    msg = registry.decode(buffer);

    BOOST_CHECK_EQUAL(routingKey, msg.getRoutingKey());
    BOOST_CHECK_EQUAL((uint64_t) data.size(), msg.getContent().size());
    BOOST_CHECK_EQUAL(data, msg.getContent());
    //BOOST_CHECK_EQUAL(messageId, msg->getProperties<MessageProperties>()->getMessageId());
    BOOST_CHECK_EQUAL(string("xyz"), msg.getPropertyAsString("abc"));
    BOOST_CHECK(msg.isPersistent());
}

QPID_AUTO_TEST_CASE(testMessageProperties)
{
  string data("abcdefghijklmn");

  qpid::types::Variant::Map properties;
  properties["abc"] = "xyz";
  Message msg = MessageUtils::createMessage(properties, data);

  // Regression test that looking up a property doesn't return a prefix
  BOOST_CHECK_EQUAL(msg.getProperty("abcdef").getType(), qpid::types::VAR_VOID);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

QPID_AUTO_TEST_SUITE_END()

}} // namespace qpid::tests
