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
#include "ReplicationTest.h"
<<<<<<< HEAD
#include "qpid/broker/Queue.h"
=======
#include "qpid/log/Statement.h"
#include "qpid/broker/Queue.h"
#include "qpid/broker/Exchange.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/framing/FieldTable.h"

namespace qpid {
namespace ha {

using types::Variant;

<<<<<<< HEAD
ReplicateLevel ReplicationTest::replicateLevel(const std::string& str) {
=======
ReplicateLevel ReplicationTest::getLevel(const std::string& str) const {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    Enum<ReplicateLevel> rl(replicateDefault);
    if (!str.empty()) rl.parse(str);
    return rl.get();
}

<<<<<<< HEAD
ReplicateLevel ReplicationTest::replicateLevel(const framing::FieldTable& f) {
    if (f.isSet(QPID_REPLICATE))
        return replicateLevel(f.getAsString(QPID_REPLICATE));
=======
ReplicateLevel ReplicationTest::getLevel(const framing::FieldTable& f) const {
    if (f.isSet(QPID_REPLICATE))
        return getLevel(f.getAsString(QPID_REPLICATE));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    else
        return replicateDefault;
}

<<<<<<< HEAD
ReplicateLevel ReplicationTest::replicateLevel(const Variant::Map& m) {
    Variant::Map::const_iterator i = m.find(QPID_REPLICATE);
    if (i != m.end())
        return replicateLevel(i->second.asString());
=======
ReplicateLevel ReplicationTest::getLevel(const Variant::Map& m) const {
    Variant::Map::const_iterator i = m.find(QPID_REPLICATE);
    if (i != m.end())
        return getLevel(i->second.asString());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    else
        return replicateDefault;
}

<<<<<<< HEAD
namespace {
const std::string AUTO_DELETE_TIMEOUT("qpid.auto_delete_timeout");
}

bool ReplicationTest::isReplicated(
    ReplicateLevel level, const Variant::Map& args, bool autodelete, bool exclusive)
{
    bool ignore = autodelete && exclusive && args.find(AUTO_DELETE_TIMEOUT) == args.end();
    return !ignore && replicateLevel(args) >= level;
}

bool ReplicationTest::isReplicated(
    ReplicateLevel level, const framing::FieldTable& args, bool autodelete, bool exclusive)
{
    bool ignore = autodelete && exclusive && !args.isSet(AUTO_DELETE_TIMEOUT);
    return !ignore && replicateLevel(args) >= level;
}

bool ReplicationTest::isReplicated(ReplicateLevel level, const broker::Queue& q)
{
    return isReplicated(level, q.getSettings(), q.isAutoDelete(), q.hasExclusiveOwner());
=======
ReplicateLevel ReplicationTest::getLevel(const broker::Queue& q) const {
    const Variant::Map& qmap(q.getSettings().original);
    Variant::Map::const_iterator i = qmap.find(QPID_REPLICATE);
    if (i != qmap.end())
        return getLevel(i->second.asString());
    else
        return getLevel(q.getSettings().storeSettings);
}

ReplicateLevel ReplicationTest::getLevel(const broker::Exchange& ex) const {
    return getLevel(ex.getArgs());
}

ReplicateLevel ReplicationTest::useLevel(const broker::Queue& q) const {
    return q.getSettings().isTemporary ? ReplicationTest(NONE).getLevel(q) : getLevel(q);
}

ReplicateLevel ReplicationTest::useLevel(const broker::Exchange& ex) const {
    return ReplicationTest::getLevel(ex);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


}} // namespace qpid::ha
