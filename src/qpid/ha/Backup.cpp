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
#include "Backup.h"
#include "BrokerReplicator.h"
<<<<<<< HEAD
#include "HaBroker.h"
#include "ReplicatingSubscription.h"
#include "Settings.h"
=======
#include "ConnectionObserver.h"
#include "HaBroker.h"
#include "Primary.h"
#include "ReplicatingSubscription.h"
#include "Settings.h"
#include "StatusCheck.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/Url.h"
#include "qpid/amqp_0_10/Codecs.h"
#include "qpid/broker/Bridge.h"
#include "qpid/broker/Broker.h"
<<<<<<< HEAD
#include "qpid/broker/SessionHandler.h"
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/broker/Link.h"
#include "qpid/framing/AMQP_ServerProxy.h"
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/MessageTransferBody.h"
#include "qpid/sys/SystemInfo.h"
#include "qpid/types/Variant.h"
<<<<<<< HEAD
=======
#include "qpid/log/Statement.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

namespace qpid {
namespace ha {

using namespace framing;
using namespace broker;
using types::Variant;
using std::string;
<<<<<<< HEAD

Backup::Backup(HaBroker& hb, const Settings& s) :
    logPrefix("Backup: "), haBroker(hb), broker(hb.getBroker()), settings(s)
{
    // Empty brokerUrl means delay initialization until seBrokertUrl() is called.
    if (!s.brokerUrl.empty()) initialize(Url(s.brokerUrl));
}

void Backup::initialize(const Url& brokers) {
    if (brokers.empty()) throw Url::Invalid("HA broker URL is empty");
    QPID_LOG(info, logPrefix << "Connecting to cluster, broker URL: " << brokers);
    string protocol = brokers[0].protocol.empty() ? "tcp" : brokers[0].protocol;
    types::Uuid uuid(true);
    // Declare the link
    std::pair<Link::shared_ptr, bool> result = broker.getLinks().declare(
        broker::QPID_NAME_PREFIX + string("ha.link.") + uuid.str(),
        brokers[0].host, brokers[0].port, protocol,
        false,                  // durable
        settings.mechanism, settings.username, settings.password,
        false);               // no amq.failover - don't want to use client URL.
    {
        sys::Mutex::ScopedLock l(lock);
        link = result.first;
        replicator.reset(new BrokerReplicator(haBroker, link));
        replicator->initialize();
        broker.getExchanges().registerExchange(replicator);
    }
    link->setUrl(brokers);          // Outside the lock, once set link doesn't change.
}

Backup::~Backup() {
    if (link) link->close();
    if (replicator.get()) broker.getExchanges().destroy(replicator->getName());
}

// Called via management.
void Backup::setBrokerUrl(const Url& url) {
    // Ignore empty URLs seen during start-up for some tests.
    if (url.empty()) return;
    bool linkSet = false;
    {
        sys::Mutex::ScopedLock l(lock);
        linkSet = link;
    }
    if (linkSet)
        link->setUrl(url);      // Outside lock, once set link doesn't change
    else
        initialize(url);        // Deferred initialization
}

void Backup::setStatus(BrokerStatus status) {
    switch (status) {
      case READY:
        QPID_LOG(notice, logPrefix << "Ready to become primary.");
        break;
      case CATCHUP:
        QPID_LOG(notice, logPrefix << "Catching up on primary, cannot be promoted.");
      default:
        assert(0);
    }
=======
using sys::Mutex;

Backup::Backup(HaBroker& hb, const Settings& s) :
    logPrefix(hb.logPrefix), membership(hb.getMembership()), stopped(false),
    haBroker(hb), broker(hb.getBroker()), settings(s),
    statusCheck(new StatusCheck(hb))
{}

void Backup::setBrokerUrl(const Url& brokers) {
    if (brokers.empty()) return;
    Mutex::ScopedLock l(lock);
    if (stopped) return;
    if (haBroker.getStatus() == JOINING) statusCheck->setUrl(brokers);
    if (!link) {                // Not yet initialized
        QPID_LOG(info, logPrefix << "Connecting to cluster: " << brokers);
        string protocol = brokers[0].protocol.empty() ? "tcp" : brokers[0].protocol;
        types::Uuid uuid(true);
        link = broker.getLinks().declare(
            broker::QPID_NAME_PREFIX + string("ha.link.") + uuid.str(),
            brokers[0].host, brokers[0].port, protocol,
            false,                  // durable
            settings.mechanism, settings.username, settings.password,
            false).first;     // no amq.failover - don't want to use client URL.
        replicator = BrokerReplicator::create(haBroker, link);
        broker.getExchanges().registerExchange(replicator);
    }
    link->setUrl(brokers);
}

void Backup::stop(Mutex::ScopedLock&) {
    if (stopped) return;
    stopped = true;
    if (link) link->close();
    if (replicator.get()) {
        replicator->shutdown();
        replicator.reset();
    }
}

Role* Backup::recover(Mutex::ScopedLock&) {
    BrokerInfo::Set backups;
    {
        Mutex::ScopedLock l(lock);
        if (stopped) return 0;
        stop(l);                 // Stop backup activity before starting primary.
        // Reset membership before allowing backups to connect.
        backups = membership.otherBackups();
        membership.clear();
    }
    return new Primary(haBroker, backups);
}

Role* Backup::promote() {
    Mutex::ScopedLock l(lock);
    if (stopped) return 0;
    switch (haBroker.getStatus()) {
      case JOINING:
        if (statusCheck->canPromote()) return recover(l);
        else {
            QPID_LOG(error, logPrefix << "Joining active cluster, cannot be promoted.");
            throw Exception("Joining active cluster, cannot be promoted.");
        }
        break;
      case CATCHUP:
        QPID_LOG(error, logPrefix << "Still catching up, cannot be promoted.");
        throw Exception("Still catching up, cannot be promoted.");
        break;
      case READY: return recover(l); break;
      default:
        assert(0);              // Not a valid state for the Backup role..
    }
    return 0; 			// Keep compiler happy
}

Backup::~Backup() {
    Mutex::ScopedLock l(lock);
    stop(l);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

}} // namespace qpid::ha
