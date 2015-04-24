/*
 *
 * Copyright (c) 2006 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "qpid/acl/Acl.h"
#include "qpid/acl/AclConnectionCounter.h"
<<<<<<< HEAD
=======
#include "qpid/acl/AclResourceCounter.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/acl/AclData.h"
#include "qpid/acl/AclValidator.h"
#include "qpid/sys/Mutex.h"

#include "qpid/broker/Broker.h"
<<<<<<< HEAD
=======
#include "qpid/broker/Connection.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/Plugin.h"
#include "qpid/Options.h"
#include "qpid/log/Logger.h"
#include "qpid/types/Variant.h"
#include "qmf/org/apache/qpid/acl/ArgsAclLookup.h"
#include "qmf/org/apache/qpid/acl/ArgsAclLookupPublish.h"
#include "qmf/org/apache/qpid/acl/Package.h"
#include "qmf/org/apache/qpid/acl/EventAllow.h"
#include "qmf/org/apache/qpid/acl/EventConnectionDeny.h"
<<<<<<< HEAD
=======
#include "qmf/org/apache/qpid/acl/EventQueueQuotaDeny.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qmf/org/apache/qpid/acl/EventDeny.h"
#include "qmf/org/apache/qpid/acl/EventFileLoaded.h"
#include "qmf/org/apache/qpid/acl/EventFileLoadFailed.h"

#include <map>

#include <boost/shared_ptr.hpp>

using namespace std;
using namespace qpid::acl;
using qpid::broker::Broker;
using namespace qpid::sys;
using qpid::management::ManagementAgent;
using qpid::management::ManagementObject;
using qpid::management::Manageable;
using qpid::management::Args;
namespace _qmf = qmf::org::apache::qpid::acl;

<<<<<<< HEAD
Acl::Acl (AclValues& av, Broker& b): aclValues(av), broker(&b), transferAcl(false), mgmtObject(0),
    connectionCounter(new ConnectionCounter(*this, aclValues.aclMaxConnectPerUser, aclValues.aclMaxConnectPerIp, aclValues.aclMaxConnectTotal))
{

    agent = broker->getManagementAgent();

    if (agent != 0){
        _qmf::Package  packageInit(agent);
        mgmtObject = new _qmf::Acl (agent, this, broker);
=======
Acl::Acl (AclValues& av, Broker& b): aclValues(av), broker(&b), transferAcl(false),
    connectionCounter(new ConnectionCounter(*this, aclValues.aclMaxConnectPerUser, aclValues.aclMaxConnectPerIp, aclValues.aclMaxConnectTotal)),
    resourceCounter(new ResourceCounter(*this, aclValues.aclMaxQueuesPerUser)),userRules(false)
{

    if (aclValues.aclMaxConnectPerUser > AclData::getConnectMaxSpec())
        throw Exception("--connection-limit-per-user switch cannot be larger than " + AclData::getMaxConnectSpecStr());
    if (aclValues.aclMaxConnectPerIp > AclData::getConnectMaxSpec())
        throw Exception("--connection-limit-per-ip switch cannot be larger than " + AclData::getMaxConnectSpecStr());
    if (aclValues.aclMaxConnectTotal > AclData::getConnectMaxSpec())
        throw Exception("--max-connections switch cannot be larger than " + AclData::getMaxConnectSpecStr());
    if (aclValues.aclMaxQueuesPerUser > AclData::getConnectMaxSpec())
        throw Exception("--max-queues-per-user switch cannot be larger than " + AclData::getMaxQueueSpecStr());

    agent = broker->getManagementAgent();

    if (agent != 0) {
        _qmf::Package  packageInit(agent);
        mgmtObject = _qmf::Acl::shared_ptr(new _qmf::Acl (agent, this, broker));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        agent->addObject (mgmtObject);
        mgmtObject->set_maxConnections(aclValues.aclMaxConnectTotal);
        mgmtObject->set_maxConnectionsPerIp(aclValues.aclMaxConnectPerIp);
        mgmtObject->set_maxConnectionsPerUser(aclValues.aclMaxConnectPerUser);
<<<<<<< HEAD
    }
    std::string errorString;
    if (!readAclFile(errorString)){
        if (mgmtObject!=0) mgmtObject->set_enforcingAcl(0);
        throw Exception("Could not read ACL file " + errorString);
=======
        mgmtObject->set_maxQueuesPerUser(aclValues.aclMaxQueuesPerUser);
    }

    if (!aclValues.aclFile.empty()) {
        std::string errorString;
        if (!readAclFile(errorString)){
            if (mgmtObject!=0) mgmtObject->set_enforcingAcl(0);
            throw Exception("Could not read ACL file " + errorString);
        }
    } else {
        loadEmptyAclRuleset();
        QPID_LOG(debug, "ACL loaded empty rule set");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    broker->getConnectionObservers().add(connectionCounter);
    QPID_LOG(info, "ACL Plugin loaded");
    if (mgmtObject!=0) mgmtObject->set_enforcingAcl(1);
}


void Acl::reportConnectLimit(const std::string user, const std::string addr)
{
    if (mgmtObject!=0)
        mgmtObject->inc_connectionDenyCount();

<<<<<<< HEAD
    agent->raiseEvent(_qmf::EventConnectionDeny(user, addr));
=======
    if (agent != 0) {
        agent->raiseEvent(_qmf::EventConnectionDeny(user, addr));
    }
}


void Acl::reportQueueLimit(const std::string user, const std::string queueName)
{
    if (mgmtObject!=0)
        mgmtObject->inc_queueQuotaDenyCount();

    if (agent != 0) {
        agent->raiseEvent(_qmf::EventQueueQuotaDeny(user, queueName));
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


bool Acl::authorise(
    const std::string&               id,
    const Action&                    action,
    const ObjectType&                objType,
    const std::string&               name,
    std::map<Property, std::string>* params)
{
    boost::shared_ptr<AclData> dataLocal;
    {
        Mutex::ScopedLock locker(dataLock);
        dataLocal = data;  //rcu copy
    }

    // add real ACL check here...
    AclResult aclreslt = dataLocal->lookup(id,action,objType,name,params);


    return result(aclreslt, id, action, objType, name);
}

bool Acl::authorise(
    const std::string& id,
    const Action&      action,
    const ObjectType& objType,
    const std::string& ExchangeName,
    const std::string& RoutingKey)
{
    boost::shared_ptr<AclData> dataLocal;
    {
        Mutex::ScopedLock locker(dataLock);
        dataLocal = data;  //rcu copy
    }

    // only use dataLocal here...
    AclResult aclreslt = dataLocal->lookup(id,action,objType,ExchangeName,RoutingKey);

    return result(aclreslt, id, action, objType, ExchangeName);
}


bool Acl::approveConnection(const qpid::broker::Connection& conn)
{
<<<<<<< HEAD
    return connectionCounter->approveConnection(conn);
}


void Acl::setUserId(const qpid::broker::Connection& connection, const std::string& username)
{
    connectionCounter->setUserId(connection, username);
=======
    const std::string& userName(conn.getUserId());
    uint16_t connectionLimit(0);

    boost::shared_ptr<AclData> dataLocal;
    {
        Mutex::ScopedLock locker(dataLock);
        dataLocal = data;  //rcu copy
    }

    (void) dataLocal->getConnQuotaForUser(userName, &connectionLimit);

    return connectionCounter->approveConnection(
        conn,
        userName,
        dataLocal->enforcingConnectionQuotas(),
        connectionLimit,
        dataLocal);
}

bool Acl::approveCreateQueue(const std::string& userId, const std::string& queueName)
{
//    return resourceCounter->approveCreateQueue(userId, queueName);
    uint16_t queueLimit(0);

    boost::shared_ptr<AclData> dataLocal;
    {
        Mutex::ScopedLock locker(dataLock);
        dataLocal = data;  //rcu copy
    }

    (void) dataLocal->getQueueQuotaForUser(userId, &queueLimit);

    return resourceCounter->approveCreateQueue(userId, queueName, dataLocal->enforcingQueueQuotas(), queueLimit);
}


void Acl::recordDestroyQueue(const std::string& queueName)
{
    resourceCounter->recordDestroyQueue(queueName);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


bool Acl::result(
    const AclResult&   aclreslt,
    const std::string& id,
    const Action&      action,
    const ObjectType&  objType,
    const std::string& name)
{
    bool result(false);

    switch (aclreslt)
    {
    case ALLOWLOG:
        QPID_LOG(info, "ACL Allow id:" << id
            << " action:" << AclHelper::getActionStr(action)
            << " ObjectType:" << AclHelper::getObjectTypeStr(objType)
            << " Name:" << name );
<<<<<<< HEAD
        agent->raiseEvent(_qmf::EventAllow(id,  AclHelper::getActionStr(action),
                          AclHelper::getObjectTypeStr(objType),
                          name, types::Variant::Map()));
=======
        if (agent != 0) {
            agent->raiseEvent(_qmf::EventAllow(id,  AclHelper::getActionStr(action),
                              AclHelper::getObjectTypeStr(objType),
                              name, types::Variant::Map()));
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        // FALLTHROUGH
    case ALLOW:
        result = true;
        break;

    case DENYLOG:
        QPID_LOG(info, "ACL Deny id:" << id
            << " action:" << AclHelper::getActionStr(action)
            << " ObjectType:" << AclHelper::getObjectTypeStr(objType)
            << " Name:" << name);
<<<<<<< HEAD
        agent->raiseEvent(_qmf::EventDeny(id, AclHelper::getActionStr(action),
                                          AclHelper::getObjectTypeStr(objType),
                                          name, types::Variant::Map()));
=======
        if (agent != 0) {
            agent->raiseEvent(_qmf::EventDeny(id, AclHelper::getActionStr(action),
                                              AclHelper::getObjectTypeStr(objType),
                                              name, types::Variant::Map()));
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        // FALLTHROUGH
    case DENY:
        if (mgmtObject!=0)
            mgmtObject->inc_aclDenyCount();
        result = false;
        break;

    default:
        assert (false);
    }

    return result;
}

bool Acl::readAclFile(std::string& errorText)
{
    // only set transferAcl = true if a rule implies the use of ACL on transfer, else keep false for performance reasons.
    return readAclFile(aclValues.aclFile, errorText);
}

bool Acl::readAclFile(std::string& aclFile, std::string& errorText) {
    boost::shared_ptr<AclData> d(new AclData);
<<<<<<< HEAD
    AclReader ar;
    if (ar.read(aclFile, d)){
        agent->raiseEvent(_qmf::EventFileLoadFailed("", ar.getError()));
=======
    AclReader ar(aclValues.aclMaxConnectPerUser, aclValues.aclMaxQueuesPerUser);
    if (ar.read(aclFile, d)){
        if (agent != 0) {
            agent->raiseEvent(_qmf::EventFileLoadFailed("", ar.getError()));
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        errorText = ar.getError();
        QPID_LOG(error,ar.getError());
        return false;
    }

    AclValidator validator;
    validator.validate(d);

    {
        Mutex::ScopedLock locker(dataLock);
        data = d;
    }
    transferAcl = data->transferAcl; // any transfer ACL
<<<<<<< HEAD
=======
    userRules = true; // rules in force came from an ACL file
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    if (data->transferAcl){
        QPID_LOG(debug,"ACL: Transfer ACL is Enabled!");
    }

<<<<<<< HEAD
=======
    if (data->enforcingConnectionQuotas()){
        QPID_LOG(debug, "ACL: Connection quotas are Enabled.");
    }

    if (data->enforcingQueueQuotas()){
        QPID_LOG(debug, "ACL: Queue quotas are Enabled.");
    }

    QPID_LOG(debug, "ACL: Default connection mode : "
        << AclHelper::getAclResultStr(d->connectionMode()));

>>>>>>> 3bbfc42... Imported Upstream version 0.32
    data->aclSource = aclFile;
    if (mgmtObject!=0){
        mgmtObject->set_transferAcl(transferAcl?1:0);
        mgmtObject->set_policyFile(aclFile);
<<<<<<< HEAD
        sys::AbsTime now = sys::AbsTime::now();
        int64_t ns = sys::Duration(sys::EPOCH, now);
        mgmtObject->set_lastAclLoad(ns);
        agent->raiseEvent(_qmf::EventFileLoaded(""));
=======
        mgmtObject->set_lastAclLoad(Duration::FromEpoch());
        if (agent != 0) {
            agent->raiseEvent(_qmf::EventFileLoaded(""));
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    return true;
}

<<<<<<< HEAD
=======
//
// loadEmptyAclRuleset()
//
// No ACL file is specified but ACL should run.
// Create a ruleset as if only "ACL ALLOW ALL ALL" was in a file
//
void Acl::loadEmptyAclRuleset() {
    boost::shared_ptr<AclData> d(new AclData);
    d->decisionMode = ALLOW;
    d->aclSource = "";
    d->connectionDecisionMode = ALLOW;
    {
        Mutex::ScopedLock locker(dataLock);
        data = d;
    }
    if (mgmtObject!=0){
        mgmtObject->set_transferAcl(transferAcl?1:0);
        mgmtObject->set_policyFile("");
        mgmtObject->set_lastAclLoad(Duration::FromEpoch());
        if (agent != 0) {
            agent->raiseEvent(_qmf::EventFileLoaded(""));
        }
    }
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32

//
// management lookup function performs general query on acl engine
//
Manageable::status_t Acl::lookup(qpid::management::Args& args, std::string& text)
{
    _qmf::ArgsAclLookup& ioArgs = (_qmf::ArgsAclLookup&) args;
    Manageable::status_t result(STATUS_USER);

   try {
        ObjectType   objType = AclHelper::getObjectType(ioArgs.i_object);
        Action       action  = AclHelper::getAction(    ioArgs.i_action);
        std::map<Property, std::string> propertyMap;
        for (::qpid::types::Variant::Map::const_iterator
             iMapIter  = ioArgs.i_propertyMap.begin();
             iMapIter != ioArgs.i_propertyMap.end();
             iMapIter++)
        {
            Property property = AclHelper::getProperty(iMapIter->first);
            propertyMap.insert(make_pair(property, iMapIter->second));
        }

        boost::shared_ptr<AclData> dataLocal;
        {
            Mutex::ScopedLock locker(dataLock);
            dataLocal = data;  //rcu copy
        }
<<<<<<< HEAD
        AclResult aclResult = dataLocal->lookup(
            ioArgs.i_userId,
            action,
            objType,
            ioArgs.i_objectName,
            &propertyMap);

=======
        AclResult aclResult;
        // CREATE CONNECTION does not use lookup()
        if (action == ACT_CREATE && objType == OBJ_CONNECTION) {
            std::string host = propertyMap[acl::PROP_HOST];
            std::string logString;
            aclResult = dataLocal->isAllowedConnection(
                ioArgs.i_userId,
                host,
                logString);
        } else {
            aclResult = dataLocal->lookup(
                ioArgs.i_userId,
                action,
                objType,
                ioArgs.i_objectName,
                &propertyMap);
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        ioArgs.o_result = AclHelper::getAclResultStr(aclResult);
        result = STATUS_OK;

    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "AclLookup invalid name : " << e.what();
        ioArgs.o_result =  oss.str();
        text = oss.str();
    }

    return result;
}


//
// management lookupPublish function performs fastpath
// PUBLISH EXCHANGE query on acl engine
//
Manageable::status_t Acl::lookupPublish(qpid::management::Args& args, std::string& /*text*/)
{
    _qmf::ArgsAclLookupPublish& ioArgs = (_qmf::ArgsAclLookupPublish&) args;
    boost::shared_ptr<AclData>       dataLocal;
    {
        Mutex::ScopedLock locker(dataLock);
        dataLocal = data;  //rcu copy
    }
    AclResult aclResult = dataLocal->lookup(
        ioArgs.i_userId,
        ACT_PUBLISH,
        OBJ_EXCHANGE,
        ioArgs.i_exchangeName,
        ioArgs.i_routingKey);

    ioArgs.o_result = AclHelper::getAclResultStr(aclResult);

    return STATUS_OK;
}


Acl::~Acl(){
    broker->getConnectionObservers().remove(connectionCounter);
}

<<<<<<< HEAD
ManagementObject* Acl::GetManagementObject(void) const
{
    return (ManagementObject*) mgmtObject;
=======
ManagementObject::shared_ptr Acl::GetManagementObject(void) const
{
    return mgmtObject;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Manageable::status_t Acl::ManagementMethod (uint32_t methodId, Args& args, string& text)
{
    Manageable::status_t status = Manageable::STATUS_UNKNOWN_METHOD;
    QPID_LOG (debug, "ACL: Queue::ManagementMethod [id=" << methodId << "]");

    switch (methodId)
    {
    case _qmf::Acl::METHOD_RELOADACLFILE :
        readAclFile(text);
        if (text.empty())
            status = Manageable::STATUS_OK;
        else
            status = Manageable::STATUS_USER;
        break;

    case _qmf::Acl::METHOD_LOOKUP :
        status = lookup(args, text);
        break;

    case _qmf::Acl::METHOD_LOOKUPPUBLISH :
        status = lookupPublish(args, text);
        break;
    }

    return status;
}
