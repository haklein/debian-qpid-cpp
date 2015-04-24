//
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//

#include "qpid/broker/Vhost.h"
#include "qpid/broker/Broker.h"
#include "qpid/management/ManagementAgent.h"

using namespace qpid::broker;
using qpid::management::ManagementAgent;
namespace _qmf = qmf::org::apache::qpid::broker;

namespace qpid { namespace management {
class Manageable;
}}

<<<<<<< HEAD
Vhost::Vhost (qpid::management::Manageable* parentBroker, Broker* broker) : mgmtObject(0)
=======
Vhost::Vhost (qpid::management::Manageable* parentBroker, Broker* broker)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    if (parentBroker != 0 && broker != 0)
    {
        ManagementAgent* agent = broker->getManagementAgent();

        if (agent != 0)
        {
<<<<<<< HEAD
            mgmtObject = new _qmf::Vhost(agent, this, parentBroker, "/");
=======
            mgmtObject = _qmf::Vhost::shared_ptr(new _qmf::Vhost(agent, this, parentBroker, "/"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            agent->addObject(mgmtObject, 0, true);
        }
    }
}

<<<<<<< HEAD
=======
Vhost::~Vhost () {
    if (mgmtObject != 0)
        mgmtObject->debugStats("destroying");
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
void Vhost::setFederationTag(const std::string& tag)
{
    mgmtObject->set_federationTag(tag);
}
