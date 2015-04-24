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

#include "qpid/Exception.h"
#include "qpid/broker/HeadersExchange.h"
<<<<<<< HEAD
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/FieldValue.h"
=======
#include "qpid/broker/Message.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/FieldValue.h"
#include "MessageUtils.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "unit_test.h"

using namespace qpid::broker;
using namespace qpid::framing;
<<<<<<< HEAD
=======
using namespace qpid::types;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

namespace qpid {
namespace tests {

QPID_AUTO_TEST_SUITE(HeadersExchangeTestSuite)

QPID_AUTO_TEST_CASE(testMatchAll)
{
<<<<<<< HEAD
    FieldTable b, m, n;
    b.setString("x-match", "all");
    b.setString("foo", "FOO");
    b.setInt("n", 42);
    m.setString("foo", "FOO");
    m.setInt("n", 42);
    BOOST_CHECK(HeadersExchange::match(b, m));

    // Ignore extras.
    m.setString("extra", "x");
    BOOST_CHECK(HeadersExchange::match(b, m));

    // Fail mismatch, wrong value.
    m.setString("foo", "NotFoo");
    BOOST_CHECK(!HeadersExchange::match(b, m));

    // Fail mismatch, missing value
    n.setInt("n", 42);
    n.setString("extra", "x");
    BOOST_CHECK(!HeadersExchange::match(b, n));
=======
    FieldTable b;
    b.setString("x-match", "all");
    b.setString("foo", "FOO");
    b.setInt("n", 42);

    Variant::Map m;
    const int32_t int_n(42);
    m["foo"] = "FOO";
    m["n"] = int_n;
    BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));

    // Ignore extras.
    m["extra"] = "x";
    BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));

    // Fail mismatch, wrong value.
    m["foo"] = "NotFoo";
    BOOST_CHECK(!HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));

    // Fail mismatch, missing value
    Variant::Map n;
    n["n"] = int_n;
    n["extra"] = "x";
    BOOST_CHECK(!HeadersExchange::match(b, MessageUtils::createMessage(n, "", "", true)));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

QPID_AUTO_TEST_CASE(testMatchAny)
{
<<<<<<< HEAD
    FieldTable b, m, n;
    b.setString("x-match", "any");
    b.setString("foo", "FOO");
    b.setInt("n", 42);
    m.setString("foo", "FOO");
    BOOST_CHECK(!HeadersExchange::match(b, n));
    BOOST_CHECK(HeadersExchange::match(b, m));
    m.setInt("n", 42);
    BOOST_CHECK(HeadersExchange::match(b, m));
=======
    FieldTable b;
    b.setString("x-match", "any");
    b.setString("foo", "FOO");
    b.setInt("n", 42);

    Variant::Map n;
    Variant::Map m;
    m["foo"] = "FOO";
    BOOST_CHECK(!HeadersExchange::match(b, MessageUtils::createMessage(n, "", "", true)));
    BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));
    const int32_t int_n(42);
    m["n"] = int_n;
    BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

QPID_AUTO_TEST_CASE(testMatchEmptyValue)
{
<<<<<<< HEAD
    FieldTable b, m;
    b.setString("x-match", "all");
    b.set("foo", FieldTable::ValuePtr());
    b.set("n", FieldTable::ValuePtr());
    BOOST_CHECK(!HeadersExchange::match(b, m));
    m.setString("foo", "blah");
    m.setInt("n", 123);
=======
    FieldTable b;
    b.setString("x-match", "all");
    b.set("foo", FieldTable::ValuePtr());
    b.set("n", FieldTable::ValuePtr());
    Variant::Map m;
    BOOST_CHECK(!HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

QPID_AUTO_TEST_CASE(testMatchEmptyArgs)
{
<<<<<<< HEAD
    FieldTable b, m;
    m.setString("foo", "FOO");

    b.setString("x-match", "all");
    BOOST_CHECK(HeadersExchange::match(b, m));
    b.setString("x-match", "any");
    BOOST_CHECK(!HeadersExchange::match(b, m));
=======
    FieldTable b;
    Variant::Map m;
    m["foo"] = "FOO";
    Message msg = MessageUtils::createMessage(m, "", "", true);

    b.setString("x-match", "all");
    BOOST_CHECK(HeadersExchange::match(b, msg));
    b.setString("x-match", "any");
    BOOST_CHECK(!HeadersExchange::match(b, msg));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


QPID_AUTO_TEST_CASE(testMatchNoXMatch)
{
<<<<<<< HEAD
    FieldTable b, m;
    b.setString("foo", "FOO");
    m.setString("foo", "FOO");
    BOOST_CHECK(!HeadersExchange::match(b, m));
=======
    FieldTable b;
    b.setString("foo", "FOO");
    Variant::Map m;
    m["foo"] = "FOO";
    BOOST_CHECK(!HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

QPID_AUTO_TEST_CASE(testBindNoXMatch)
{
    HeadersExchange exchange("test");
    Queue::shared_ptr queue;
    std::string key;
    FieldTable args;
    try {
        //just checking this doesn't cause assertion etc
        exchange.bind(queue, key, &args);
    } catch(qpid::Exception&) {
        //expected
    }
}

<<<<<<< HEAD
=======

QPID_AUTO_TEST_CASE(testMatchSizedIntUint)
{
    typedef std::list<Variant::Map> vml;

    const int8_t i8(1);
    const int16_t i16(1);
    const int32_t i32(1);
    const int64_t i64(1);
    const uint8_t u8(1);
    const uint16_t u16(1);
    const uint32_t u32(1);
    const uint64_t u64(1);

    Variant::Map mi8, mi16, mi32, mi64;
    Variant::Map mu8, mu16, mu32, mu64;

    mi8["bk"] = i8;
    mi16["bk"] = i16;
    mi32["bk"] = i32;
    mi64["bk"] = i64;
    mu8["bk"] = u8;
    mu16["bk"] = u16;
    mu32["bk"] = u32;
    mu64["bk"] = u64;

    vml mMap;
    mMap.push_back(mi8);
    mMap.push_back(mi16);
    mMap.push_back(mi32);
    mMap.push_back(mi64);
    mMap.push_back(mu8);
    mMap.push_back(mu16);
    mMap.push_back(mu32);
    mMap.push_back(mu64);

    for (vml::iterator bVal=mMap.begin(); bVal!=mMap.end(); ++bVal) {
        FieldTable b;
        qpid::amqp_0_10::translate(*bVal, b);
        b.setString("x-match", "all");
        for (vml::iterator mVal=mMap.begin(); mVal!=mMap.end(); ++mVal) {
            BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(*mVal, "", "", true)));
        }
    }
}

// TODO: Headers exchange match on single

QPID_AUTO_TEST_CASE(testMatchFloatDouble)
{
    const double iFloat(1.0);
    Variant::Map m;
    m["bk"] = iFloat;

    FieldTable b;
    qpid::amqp_0_10::translate(m, b);
    b.setString("x-match", "all");
    BOOST_CHECK(HeadersExchange::match(b, MessageUtils::createMessage(m, "", "", true)));
}


>>>>>>> 3bbfc42... Imported Upstream version 0.32
QPID_AUTO_TEST_SUITE_END()

}} // namespace qpid::tests
