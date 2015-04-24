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
#include "qpid/broker/LinkRegistry.h"
<<<<<<< HEAD
#include "qpid/broker/Link.h"
#include "qpid/broker/Connection.h"
=======

#include "qpid/broker/Broker.h"
#include "qpid/broker/amqp_0_10/Connection.h"
#include "qpid/broker/Link.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/log/Statement.h"
#include <iostream>
#include <boost/format.hpp>

namespace qpid {
namespace broker {

using namespace qpid::sys;
using std::string;
using std::pair;
using std::stringstream;
using boost::intrusive_ptr;
using boost::format;
using boost::str;
namespace _qmf = qmf::org::apache::qpid::broker;

// TODO: This constructor is only used by the store unit tests -
// That probably indicates that LinkRegistry isn't correctly
// factored: The persistence element should be factored separately
LinkRegistry::LinkRegistry () :
    broker(0),
<<<<<<< HEAD
    parent(0), store(0), passive(false),
=======
    parent(0), store(0),
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    realm("")
{
}

class LinkRegistryConnectionObserver : public ConnectionObserver {
    LinkRegistry& links;
  public:
    LinkRegistryConnectionObserver(LinkRegistry& l) : links(l) {}
<<<<<<< HEAD
    void connection(Connection& c) { links.notifyConnection(c.getMgmtId(), &c); }
    void opened(Connection& c) { links.notifyOpened(c.getMgmtId()); }
    void closed(Connection& c) { links.notifyClosed(c.getMgmtId()); }
    void forced(Connection& c, const string& text) { links.notifyConnectionForced(c.getMgmtId(), text); }
=======
    void connection(Connection& in)
    {
        amqp_0_10::Connection* c = dynamic_cast<amqp_0_10::Connection*>(&in);
        if (c) links.notifyConnection(c->getMgmtId(), c);
    }
    void opened(Connection& in)
    {
        amqp_0_10::Connection* c = dynamic_cast<amqp_0_10::Connection*>(&in);
        if (c) links.notifyOpened(c->getMgmtId());
    }
    void closed(Connection& in)
    {
        amqp_0_10::Connection* c = dynamic_cast<amqp_0_10::Connection*>(&in);
        if (c) links.notifyClosed(c->getMgmtId());
    }
    void forced(Connection& in, const string& text)
    {
        amqp_0_10::Connection* c = dynamic_cast<amqp_0_10::Connection*>(&in);
        if (c) links.notifyConnectionForced(c->getMgmtId(), text);
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

LinkRegistry::LinkRegistry (Broker* _broker) :
    broker(_broker),
<<<<<<< HEAD
    parent(0), store(0), passive(false),
    realm(broker->getOptions().realm)
=======
    parent(0), store(0),
    realm(broker->getRealm())
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    broker->getConnectionObservers().add(
        boost::shared_ptr<ConnectionObserver>(new LinkRegistryConnectionObserver(*this)));
}

LinkRegistry::~LinkRegistry() {}

/** find link by the *configured* remote address */
boost::shared_ptr<Link> LinkRegistry::getLink(const std::string& host,
                                              uint16_t           port,
                                              const std::string& transport)
{
    Mutex::ScopedLock   locker(lock);
    for (LinkMap::iterator i = links.begin(); i != links.end(); ++i) {
        Link::shared_ptr& link = i->second;
        if (link->getHost() == host &&
            link->getPort() == port &&
            (transport.empty() || link->getTransport() == transport))
            return link;
     }
    return boost::shared_ptr<Link>();
}

/** find link by name */
boost::shared_ptr<Link> LinkRegistry::getLink(const std::string& name)
{
    Mutex::ScopedLock   locker(lock);
    LinkMap::iterator l = links.find(name);
    if (l != links.end())
        return l->second;
    return boost::shared_ptr<Link>();
}

pair<Link::shared_ptr, bool> LinkRegistry::declare(const string& name,
                                                   const string&  host,
                                                   uint16_t port,
                                                   const string&  transport,
                                                   bool     durable,
                                                   const string&  authMechanism,
                                                   const string&  username,
                                                   const string&  password,
                                                   bool failover)

{
    Mutex::ScopedLock   locker(lock);

    LinkMap::iterator i = links.find(name);
    if (i == links.end())
    {
        Link::shared_ptr link;

        link = Link::shared_ptr (
            new Link (name, this, host, port, transport,
                      boost::bind(&LinkRegistry::linkDestroyed, this, _1),
                      durable, authMechanism, username, password, broker,
                      parent, failover));
<<<<<<< HEAD
        if (durable && store) store->create(*link);
        links[name] = link;
=======
        if (durable && store && !broker->inRecovery())
            store->create(*link);
        links[name] = link;
        pendingLinks[name] = link;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        QPID_LOG(debug, "Creating new link; name=" << name );
        return std::pair<Link::shared_ptr, bool>(link, true);
    }
    return std::pair<Link::shared_ptr, bool>(i->second, false);
}

/** find bridge by link & route info */
Bridge::shared_ptr LinkRegistry::getBridge(const Link&  link,
                                           const std::string& src,
                                           const std::string& dest,
                                           const std::string& key)
{
    Mutex::ScopedLock   locker(lock);
    for (BridgeMap::iterator i = bridges.begin(); i != bridges.end(); ++i) {
        if (i->second->getSrc() == src && i->second->getDest() == dest &&
            i->second->getKey() == key && i->second->getLink() &&
            i->second->getLink()->getName() == link.getName()) {
            return i->second;
        }
    }
    return Bridge::shared_ptr();
}

/** find bridge by name */
Bridge::shared_ptr LinkRegistry::getBridge(const std::string& name)
{
    Mutex::ScopedLock   locker(lock);
    BridgeMap::iterator b = bridges.find(name);
    if (b != bridges.end())
        return b->second;
    return Bridge::shared_ptr();
}

pair<Bridge::shared_ptr, bool> LinkRegistry::declare(const std::string& name,
                                                     Link&        link,
                                                     bool         durable,
                                                     const std::string& src,
                                                     const std::string& dest,
                                                     const std::string& key,
                                                     bool         isQueue,
                                                     bool         isLocal,
                                                     const std::string& tag,
                                                     const std::string& excludes,
                                                     bool         dynamic,
                                                     uint16_t     sync,
<<<<<<< HEAD
=======
                                                     uint32_t     credit,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                                                     Bridge::InitializeCallback init,
                                                     const std::string& queueName,
                                                     const std::string& altExchange
)
{
    Mutex::ScopedLock locker(lock);

    // Durable bridges are only valid on durable links
    if (durable && !link.isDurable()) {
        QPID_LOG(error, "Can't create a durable route '" << name << "' on a non-durable link '" << link.getName());
         return pair<Bridge::shared_ptr, bool>(Bridge::shared_ptr(), false);
    }

    if (dynamic) {
        Exchange::shared_ptr exchange = broker->getExchanges().get(src);
        if (exchange.get() == 0) {
            QPID_LOG(error, "Exchange not found, name='" << src << "'" );
            return pair<Bridge::shared_ptr, bool>(Bridge::shared_ptr(), false);
        }
        if (!exchange->supportsDynamicBinding()) {
            QPID_LOG(error, "Exchange type does not support dynamic routing, name='" << src << "'");
            return pair<Bridge::shared_ptr, bool>(Bridge::shared_ptr(), false);
        }
    }

    BridgeMap::iterator b = bridges.find(name);
    if (b == bridges.end())
    {
        _qmf::ArgsLinkBridge args;
        Bridge::shared_ptr bridge;

        args.i_durable    = durable;
        args.i_src        = src;
        args.i_dest       = dest;
        args.i_key        = key;
        args.i_srcIsQueue = isQueue;
        args.i_srcIsLocal = isLocal;
        args.i_tag        = tag;
        args.i_excludes   = excludes;
        args.i_dynamic    = dynamic;
        args.i_sync       = sync;
<<<<<<< HEAD
=======
        args.i_credit     = credit;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        bridge = Bridge::shared_ptr
          (new Bridge (name, &link, link.nextChannel(),
                       boost::bind(&LinkRegistry::destroyBridge, this, _1),
                       args, init, queueName, altExchange));
        bridges[name] = bridge;
        link.add(bridge);
<<<<<<< HEAD
        if (durable && store)
=======
        if (durable && store && !broker->inRecovery())
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            store->create(*bridge);

        QPID_LOG(debug, "Bridge '" << name <<"' declared on link '" << link.getName() <<
                 "' from " << src << " to " << dest << " (" << key << ")");

        return std::pair<Bridge::shared_ptr, bool>(bridge, true);
    }
    return std::pair<Bridge::shared_ptr, bool>(b->second, false);
}

/** called back by the link when it has completed its cleanup and can be removed. */
void LinkRegistry::linkDestroyed(Link *link)
{
    QPID_LOG(debug, "LinkRegistry::destroy(); link= " << link->getName());
    Mutex::ScopedLock   locker(lock);

<<<<<<< HEAD
=======
    pendingLinks.erase(link->getName());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    LinkMap::iterator i = links.find(link->getName());
    if (i != links.end())
    {
        if (i->second->isDurable() && store)
            store->destroy(*(i->second));
        links.erase(i);
    }
}

/** called back by bridge when its destruction has been requested */
void LinkRegistry::destroyBridge(Bridge *bridge)
{
    QPID_LOG(debug, "LinkRegistry::destroy(); bridge= " << bridge->getName());
    Mutex::ScopedLock locker(lock);

    BridgeMap::iterator b = bridges.find(bridge->getName());
    if (b == bridges.end())
        return;

    Link *link = b->second->getLink();
    if (link) {
        link->cancel(b->second);
<<<<<<< HEAD
=======
        link->returnChannel( bridge->getChannel() );
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    if (b->second->isDurable())
        store->destroy(*(b->second));
    bridges.erase(b);
}

void LinkRegistry::setStore (MessageStore* _store)
{
    store = _store;
}

MessageStore* LinkRegistry::getStore() const {
    return store;
}

<<<<<<< HEAD
namespace {
    void extractHostPort(const std::string& connId, std::string *host, uint16_t *port)
    {
        // Extract host and port of remote broker from connection id string.
        //
        // TODO aconway 2011-02-01: centralize code that constructs/parses connection
        // management IDs. Currently sys:: protocol factories and IO plugins construct the
        // IDs and LinkRegistry parses them.
        // KAG: current connection id format assumed:
        // "localhost:port-remotehost:port".  In the case of IpV6, the host addresses are
        // contained within brackets "[...]", example:
        // connId="[::1]:36859-[::1]:48603". Liberal use of "asserts" provided to alert us
        // if this assumption changes!
        size_t separator = connId.find('-');
        assert(separator != std::string::npos);
        std::string remote = connId.substr(separator+1, std::string::npos);
        separator = remote.rfind(":");
        assert(separator != std::string::npos);
        *host = remote.substr(0, separator);
        // IPv6 - host is bracketed by "[]", strip them
        if ((*host)[0] == '[' && (*host)[host->length() - 1] == ']') {
            *host = host->substr(1, host->length() - 2);
        }
        try {
            *port = boost::lexical_cast<uint16_t>(remote.substr(separator+1, std::string::npos));
        } catch (const boost::bad_lexical_cast&) {
            QPID_LOG(error, "Invalid format for connection identifier! '" << connId << "'");
            assert(false);
        }
    }
}

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
/** find the Link that corresponds to the given connection */
Link::shared_ptr LinkRegistry::findLink(const std::string& connId)
{
    Mutex::ScopedLock locker(lock);
    ConnectionMap::iterator c = connections.find(connId);
    if (c != connections.end()) {
        LinkMap::iterator l = links.find(c->second);
        if (l != links.end())
            return l->second;
    }
    return Link::shared_ptr();
}

<<<<<<< HEAD
void LinkRegistry::notifyConnection(const std::string& key, Connection* c)
=======
void LinkRegistry::notifyConnection(const std::string& key, amqp_0_10::Connection* c)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    // find a link that is attempting to connect to the remote, and
    // create a mapping from connection id to link
    QPID_LOG(debug, "LinkRegistry::notifyConnection(); key=" << key );
    std::string host;
<<<<<<< HEAD
    uint16_t port = 0;
    extractHostPort( key, &host, &port );
    Link::shared_ptr link;
    {
        Mutex::ScopedLock locker(lock);
        for (LinkMap::iterator l = links.begin(); l != links.end(); ++l) {
            if (l->second->pendingConnection(host, port)) {
                link = l->second;
                connections[key] = link->getName();
                break;
            }
=======
    Link::shared_ptr link;
    {
        Mutex::ScopedLock locker(lock);
        LinkMap::iterator l = pendingLinks.find(key);
        if (l != pendingLinks.end()) {
            link = l->second;
            pendingLinks.erase(l);
            connections[key] = link->getName();
            QPID_LOG(debug, "LinkRegistry:: found pending =" << link->getName());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        }
    }

    if (link) {
        link->established(c);
        c->setUserId(str(format("%1%@%2%") % link->getUsername() % realm));
    }
}

void LinkRegistry::notifyOpened(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (link) link->opened();
}

void LinkRegistry::notifyClosed(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (link) {
<<<<<<< HEAD
=======
        {
            Mutex::ScopedLock locker(lock);
            pendingLinks[link->getName()] = link;
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        link->closed(0, "Closed by peer");
    }
}

void LinkRegistry::notifyConnectionForced(const std::string& key, const std::string& text)
{
    Link::shared_ptr link = findLink(key);
    if (link) {
<<<<<<< HEAD
=======
        {
            Mutex::ScopedLock locker(lock);
            pendingLinks[link->getName()] = link;
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        link->notifyConnectionForced(text);
    }
}

std::string LinkRegistry::getAuthMechanism(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (link)
        return link->getAuthMechanism();
    return string("ANONYMOUS");
}

std::string LinkRegistry::getAuthCredentials(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (!link)
        return string();

    string result;
    result += '\0';
    result += link->getUsername();
    result += '\0';
    result += link->getPassword();

    return result;
}

std::string LinkRegistry::getUsername(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (!link)
        return string();

    return link->getUsername();
}

/** note: returns the current remote host (may be different from the host originally
    configured for the Link due to failover) */
std::string LinkRegistry::getHost(const std::string& key)
{
     Link::shared_ptr link = findLink(key);
     if (!link)
         return string();

     qpid::Address addr;
     link->getRemoteAddress(addr);
     return addr.host;
}

/** returns the current remote port (ditto above) */
uint16_t LinkRegistry::getPort(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (!link)
        return 0;

     qpid::Address addr;
     link->getRemoteAddress(addr);
     return addr.port;
}

std::string LinkRegistry::getPassword(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (!link)
        return string();

    return link->getPassword();
}

std::string LinkRegistry::getAuthIdentity(const std::string& key)
{
    Link::shared_ptr link = findLink(key);
    if (!link)
        return string();

    return link->getUsername();
}

<<<<<<< HEAD

void LinkRegistry::setPassive(bool p)
{
    Mutex::ScopedLock locker(lock);
    passive = p;
    if (passive) { QPID_LOG(info, "Passivating links"); }
    else { QPID_LOG(info, "Activating links"); }
    for (LinkMap::iterator i = links.begin(); i != links.end(); i++) {
        i->second->setPassive(passive);
    }
}

void LinkRegistry::eachLink(boost::function<void(boost::shared_ptr<Link>)> f) {
    Mutex::ScopedLock locker(lock);
    for (LinkMap::iterator i = links.begin(); i != links.end(); ++i) f(i->second);
}

void LinkRegistry::eachBridge(boost::function<void(boost::shared_ptr<Bridge>)> f) {
    Mutex::ScopedLock locker(lock);
    for (BridgeMap::iterator i = bridges.begin(); i != bridges.end(); ++i) f(i->second);
}

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}} // namespace qpid::broker
