#ifndef QPID_HA_BACKUP_H
#define QPID_HA_BACKUP_H

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
=======
#include "LogPrefix.h"
#include "Role.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "Settings.h"
#include "qpid/Url.h"
#include "qpid/sys/Mutex.h"
#include <boost/shared_ptr.hpp>

namespace qpid {

namespace broker {
class Broker;
class Link;
}

namespace ha {
class Settings;
class BrokerReplicator;
class HaBroker;
<<<<<<< HEAD

/**
 * State associated with a backup broker. Manages connections to primary.
 *
 * THREAD SAFE
 */
class Backup
=======
class StatusCheck;
class Membership;

/**
 * Backup role: Manages connections to primary, replicates  management events and queue contents.
 *
 * THREAD SAFE
 */
class Backup : public Role
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
  public:
    Backup(HaBroker&, const Settings&);
    ~Backup();
<<<<<<< HEAD
    void setBrokerUrl(const Url&);
    void setStatus(BrokerStatus);

  private:
    void initialize(const Url&);
    std::string logPrefix;

    sys::Mutex lock;
=======

    void setBrokerUrl(const Url&);

    Role* promote();

    boost::shared_ptr<BrokerReplicator> getBrokerReplicator() { return replicator; }

  private:
    void stop(sys::Mutex::ScopedLock&);
    Role* recover(sys::Mutex::ScopedLock&);

    const LogPrefix& logPrefix;
    Membership& membership;

    sys::Mutex lock;
    bool stopped;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    HaBroker& haBroker;
    broker::Broker& broker;
    Settings settings;
    boost::shared_ptr<broker::Link> link;
    boost::shared_ptr<BrokerReplicator> replicator;
<<<<<<< HEAD
=======
    std::auto_ptr<StatusCheck> statusCheck;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}} // namespace qpid::ha

#endif  /*!QPID_HA_BACKUP_H*/
