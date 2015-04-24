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

#include "qpid/broker/Broker.h"
#include "qpid/broker/ExchangeRegistry.h"
#include "qpid/broker/DirectExchange.h"
#include "qpid/broker/FanOutExchange.h"
#include "qpid/broker/HeadersExchange.h"
#include "qpid/broker/TopicExchange.h"
#include "qpid/broker/Link.h"
#include "qpid/management/ManagementDirectExchange.h"
#include "qpid/management/ManagementTopicExchange.h"
#include "qpid/framing/reply_exceptions.h"
<<<<<<< HEAD
=======
#include "qmf/org/apache/qpid/broker/EventExchangeDeclare.h"
#include "qmf/org/apache/qpid/broker/EventExchangeDelete.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

using namespace qpid::broker;
using namespace qpid::sys;
using std::pair;
using std::string;
using qpid::framing::FieldTable;
<<<<<<< HEAD

pair<Exchange::shared_ptr, bool> ExchangeRegistry::declare(const string& name, const string& type){

    return declare(name, type, false, FieldTable());
}

pair<Exchange::shared_ptr, bool> ExchangeRegistry::declare(const string& name, const string& type,
                                                           bool durable, const FieldTable& args){
=======
using qpid::management::ManagementAgent;
namespace _qmf = qmf::org::apache::qpid::broker;

pair<Exchange::shared_ptr, bool> ExchangeRegistry::declare(const string& name, const string& type){

    return declare(name, type, false, false, FieldTable());
}

pair<Exchange::shared_ptr, bool> ExchangeRegistry::declare(
    const string& name, const string& type, bool durable, bool autodelete, const FieldTable& args,
    Exchange::shared_ptr alternate, const string& connectionId, const string& userId)
{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    Exchange::shared_ptr exchange;
    std::pair<Exchange::shared_ptr, bool> result;
    {
        RWlock::ScopedWlock locker(lock);
        ExchangeMap::iterator i =  exchanges.find(name);
        if (i == exchanges.end()) {
            if (type == TopicExchange::typeName){
<<<<<<< HEAD
                exchange = Exchange::shared_ptr(new TopicExchange(name, durable, args, parent, broker));
            }else if(type == DirectExchange::typeName){
                exchange = Exchange::shared_ptr(new DirectExchange(name, durable, args, parent, broker));
            }else if(type == FanOutExchange::typeName){
                exchange = Exchange::shared_ptr(new FanOutExchange(name, durable, args, parent, broker));
            }else if (type == HeadersExchange::typeName) {
                exchange = Exchange::shared_ptr(new HeadersExchange(name, durable, args, parent, broker));
=======
                exchange = Exchange::shared_ptr(new TopicExchange(name, durable, autodelete, args, parent, broker));
            }else if(type == DirectExchange::typeName){
                exchange = Exchange::shared_ptr(new DirectExchange(name, durable, autodelete, args, parent, broker));
            }else if(type == FanOutExchange::typeName){
                exchange = Exchange::shared_ptr(new FanOutExchange(name, durable, autodelete, args, parent, broker));
            }else if (type == HeadersExchange::typeName) {
                exchange = Exchange::shared_ptr(new HeadersExchange(name, durable, autodelete, args, parent, broker));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            }else if (type == ManagementDirectExchange::typeName) {
                exchange = Exchange::shared_ptr(new ManagementDirectExchange(name, durable, args, parent, broker));
            }else if (type == ManagementTopicExchange::typeName) {
                exchange = Exchange::shared_ptr(new ManagementTopicExchange(name, durable, args, parent, broker));
            }else if (type == Link::exchangeTypeName) {
                exchange = Link::linkExchangeFactory(name);
            }else{
                FunctionMap::iterator i =  factory.find(type);
                if (i == factory.end()) {
<<<<<<< HEAD
                    throw UnknownExchangeTypeException();
                } else {
                    exchange = i->second(name, durable, args, parent, broker);
=======
                    throw UnknownExchangeTypeException(type);
                } else {
                    exchange = i->second(name, durable, autodelete, args, parent, broker);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                }
            }
            exchanges[name] = exchange;
            result = std::pair<Exchange::shared_ptr, bool>(exchange, true);
<<<<<<< HEAD
        } else {
            result = std::pair<Exchange::shared_ptr, bool>(i->second, false);
        }
    }
    if (broker && exchange) broker->getConfigurationObservers().exchangeCreate(exchange);
    return result;
}

void ExchangeRegistry::destroy(const string& name){
=======
            if (alternate) exchange->setAlternate(alternate);
            // Call exchangeCreate inside the lock to ensure correct ordering.
            if (broker) broker->getBrokerObservers().exchangeCreate(exchange);
        } else {
            result = std::pair<Exchange::shared_ptr, bool>(i->second, false);
        }
        if (broker && broker->getManagementAgent()) {
            // Call raiseEvent inside the lock to ensure correct ordering.
            broker->getManagementAgent()->raiseEvent(
                _qmf::EventExchangeDeclare(
                    connectionId,
                    userId,
                    name,
                    type,
                    alternate ? alternate->getName() : string(),
                    durable,
                    false,
                    ManagementAgent::toMap(result.first->getArgs()),
                    result.second ? "created" : "existing"));
        }
    }
    return result;
}

