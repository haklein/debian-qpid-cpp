#ifndef QPID_HA_BROKER_H
#define QPID_HA_BROKER_H

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

#include "BrokerInfo.h"
#include "Membership.h"
#include "types.h"
<<<<<<< HEAD
#include "ReplicationTest.h"
#include "Settings.h"
#include "qpid/Url.h"
=======
#include "LogPrefix.h"
#include "Settings.h"
#include "qpid/Url.h"
#include "FailoverExchange.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/sys/Mutex.h"
#include "qmf/org/apache/qpid/ha/HaBroker.h"
#include "qpid/management/Manageable.h"
#include "qpid/types/Variant.h"
<<<<<<< HEAD
#include <memory>
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <set>
#include <boost/shared_ptr.hpp>

namespace qpid {

namespace types {
class Variant;
}

namespace broker {
class Broker;
class Queue;
}
namespace framing {
class FieldTable;
}

namespace ha {
class Backup;
class ConnectionObserver;
class Primary;
<<<<<<< HEAD
=======
class Role;
class QueueReplicator;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

/**
 * HA state and actions associated with a HA broker. Holds all the management info.
 *
 * THREAD SAFE: may be called in arbitrary broker IO or timer threads.
<<<<<<< HEAD
=======

 * NOTE: HaBroker and Role subclasses follow this lock hierarchy:
 * - HaBroker MUST NOT hold its own lock across calls Role subclasses.
 * - Role subclasses MAY hold their locks accross calls to HaBroker.
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 */
class HaBroker : public management::Manageable
{
  public:
    /** HaBroker is constructed during earlyInitialize */
    HaBroker(broker::Broker&, const Settings&);
    ~HaBroker();

    /** Called during plugin initialization */
    void initialize();

    // Implement Manageable.
<<<<<<< HEAD
    qpid::management::ManagementObject* GetManagementObject() const { return mgmtObject; }
=======
    qpid::management::ManagementObject::shared_ptr GetManagementObject() const { return mgmtObject; }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    management::Manageable::status_t ManagementMethod (
        uint32_t methodId, management::Args& args, std::string& text);

    broker::Broker& getBroker() { return broker; }
    const Settings& getSettings() const { return settings; }
<<<<<<< HEAD

    /** Shut down the broker. Caller should log a critical error message. */
    void shutdown();

    BrokerStatus getStatus() const;
    void setStatus(BrokerStatus);
    void activate();

    Backup* getBackup() { return backup.get(); }
    ReplicationTest getReplicationTest() const { return replicationTest; }

    boost::shared_ptr<ConnectionObserver> getObserver() { return observer; }

    const BrokerInfo& getBrokerInfo() const { return brokerInfo; }

    void setMembership(const types::Variant::List&); // Set membership from list.
    void resetMembership(const BrokerInfo& b); // Reset to contain just one member.
    void addBroker(const BrokerInfo& b);       // Add a broker to the membership.
    void removeBroker(const types::Uuid& id);  // Remove a broker from membership.

    types::Uuid getSystemId() const { return systemId; }

  private:
    void setClientUrl(const Url&);
    void setBrokerUrl(const Url&);
    void updateClientUrl(sys::Mutex::ScopedLock&);

    bool isPrimary(sys::Mutex::ScopedLock&) { return !backup.get(); }

    void setStatus(BrokerStatus, sys::Mutex::ScopedLock&);
    void recover();
    void statusChanged(sys::Mutex::ScopedLock&);
    void setLinkProperties(sys::Mutex::ScopedLock&);

    std::vector<Url> getKnownBrokers() const;

    void membershipUpdated(sys::Mutex::ScopedLock&);

    std::string logPrefix;
    broker::Broker& broker;
    types::Uuid systemId;
    const Settings settings;

    mutable sys::Mutex lock;
    boost::shared_ptr<ConnectionObserver> observer; // Used by Backup and Primary
    std::auto_ptr<Backup> backup;
    std::auto_ptr<Primary> primary;
    qmf::org::apache::qpid::ha::HaBroker* mgmtObject;
    Url clientUrl, brokerUrl;
    std::vector<Url> knownBrokers;
    BrokerStatus status;
    BrokerInfo brokerInfo;
    Membership membership;
    ReplicationTest replicationTest;
=======
    boost::shared_ptr<Role> getRole() const {return role; }

    /** Shut down the broker because of a critical error. */
    void shutdown(const std::string& message);

    BrokerStatus getStatus() const;
    boost::shared_ptr<ConnectionObserver> getObserver() { return observer; }

    BrokerInfo getBrokerInfo() const { return membership.getSelf(); }
    Membership& getMembership() { return membership; }
    types::Uuid getSystemId() const { return systemId; }

    void setAddress(const Address&); // set self address from a self-connection

    boost::shared_ptr<QueueReplicator> findQueueReplicator(const std::string& queueName);

    /** Authenticated user ID for queue create/delete */
    std::string getUserId() const { return userId; }

    /** logPrefix is thread safe and used by other classes (Membership) */
    LogPrefix logPrefix;

  private:
    class BrokerObserver;

    void setPublicUrl(const Url&);
    void setBrokerUrl(const Url&);
    void updateClientUrl(sys::Mutex::ScopedLock&);

    std::vector<Url> getKnownBrokers() const;

    // Immutable members
    const types::Uuid systemId;
    const Settings settings;
    const std::string userId;

    // Member variables protected by lock
    mutable sys::Mutex lock;
    Url publicUrl, brokerUrl;
    std::vector<Url> knownBrokers;

    // Independently thread-safe member variables
    broker::Broker& broker;
    qmf::org::apache::qpid::ha::HaBroker::shared_ptr mgmtObject;
    boost::shared_ptr<ConnectionObserver> observer; // Used by Backup and Primary
    boost::shared_ptr<Role> role;
    Membership membership;
    boost::shared_ptr<FailoverExchange> failoverExchange;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};
}} // namespace qpid::ha

#endif  /*!QPID_HA_BROKER_H*/