void ExchangeRegistry::destroy(
    const string& name, const string& connectionId, const string& userId)
{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (name.empty() ||
        (name.find("amq.") == 0 &&
         (name == "amq.direct" || name == "amq.fanout" || name == "amq.topic" || name == "amq.match")) ||
        name == "qpid.management")
        throw framing::NotAllowedException(QPID_MSG("Cannot delete default exchange: '" << name << "'"));
<<<<<<< HEAD
    Exchange::shared_ptr exchange;
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    {
        RWlock::ScopedWlock locker(lock);
        ExchangeMap::iterator i =  exchanges.find(name);
        if (i != exchanges.end()) {
<<<<<<< HEAD
            exchange = i->second;
            i->second->destroy();
            exchanges.erase(i);
        }
    }
    if (broker && exchange) broker->getConfigurationObservers().exchangeDestroy(exchange);
=======
            if (broker) {
                // Call exchangeDestroy and raiseEvent inside the lock to ensure
                // correct ordering.
                broker->getBrokerObservers().exchangeDestroy(i->second);
                if (broker->getManagementAgent())
                    broker->getManagementAgent()->raiseEvent(
                        _qmf::EventExchangeDelete(connectionId, userId, name));
            }
            i->second->destroy();
            exchanges.erase(i);

        }
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Exchange::shared_ptr ExchangeRegistry::find(const string& name){
    RWlock::ScopedRlock locker(lock);
    ExchangeMap::iterator i =  exchanges.find(name);
    if (i == exchanges.end())
        return Exchange::shared_ptr();
    else
        return i->second;
}

Exchange::shared_ptr ExchangeRegistry::get(const string& name) {
    Exchange::shared_ptr ex = find(name);
    if (!ex) throw framing::NotFoundException(QPID_MSG("Exchange not found: "<<name));
    return ex;
}

bool ExchangeRegistry::registerExchange(const Exchange::shared_ptr& ex) {
<<<<<<< HEAD
=======
    RWlock::ScopedWlock locker(lock);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    return exchanges.insert(ExchangeMap::value_type(ex->getName(), ex)).second;
}

void ExchangeRegistry::registerType(const std::string& type, FactoryFunction f)
{
    factory[type] = f;
}

<<<<<<< HEAD

namespace
{
const std::string empty;
=======
void ExchangeRegistry::checkType(const std::string& type)
{
    if (type != TopicExchange::typeName && type != DirectExchange::typeName && type != FanOutExchange::typeName
        && type != HeadersExchange::typeName && type != ManagementDirectExchange::typeName
        && type != ManagementTopicExchange::typeName && type != Link::exchangeTypeName
        && factory.find(type) == factory.end()) {
        throw UnknownExchangeTypeException(type);
    }
}


namespace
{
const std::string EMPTY;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Exchange::shared_ptr ExchangeRegistry::getDefault()
{
<<<<<<< HEAD
    return get(empty);
=======
    return get(EMPTY);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}
