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
<<<<<<< HEAD
#include "qpid/broker/ConnectionState.h"
=======

#include "qpid/broker/AclModule.h"
#include "qpid/broker/BrokerOptions.h"
#include "qpid/broker/Connection.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/broker/DirectExchange.h"
#include "qpid/broker/FanOutExchange.h"
#include "qpid/broker/HeadersExchange.h"
#include "qpid/broker/MessageStoreModule.h"
<<<<<<< HEAD
#include "qpid/broker/NullMessageStore.h"
#include "qpid/broker/RecoveryManagerImpl.h"
#include "qpid/broker/SaslAuthenticator.h"
#include "qpid/broker/SecureConnectionFactory.h"
#include "qpid/broker/TopicExchange.h"
#include "qpid/broker/Link.h"
#include "qpid/broker/ExpiryPolicy.h"
#include "qpid/broker/QueueFlowLimit.h"
#include "qpid/broker/MessageGroupManager.h"

#include "qmf/org/apache/qpid/broker/Package.h"
=======
#include "qpid/broker/NameGenerator.h"
#include "qpid/broker/NullMessageStore.h"
#include "qpid/broker/RecoveryManagerImpl.h"
#include "qpid/broker/SaslAuthenticator.h"
#include "qpid/broker/TopicExchange.h"
#include "qpid/broker/Link.h"
#include "qpid/broker/PersistableObject.h"
#include "qpid/broker/QueueFlowLimit.h"
#include "qpid/broker/QueueSettings.h"
#include "qpid/broker/TransactionObserver.h"
#include "qpid/broker/MessageGroupManager.h"

#include "qmf/org/apache/qpid/broker/Package.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerConnect.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qmf/org/apache/qpid/broker/ArgsBrokerCreate.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerDelete.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerQuery.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerEcho.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerGetLogLevel.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerQueueMoveMessages.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerSetLogLevel.h"
<<<<<<< HEAD
#include "qmf/org/apache/qpid/broker/ArgsBrokerSetTimestampConfig.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerGetTimestampConfig.h"
#include "qmf/org/apache/qpid/broker/EventExchangeDeclare.h"
#include "qmf/org/apache/qpid/broker/EventExchangeDelete.h"
#include "qmf/org/apache/qpid/broker/EventQueueDeclare.h"
#include "qmf/org/apache/qpid/broker/EventQueueDelete.h"
#include "qmf/org/apache/qpid/broker/EventBind.h"
#include "qmf/org/apache/qpid/broker/EventUnbind.h"
=======
#include "qmf/org/apache/qpid/broker/ArgsBrokerGetLogHiresTimestamp.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerSetLogHiresTimestamp.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerSetTimestampConfig.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerGetTimestampConfig.h"
#include "qmf/org/apache/qpid/broker/ArgsBrokerQueueRedirect.h"
#include "qmf/org/apache/qpid/broker/EventExchangeDeclare.h"
#include "qmf/org/apache/qpid/broker/EventExchangeDelete.h"
#include "qmf/org/apache/qpid/broker/EventBind.h"
#include "qmf/org/apache/qpid/broker/EventUnbind.h"
#include "qmf/org/apache/qpid/broker/EventQueueRedirect.h"
#include "qmf/org/apache/qpid/broker/EventQueueRedirectCancelled.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/amqp_0_10/Codecs.h"
#include "qpid/management/ManagementDirectExchange.h"
#include "qpid/management/ManagementTopicExchange.h"
#include "qpid/log/Logger.h"
#include "qpid/log/Options.h"
#include "qpid/log/Statement.h"
<<<<<<< HEAD
#include "qpid/log/posix/SinkOptions.h"
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/framing/AMQFrame.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/ProtocolInitiation.h"
#include "qpid/framing/reply_exceptions.h"
#include "qpid/framing/Uuid.h"
<<<<<<< HEAD
#include "qpid/sys/ProtocolFactory.h"
=======
#include "qpid/sys/TransportFactory.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/sys/Poller.h"
#include "qpid/sys/Dispatcher.h"
#include "qpid/sys/Thread.h"
#include "qpid/sys/Time.h"
<<<<<<< HEAD
#include "qpid/sys/ConnectionInputHandler.h"
#include "qpid/sys/ConnectionInputHandlerFactory.h"
#include "qpid/sys/TimeoutHandler.h"
=======
#include "qpid/sys/Timer.h"
#include "qpid/sys/ConnectionInputHandlerFactory.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/sys/SystemInfo.h"
#include "qpid/Address.h"
#include "qpid/StringUtils.h"
#include "qpid/Url.h"
#include "qpid/Version.h"

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <memory>
<<<<<<< HEAD

using qpid::sys::ProtocolFactory;
=======
#include <set>

using qpid::sys::TransportAcceptor;
using qpid::sys::TransportConnector;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
using qpid::sys::Poller;
using qpid::sys::Dispatcher;
using qpid::sys::Thread;
using qpid::framing::FrameHandler;
using qpid::framing::ChannelId;
using qpid::management::ManagementAgent;
using qpid::management::ManagementObject;
using qpid::management::Manageable;
using qpid::management::Args;
<<<<<<< HEAD
using qpid::management::getManagementExecutionContext;
=======
using qpid::management::getCurrentPublisher;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
using qpid::types::Variant;
using std::string;
using std::make_pair;

namespace _qmf = qmf::org::apache::qpid::broker;

namespace qpid {
namespace broker {

<<<<<<< HEAD
Broker::Options::Options(const std::string& name) :
=======
const std::string empty;
const std::string amq_direct("amq.direct");
const std::string amq_topic("amq.topic");
const std::string amq_fanout("amq.fanout");
const std::string amq_match("amq.match");
const std::string qpid_management("qpid.management");
const std::string knownHostsNone("none");

BrokerOptions::BrokerOptions(const std::string& name) :
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    qpid::Options(name),
    noDataDir(0),
    port(DEFAULT_PORT),
    workerThreads(5),
    connectionBacklog(10),
    enableMgmt(1),
    mgmtPublish(1),
<<<<<<< HEAD
    mgmtPubInterval(10),
    queueCleanInterval(60*10),//10 minutes
=======
    mgmtPubInterval(10*sys::TIME_SEC),
    queueCleanInterval(60*sys::TIME_SEC*10),//10 minutes
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    auth(SaslAuthenticator::available()),
    realm("QPID"),
    replayFlushLimit(0),
    replayHardLimit(0),
    queueLimit(100*1048576/*100M default limit*/),
<<<<<<< HEAD
    tcpNoDelay(false),
    requireEncrypted(false),
    asyncQueueEvents(false),     // Must be false in a cluster.
    qmf2Support(true),
    qmf1Support(true),
=======
    tcpNoDelay(true),
    requireEncrypted(false),
    knownHosts(knownHostsNone),
    qmf2Support(true),
    qmf1Support(false),
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    queueFlowStopRatio(80),
    queueFlowResumeRatio(70),
    queueThresholdEventRatio(80),
    defaultMsgGroup("qpid.no-group"),
    timestampRcvMsgs(false),    // set the 0.10 timestamp delivery property
<<<<<<< HEAD
    linkMaintenanceInterval(2),
    linkHeartbeatInterval(120),
    maxNegotiateTime(2000)      // 2s
=======
    linkMaintenanceInterval(2*sys::TIME_SEC),
    linkHeartbeatInterval(120*sys::TIME_SEC),
    dtxDefaultTimeout(60),      // 60s
    dtxMaxTimeout(3600),        // 3600s
    maxNegotiateTime(10000)     // 10s
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    int c = sys::SystemInfo::concurrency();
    workerThreads=c+1;
    std::string home = getHome();

    if (home.length() == 0)
        dataDir += DEFAULT_DATA_DIR_LOCATION;
    else
        dataDir += home;
    dataDir += DEFAULT_DATA_DIR_NAME;

    addOptions()
        ("data-dir", optValue(dataDir,"DIR"), "Directory to contain persistent data generated by the broker")
        ("no-data-dir", optValue(noDataDir), "Don't use a data directory.  No persistent configuration will be loaded or stored")
<<<<<<< HEAD
        ("port,p", optValue(port,"PORT"), "Tells the broker to listen on PORT")
=======
        ("paging-dir", optValue(pagingDir,"DIR"), "Directory in which paging files will be created for paged queues")
        ("port,p", optValue(port,"PORT"), "Tells the broker to listen on PORT")
        ("interface", optValue(listenInterfaces, "<interface name>|<interface address>"), "Which network interfaces to use to listen for incoming connections")
        ("listen-disable", optValue(listenDisabled, "<transport name>"), "Transports to disable listening")
        ("protocols", optValue(protocols, "<protocol name+version>"), "Which protocol versions to allow")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        ("worker-threads", optValue(workerThreads, "N"), "Sets the broker thread pool size")
        ("connection-backlog", optValue(connectionBacklog, "N"), "Sets the connection backlog limit for the server socket")
        ("mgmt-enable,m", optValue(enableMgmt,"yes|no"), "Enable Management")
        ("mgmt-publish", optValue(mgmtPublish,"yes|no"), "Enable Publish of Management Data ('no' implies query-only)")
        ("mgmt-qmf2", optValue(qmf2Support,"yes|no"), "Enable broadcast of management information over QMF v2")
        ("mgmt-qmf1", optValue(qmf1Support,"yes|no"), "Enable broadcast of management information over QMF v1")
<<<<<<< HEAD
        // FIXME aconway 2012-02-13: consistent treatment of values in SECONDS
        // allow sub-second intervals.
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        ("mgmt-pub-interval", optValue(mgmtPubInterval, "SECONDS"), "Management Publish Interval")
        ("queue-purge-interval", optValue(queueCleanInterval, "SECONDS"),
         "Interval between attempts to purge any expired messages from queues")
        ("auth", optValue(auth, "yes|no"), "Enable authentication, if disabled all incoming connections will be trusted")
        ("realm", optValue(realm, "REALM"), "Use the given realm when performing authentication")
        ("default-queue-limit", optValue(queueLimit, "BYTES"), "Default maximum size for queues (in bytes)")
        ("tcp-nodelay", optValue(tcpNoDelay), "Set TCP_NODELAY on TCP connections")
        ("require-encryption", optValue(requireEncrypted), "Only accept connections that are encrypted")
        ("known-hosts-url", optValue(knownHosts, "URL or 'none'"), "URL to send as 'known-hosts' to clients ('none' implies empty list)")
<<<<<<< HEAD
        ("sasl-config", optValue(saslConfigPath, "DIR"), "gets sasl config info from nonstandard location")
        ("async-queue-events", optValue(asyncQueueEvents, "yes|no"), "Set Queue Events async, used for services like replication")
=======
        ("sasl-config", optValue(saslConfigPath, "DIR"), "Allows SASL config path, if supported by platform, to be overridden.  For default location on Linux, see Cyrus SASL documentation.  There is no SASL config dir on Windows.")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        ("default-flow-stop-threshold", optValue(queueFlowStopRatio, "PERCENT"), "Percent of queue's maximum capacity at which flow control is activated.")
        ("default-flow-resume-threshold", optValue(queueFlowResumeRatio, "PERCENT"), "Percent of queue's maximum capacity at which flow control is de-activated.")
        ("default-event-threshold-ratio", optValue(queueThresholdEventRatio, "%age of limit"), "The ratio of any specified queue limit at which an event will be raised")
        ("default-message-group", optValue(defaultMsgGroup, "GROUP-IDENTIFER"), "Group identifier to assign to messages delivered to a message group queue that do not contain an identifier.")
        ("enable-timestamp", optValue(timestampRcvMsgs, "yes|no"), "Add current time to each received message.")
<<<<<<< HEAD
        ("link-maintenace-interval", optValue(linkMaintenanceInterval, "SECONDS"))
        ("link-heartbeat-interval", optValue(linkHeartbeatInterval, "SECONDS"))
        ("max-negotiate-time", optValue(maxNegotiateTime, "MilliSeconds"), "Maximum time a connection can take to send the initial protocol negotiation")
=======
        ("link-maintenance-interval", optValue(linkMaintenanceInterval, "SECONDS"),
         "Interval to check federation link health and re-connect if need be")
        ("link-heartbeat-interval", optValue(linkHeartbeatInterval, "SECONDS"),
         "Heartbeat interval for a federation link")
        ("dtx-default-timeout", optValue(dtxDefaultTimeout, "SECONDS"), "Default timeout for DTX transaction before aborting it")
        ("dtx-max-timeout", optValue(dtxMaxTimeout, "SECONDS"), "Maximum allowed timeout for DTX transaction. A value of zero disables maximum timeout limit checks and allows arbitrarily large timeout settings.")
        ("max-negotiate-time", optValue(maxNegotiateTime, "MILLISECONDS"), "Maximum time a connection can take to send the initial protocol negotiation")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        ("federation-tag", optValue(fedTag, "NAME"), "Override the federation tag")
        ;
}

<<<<<<< HEAD
const std::string empty;
const std::string amq_direct("amq.direct");
const std::string amq_topic("amq.topic");
const std::string amq_fanout("amq.fanout");
const std::string amq_match("amq.match");
const std::string qpid_management("qpid.management");
const std::string knownHostsNone("none");

Broker::Broker(const Broker::Options& conf) :
    poller(new Poller),
=======
namespace {
// Arguments to declare a non-replicated exchange.
framing::FieldTable noReplicateArgs() {
    framing::FieldTable args;
    args.setString("qpid.replicate", "none");
    return args;
}
}

Broker::LogPrefix::LogPrefix() :
    std::string(Msg() << "Broker (pid=" << sys::SystemInfo::getProcessId() << ") ") {
    QPID_LOG(notice, *this << "start-up");
}

Broker::LogPrefix::~LogPrefix() { QPID_LOG(notice, *this << "shut-down"); }

Broker::Broker(const BrokerOptions& conf) :
    poller(new Poller),
    timer(new qpid::sys::Timer),
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    config(conf),
    managementAgent(conf.enableMgmt ? new ManagementAgent(conf.qmf1Support,
                                                          conf.qmf2Support)
                                    : 0),
<<<<<<< HEAD
    store(new NullMessageStore),
    acl(0),
    dataDir(conf.noDataDir ? std::string() : conf.dataDir),
    queues(this),
    exchanges(this),
    links(this),
    factory(new SecureConnectionFactory(*this)),
    dtxManager(timer),
=======
    disabledListeningTransports(conf.listenDisabled.begin(), conf.listenDisabled.end()),
    store(new NullMessageStore),
    acl(0),
    dataDir(conf.noDataDir ? std::string() : conf.dataDir),
    pagingDir(!conf.pagingDir.empty() ? conf.pagingDir :
              dataDir.isEnabled() ? dataDir.getPath() + BrokerOptions::DEFAULT_PAGED_QUEUE_DIR :
              std::string() ),
    queues(this),
    exchanges(this),
    links(this),
    dtxManager(*timer.get(), conf.dtxDefaultTimeout),
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    sessionManager(
        qpid::SessionState::Configuration(
            conf.replayFlushLimit*1024, // convert kb to bytes.
            conf.replayHardLimit*1024),
        *this),
<<<<<<< HEAD
    mgmtObject(0),
    queueCleaner(queues, &timer),
    queueEvents(poller,!conf.asyncQueueEvents),
    recovery(true),
    inCluster(false),
    clusterUpdatee(false),
    expiryPolicy(new ExpiryPolicy),
    getKnownBrokers(boost::bind(&Broker::getKnownBrokersImpl, this)),
    deferDelivery(boost::bind(&Broker::deferDeliveryImpl, this, _1, _2))
{
=======
    queueCleaner(queues, poller, timer.get()),
    recoveryInProgress(false),
    protocolRegistry(std::set<std::string>(conf.protocols.begin(), conf.protocols.end()), this),
    timestampRcvMsgs(conf.timestampRcvMsgs),
    getKnownBrokers(boost::bind(&Broker::getKnownBrokersImpl, this))
{
    if (!dataDir.isEnabled()) {
        QPID_LOG (info, "No data directory - Disabling persistent configuration");
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    try {
    if (conf.enableMgmt) {
        QPID_LOG(info, "Management enabled");
        managementAgent->configure(dataDir.isEnabled() ? dataDir.getPath() : string(), conf.mgmtPublish,
<<<<<<< HEAD
                                   conf.mgmtPubInterval, this, conf.workerThreads + 3);
=======
                                   conf.mgmtPubInterval/sys::TIME_SEC, this, conf.workerThreads + 3);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        managementAgent->setName("apache.org", "qpidd");
        _qmf::Package packageInitializer(managementAgent.get());

        System* system = new System (dataDir.isEnabled() ? dataDir.getPath() : string(), this);
        systemObject = System::shared_ptr(system);

<<<<<<< HEAD
        mgmtObject = new _qmf::Broker(managementAgent.get(), this, system, "amqp-broker");
=======
        mgmtObject = _qmf::Broker::shared_ptr(new _qmf::Broker(managementAgent.get(), this, system, "amqp-broker"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        mgmtObject->set_systemRef(system->GetManagementObject()->getObjectId());
        mgmtObject->set_port(conf.port);
        mgmtObject->set_workerThreads(conf.workerThreads);
        mgmtObject->set_connBacklog(conf.connectionBacklog);
<<<<<<< HEAD
        mgmtObject->set_mgmtPubInterval(conf.mgmtPubInterval);
=======
        mgmtObject->set_mgmtPubInterval(conf.mgmtPubInterval/sys::TIME_SEC);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        mgmtObject->set_mgmtPublish(conf.mgmtPublish);
        mgmtObject->set_version(qpid::version);
        if (dataDir.isEnabled())
            mgmtObject->set_dataDir(dataDir.getPath());
        else
            mgmtObject->clr_dataDir();

        managementAgent->addObject(mgmtObject, 0, true);

        // Since there is currently no support for virtual hosts, a placeholder object
        // representing the implied single virtual host is added here to keep the
        // management schema correct.
        Vhost* vhost = new Vhost(this, this);
        vhostObject = Vhost::shared_ptr(vhost);
        if (conf.fedTag.empty()) {
            framing::Uuid uuid(managementAgent->getUuid());
            federationTag = uuid.str();
        } else
            federationTag = conf.fedTag;
        vhostObject->setFederationTag(federationTag);

        queues.setParent(vhost);
        exchanges.setParent(vhost);
        links.setParent(vhost);
    } else {
        // Management is disabled so there is no broker management ID.
        // Create a unique uuid to use as the federation tag.
        if (conf.fedTag.empty()) {
            framing::Uuid uuid(true);
            federationTag = uuid.str();
        } else
            federationTag = conf.fedTag;
    }

<<<<<<< HEAD
    QueuePolicy::setDefaultMaxSize(conf.queueLimit);

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    // Early-Initialize plugins
    Plugin::earlyInitAll(*this);

    QueueFlowLimit::setDefaults(conf.queueLimit, conf.queueFlowStopRatio, conf.queueFlowResumeRatio);
    MessageGroupManager::setDefaults(conf.defaultMsgGroup);

    // If no plugin store module registered itself, set up the null store.
    if (NullMessageStore::isNullStore(store.get()))
        setStore();

<<<<<<< HEAD
    exchanges.declare(empty, DirectExchange::typeName); // Default exchange.

    if (store.get() != 0) {
        // The cluster plug-in will setRecovery(false) on all but the first
        // broker to join a cluster.
        if (getRecovery()) {
            RecoveryManagerImpl recoverer(queues, exchanges, links, dtxManager);
            store->recover(recoverer);
        }
        else {
            QPID_LOG(notice, "Cluster recovery: recovered journal data discarded and journal files pushed down");
            store->truncateInit(true); // save old files in subdir
        }
=======
    framing::FieldTable args;

    // Default exchnge is not replicated.
    exchanges.declare(empty, DirectExchange::typeName, false, false, noReplicateArgs());

    RecoveredObjects objects;
    if (store.get() != 0) {
        RecoveryManagerImpl recoverer(
            queues, exchanges, links, dtxManager, protocolRegistry, objects);
        recoveryInProgress = true;
        store->recover(recoverer);
        recoveryInProgress = false;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }

    //ensure standard exchanges exist (done after recovery from store)
    declareStandardExchange(amq_direct, DirectExchange::typeName);
    declareStandardExchange(amq_topic, TopicExchange::typeName);
    declareStandardExchange(amq_fanout, FanOutExchange::typeName);
    declareStandardExchange(amq_match, HeadersExchange::typeName);

    if(conf.enableMgmt) {
<<<<<<< HEAD
        exchanges.declare(qpid_management, ManagementTopicExchange::typeName);
=======
        exchanges.declare(qpid_management, ManagementTopicExchange::typeName, false, false, noReplicateArgs());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        Exchange::shared_ptr mExchange = exchanges.get(qpid_management);
        Exchange::shared_ptr dExchange = exchanges.get(amq_direct);
        managementAgent->setExchange(mExchange, dExchange);
        boost::dynamic_pointer_cast<ManagementTopicExchange>(mExchange)->setManagmentAgent(managementAgent.get(), 1);

        std::string qmfTopic("qmf.default.topic");
        std::string qmfDirect("qmf.default.direct");

<<<<<<< HEAD
        std::pair<Exchange::shared_ptr, bool> topicPair(exchanges.declare(qmfTopic, ManagementTopicExchange::typeName));
        std::pair<Exchange::shared_ptr, bool> directPair(exchanges.declare(qmfDirect, ManagementDirectExchange::typeName));
=======
        std::pair<Exchange::shared_ptr, bool> topicPair(
            exchanges.declare(qmfTopic, ManagementTopicExchange::typeName, false, false, noReplicateArgs()));
        std::pair<Exchange::shared_ptr, bool> directPair(
            exchanges.declare(qmfDirect, ManagementDirectExchange::typeName, false, false, noReplicateArgs()));
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        boost::dynamic_pointer_cast<ManagementDirectExchange>(directPair.first)->setManagmentAgent(managementAgent.get(), 2);
        boost::dynamic_pointer_cast<ManagementTopicExchange>(topicPair.first)->setManagmentAgent(managementAgent.get(), 2);

        managementAgent->setExchangeV2(topicPair.first, directPair.first);
    }
    else
        QPID_LOG(info, "Management not enabled");

    // this feature affects performance, so let's be sure that gets logged!
    if (conf.timestampRcvMsgs) {
        QPID_LOG(notice, "Receive message timestamping is ENABLED.");
    }

    /**
     * SASL setup, can fail and terminate startup
     */
    if (conf.auth) {
        SaslAuthenticator::init(qpid::saslName, conf.saslConfigPath);
        QPID_LOG(info, "SASL enabled");
    } else {
        QPID_LOG(notice, "SASL disabled: No Authentication Performed");
    }

    // Initialize plugins
    Plugin::initializeAll(*this);
<<<<<<< HEAD

    if (managementAgent.get()) managementAgent->pluginsInitialized();

    if (conf.queueCleanInterval) {
        queueCleaner.start(conf.queueCleanInterval * qpid::sys::TIME_SEC);
    }

    //initialize known broker urls (TODO: add support for urls for other transports (SSL, RDMA)):
    if (conf.knownHosts.empty()) {
        boost::shared_ptr<ProtocolFactory> factory = getProtocolFactory(TCP_TRANSPORT);
        if (factory) {
            knownBrokers.push_back ( qpid::Url::getIpAddressesUrl ( factory->getPort() ) );
        }
    } else if (conf.knownHosts != knownHostsNone) {
        knownBrokers.push_back(Url(conf.knownHosts));
    }

    } catch (const std::exception&) {
        finalize();
        throw;
    }
=======
    //recover any objects via object factories
    objects.restore(*this);

    // Assign to queues their users who created them (can be done after ACL is loaded in Plugin::initializeAll above
    if ((getAcl()) && (store.get())) {
        queues.eachQueue(boost::bind(&qpid::broker::Queue::updateAclUserQueueCount, _1));
    }

    if(conf.enableMgmt) {
        if (getAcl()) {
            mgmtObject->set_maxConns(getAcl()->getMaxConnectTotal());
        }
    }

    if (conf.queueCleanInterval) {
        queueCleaner.start(conf.queueCleanInterval);
    }

    if (!conf.knownHosts.empty() && conf.knownHosts != knownHostsNone) {
        knownBrokers.push_back(Url(conf.knownHosts));
    }

    } catch (const std::exception& e) {
        QPID_LOG(critical, logPrefix << "start-up failed: " << e.what());
        finalize();
        throw;
    }
    QPID_LOG(info, logPrefix << "initialized");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Broker::declareStandardExchange(const std::string& name, const std::string& type)
{
    bool storeEnabled = store.get() != NULL;
<<<<<<< HEAD
    std::pair<Exchange::shared_ptr, bool> status = exchanges.declare(name, type, storeEnabled);
=======
    framing::FieldTable args;
    // Standard exchanges are not replicated.
    std::pair<Exchange::shared_ptr, bool> status =
        exchanges.declare(name, type, storeEnabled, false, noReplicateArgs());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (status.second && storeEnabled) {
        store->create(*status.first, framing::FieldTable ());
    }
}

<<<<<<< HEAD

boost::intrusive_ptr<Broker> Broker::create(int16_t port)
{
    Options config;
=======
bool Broker::isAuthenticating() const
{
    return config.auth;
}

bool Broker::requireEncrypted() const
{
    return config.requireEncrypted;
}

std::string Broker::getRealm() const
{
    return config.realm;
}

bool Broker::getTcpNoDelay() const
{
    return config.tcpNoDelay;
}

uint32_t Broker::getMaxNegotiateTime() const
{
    return config.maxNegotiateTime;
}

uint16_t Broker::getPortOption() const
{
    return config.port;
}

const std::vector<std::string>& Broker::getListenInterfaces() const
{
    return config.listenInterfaces;
}

int Broker::getConnectionBacklog() const
{
    return config.connectionBacklog;
}

sys::Duration Broker::getLinkMaintenanceInterval() const
{
    return config.linkMaintenanceInterval;
}

sys::Duration Broker::getLinkHeartbeatInterval() const
{
    return config.linkHeartbeatInterval;
}

uint32_t Broker::getDtxMaxTimeout() const
{
    return config.dtxMaxTimeout;
}

uint16_t Broker::getQueueThresholdEventRatio() const
{
    return config.queueThresholdEventRatio;
}

uint Broker::getQueueLimit() const
{
    return config.queueLimit;
}

boost::intrusive_ptr<Broker> Broker::create(int16_t port)
{
    BrokerOptions config;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    config.port=port;
    return create(config);
}

<<<<<<< HEAD
boost::intrusive_ptr<Broker> Broker::create(const Options& opts)
=======
boost::intrusive_ptr<Broker> Broker::create(const BrokerOptions& opts)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    return boost::intrusive_ptr<Broker>(new Broker(opts));
}

<<<<<<< HEAD
void Broker::setStore (boost::shared_ptr<MessageStore>& _store)
{
=======
void Broker::setStore (const boost::shared_ptr<MessageStore>& _store)
{
    // Exit now if multiple store plugins are attempting to load
    if (!NullMessageStore::isNullStore(store.get())) {
        QPID_LOG(error, "Multiple store plugins are not supported");
        throw Exception(QPID_MSG("Failed to start broker: Multiple store plugins were loaded"));
    }

>>>>>>> 3bbfc42... Imported Upstream version 0.32
    store.reset(new MessageStoreModule (_store));
    setStore();
}

void Broker::setStore () {
    queues.setStore     (store.get());
    dtxManager.setStore (store.get());
    links.setStore      (store.get());
}

void Broker::run() {
    if (config.workerThreads > 0) {
<<<<<<< HEAD
        QPID_LOG(notice, "Broker running");
=======
        QPID_LOG(info, logPrefix << "running");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        Dispatcher d(poller);
        int numIOThreads = config.workerThreads;
        std::vector<Thread> t(numIOThreads-1);

        // Run n-1 io threads
        for (int i=0; i<numIOThreads-1; ++i)
            t[i] = Thread(d);

        // Run final thread
        d.run();

        // Now wait for n-1 io threads to exit
        for (int i=0; i<numIOThreads-1; ++i) {
            t[i].join();
        }
<<<<<<< HEAD
=======
        QPID_LOG(info, logPrefix << "stopped");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else {
        throw Exception((boost::format("Invalid value for worker-threads: %1%") % config.workerThreads).str());
    }
}

void Broker::shutdown() {
    // NB: this function must be async-signal safe, it must not
    // call any function that is not async-signal safe.
    // Any unsafe shutdown actions should be done in the destructor.
    poller->shutdown();
}

Broker::~Broker() {
<<<<<<< HEAD
    shutdown();
    queueEvents.shutdown();
    finalize();                 // Finalize any plugins.
    if (config.auth)
        SaslAuthenticator::fini();
    timer.stop();
    QPID_LOG(notice, "Shut down");
}

ManagementObject* Broker::GetManagementObject(void) const
{
    return (ManagementObject*) mgmtObject;
=======
    QPID_LOG(info, logPrefix << "shutting down");
    if (mgmtObject != 0)
        mgmtObject->debugStats("destroying");
    shutdown();
    finalize();                 // Finalize any plugins.
    if (config.auth)
        SaslAuthenticator::fini();
    timer->stop();
    managementAgent.reset();
}

ManagementObject::shared_ptr Broker::GetManagementObject(void) const
{
    return mgmtObject;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Manageable* Broker::GetVhostObject(void) const
{
    return vhostObject.get();
}

Manageable::status_t Broker::ManagementMethod (uint32_t methodId,
                                               Args&    args,
                                               string&  text)
{
    Manageable::status_t status = Manageable::STATUS_UNKNOWN_METHOD;

    switch (methodId)
    {
    case _qmf::Broker::METHOD_ECHO :
        QPID_LOG (debug, "Broker::echo("
                  << dynamic_cast<_qmf::ArgsBrokerEcho&>(args).io_sequence
                  << ", "
                  << dynamic_cast<_qmf::ArgsBrokerEcho&>(args).io_body
                  << ")");
        status = Manageable::STATUS_OK;
        break;
    case _qmf::Broker::METHOD_CONNECT : {
        /** Management is creating a Link to a remote broker using the host and port of
         * the remote.  This (old) interface does not allow management to specify a name
         * for the link, nor does it allow multiple Links to the same remote.  Use the
         * "create()" broker method if these features are needed.
         * TBD: deprecate this interface.
         */
        QPID_LOG(info, "The Broker::connect() method will be removed in a future release of QPID."
                 " Please use the Broker::create() method with type='link' instead.");
        _qmf::ArgsBrokerConnect& hp=
            dynamic_cast<_qmf::ArgsBrokerConnect&>(args);

        string transport = hp.i_transport.empty() ? TCP_TRANSPORT : hp.i_transport;
        QPID_LOG (debug, "Broker::connect() " << hp.i_host << ":" << hp.i_port << "; transport=" << transport <<
                        "; durable=" << (hp.i_durable?"T":"F") << "; authMech=\"" << hp.i_authMechanism << "\"");
<<<<<<< HEAD
        if (!getProtocolFactory(transport)) {
=======
        if (!getTransportInfo(transport).connectorFactory) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            QPID_LOG(error, "Transport '" << transport << "' not supported");
            text = "transport type not supported";
            return  Manageable::STATUS_NOT_IMPLEMENTED;
        }

        // Does a link to the remote already exist?  If so, re-use the existing link
        // - this behavior is backward compatible with previous releases.
        if (!links.getLink(hp.i_host, hp.i_port, transport)) {
            // new link, need to generate a unique name for it
            std::pair<Link::shared_ptr, bool> response =
              links.declare(Link::createName(transport, hp.i_host, hp.i_port),
                            hp.i_host, hp.i_port, transport,
                            hp.i_durable, hp.i_authMechanism, hp.i_username, hp.i_password);
            if (!response.first) {
                text = "Unable to create Link";
                status = Manageable::STATUS_PARAMETER_INVALID;
                break;
            }
        }
        status = Manageable::STATUS_OK;
        break;
      }
    case _qmf::Broker::METHOD_QUEUEMOVEMESSAGES : {
        _qmf::ArgsBrokerQueueMoveMessages& moveArgs=
            dynamic_cast<_qmf::ArgsBrokerQueueMoveMessages&>(args);
        QPID_LOG (debug, "Broker::queueMoveMessages()");
<<<<<<< HEAD
        if (queueMoveMessages(moveArgs.i_srcQueue, moveArgs.i_destQueue, moveArgs.i_qty, moveArgs.i_filter))
=======
        if (queueMoveMessages(moveArgs.i_srcQueue, moveArgs.i_destQueue, moveArgs.i_qty,
                              moveArgs.i_filter, getCurrentPublisher()) >=0)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            status = Manageable::STATUS_OK;
        else
            return Manageable::STATUS_PARAMETER_INVALID;
        break;
      }
    case _qmf::Broker::METHOD_SETLOGLEVEL :
        setLogLevel(dynamic_cast<_qmf::ArgsBrokerSetLogLevel&>(args).i_level);
        QPID_LOG (debug, "Broker::setLogLevel()");
        status = Manageable::STATUS_OK;
        break;
    case _qmf::Broker::METHOD_GETLOGLEVEL :
        dynamic_cast<_qmf::ArgsBrokerGetLogLevel&>(args).o_level = getLogLevel();
        QPID_LOG (debug, "Broker::getLogLevel()");
        status = Manageable::STATUS_OK;
        break;
    case _qmf::Broker::METHOD_CREATE :
      {
          _qmf::ArgsBrokerCreate& a = dynamic_cast<_qmf::ArgsBrokerCreate&>(args);
<<<<<<< HEAD
          createObject(a.i_type, a.i_name, a.i_properties, a.i_strict, getManagementExecutionContext());
=======
          createObject(a.i_type, a.i_name, a.i_properties, a.i_strict, getCurrentPublisher());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
          status = Manageable::STATUS_OK;
          break;
      }
    case _qmf::Broker::METHOD_DELETE :
      {
          _qmf::ArgsBrokerDelete& a = dynamic_cast<_qmf::ArgsBrokerDelete&>(args);
<<<<<<< HEAD
          deleteObject(a.i_type, a.i_name, a.i_options, getManagementExecutionContext());
=======
          deleteObject(a.i_type, a.i_name, a.i_options, getCurrentPublisher());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
          status = Manageable::STATUS_OK;
          break;
      }
    case _qmf::Broker::METHOD_QUERY :
      {
          _qmf::ArgsBrokerQuery& a = dynamic_cast<_qmf::ArgsBrokerQuery&>(args);
<<<<<<< HEAD
          status = queryObject(a.i_type, a.i_name, a.o_results, getManagementExecutionContext());
=======
          status = queryObject(a.i_type, a.i_name, a.o_results, getCurrentPublisher());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
          break;
      }
    case _qmf::Broker::METHOD_GETTIMESTAMPCONFIG:
        {
          _qmf::ArgsBrokerGetTimestampConfig& a = dynamic_cast<_qmf::ArgsBrokerGetTimestampConfig&>(args);
<<<<<<< HEAD
          status = getTimestampConfig(a.o_receive, getManagementExecutionContext());
=======
          status = getTimestampConfig(a.o_receive, getCurrentPublisher());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
          break;
        }
    case _qmf::Broker::METHOD_SETTIMESTAMPCONFIG:
        {
          _qmf::ArgsBrokerSetTimestampConfig& a = dynamic_cast<_qmf::ArgsBrokerSetTimestampConfig&>(args);
<<<<<<< HEAD
          status = setTimestampConfig(a.i_receive, getManagementExecutionContext());
          break;
        }
   default:
=======
          status = setTimestampConfig(a.i_receive, getCurrentPublisher());
          break;
        }

    case _qmf::Broker::METHOD_GETLOGHIRESTIMESTAMP:
    {
        dynamic_cast<_qmf::ArgsBrokerGetLogHiresTimestamp&>(args).o_logHires = getLogHiresTimestamp();
        QPID_LOG (debug, "Broker::getLogHiresTimestamp()");
        status = Manageable::STATUS_OK;
        break;
    }
    case _qmf::Broker::METHOD_SETLOGHIRESTIMESTAMP:
    {
        setLogHiresTimestamp(dynamic_cast<_qmf::ArgsBrokerSetLogHiresTimestamp&>(args).i_logHires);
        QPID_LOG (debug, "Broker::setLogHiresTimestamp()");
        status = Manageable::STATUS_OK;
        break;
    }
    case _qmf::Broker::METHOD_QUEUEREDIRECT:
    {
        string srcQueue(dynamic_cast<_qmf::ArgsBrokerQueueRedirect&>(args).i_sourceQueue);
        string tgtQueue(dynamic_cast<_qmf::ArgsBrokerQueueRedirect&>(args).i_targetQueue);
        QPID_LOG (debug, "Broker::queueRedirect source queue:" << srcQueue << " to target queue " << tgtQueue);
        status =  queueRedirect(srcQueue, tgtQueue, getCurrentPublisher());
        break;
    }
    case _qmf::Broker::METHOD_SHUTDOWN :
    {
        QPID_LOG (info, "Broker received shutdown command");
        shutdown();
    }
    default:
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        QPID_LOG (debug, "Broker ManagementMethod not implemented: id=" << methodId << "]");
        status = Manageable::STATUS_NOT_IMPLEMENTED;
        break;
    }

    return status;
}

namespace
{
const std::string TYPE_QUEUE("queue");
const std::string TYPE_EXCHANGE("exchange");
const std::string TYPE_TOPIC("topic");
const std::string TYPE_BINDING("binding");
const std::string TYPE_LINK("link");
const std::string TYPE_BRIDGE("bridge");
const std::string DURABLE("durable");
const std::string AUTO_DELETE("auto-delete");
const std::string ALTERNATE_EXCHANGE("alternate-exchange");
const std::string EXCHANGE_TYPE("exchange-type");
const std::string QUEUE_NAME("queue");
const std::string EXCHANGE_NAME("exchange");

const std::string ATTRIBUTE_TIMESTAMP_0_10("timestamp-0.10");

const std::string _TRUE("true");
const std::string _FALSE("false");

// parameters for creating a Link object, see mgmt schema
const std::string HOST("host");
const std::string PORT("port");
const std::string TRANSPORT("transport");
const std::string AUTH_MECHANISM("authMechanism");
const std::string USERNAME("username");
const std::string PASSWORD("password");

// parameters for creating a Bridge object, see mgmt schema
const std::string LINK("link");
const std::string SRC("src");
const std::string DEST("dest");
const std::string KEY("key");
const std::string TAG("tag");
const std::string EXCLUDES("excludes");
const std::string SRC_IS_QUEUE("srcIsQueue");
const std::string SRC_IS_LOCAL("srcIsLocal");
const std::string DYNAMIC("dynamic");
const std::string SYNC("sync");
<<<<<<< HEAD
=======
const std::string CREDIT("credit");

// parameters for deleting a Queue object
const std::string IF_EMPTY("if_empty");
const std::string IF_UNUSED("if_unused");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

struct InvalidBindingIdentifier : public qpid::Exception
{
    InvalidBindingIdentifier(const std::string& name) : qpid::Exception(name) {}
    std::string getPrefix() const { return "invalid binding"; }
};

struct BindingIdentifier
{
    std::string exchange;
    std::string queue;
    std::string key;

    BindingIdentifier(const std::string& name)
    {
        std::vector<std::string> path;
        split(path, name, "/");
        switch (path.size()) {
          case 1:
            queue = path[0];
            break;
          case 2:
            exchange = path[0];
            queue = path[1];
            break;
          case 3:
            exchange = path[0];
            queue = path[1];
            key = path[2];
            break;
          default:
            throw InvalidBindingIdentifier(name);
        }
    }
};

struct ObjectAlreadyExists : public qpid::Exception
{
    ObjectAlreadyExists(const std::string& name) : qpid::Exception(name) {}
    std::string getPrefix() const { return "object already exists"; }
};

struct UnknownObjectType : public qpid::Exception
{
    UnknownObjectType(const std::string& type) : qpid::Exception(type) {}
    std::string getPrefix() const { return "unknown object type"; }
};

struct ReservedObjectName : public qpid::Exception
{
    ReservedObjectName(const std::string& type) : qpid::Exception(type) {}
    std::string getPrefix() const { return std::string("names prefixed with '")
          + QPID_NAME_PREFIX + std::string("' are reserved"); }
};

struct UnsupportedTransport : public qpid::Exception
{
    UnsupportedTransport(const std::string& type) : qpid::Exception(type) {}
    std::string getPrefix() const { return "transport is not supported"; }
};

struct InvalidParameter : public qpid::Exception
{
    InvalidParameter(const std::string& type) : qpid::Exception(type) {}
    std::string getPrefix() const { return "invalid parameter to method call"; }
};

void Broker::createObject(const std::string& type, const std::string& name,
<<<<<<< HEAD
                          const Variant::Map& properties, bool /*strict*/, const ConnectionState* context)
=======
                          const Variant::Map& properties, bool /*strict*/, const Connection* context)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::string userId;
    std::string connectionId;
    if (context) {
        userId = context->getUserId();
<<<<<<< HEAD
        connectionId = context->getUrl();
    }
    //TODO: implement 'strict' option (check there are no unrecognised properties)
    QPID_LOG (debug, "Broker::create(" << type << ", " << name << "," << properties << ")");
    if (type == TYPE_QUEUE) {
=======
        connectionId = context->getMgmtId();
    }
    //TODO: implement 'strict' option (check there are no unrecognised properties)
    QPID_LOG (debug, "Broker::create(" << type << ", " << name << "," << properties << ")");
    if (objectFactory.createObject(*this, type, name, properties, userId, connectionId)) {
        QPID_LOG (debug, "Broker::create(" << type << ", " << name << "," << properties << ") handled by registered factory");
    } else if (type == TYPE_QUEUE) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        bool durable(false);
        bool autodelete(false);
        std::string alternateExchange;
        Variant::Map extensions;
        for (Variant::Map::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            // extract durable, auto-delete and alternate-exchange properties
            if (i->first == DURABLE) durable = i->second;
            else if (i->first == AUTO_DELETE) autodelete = i->second;
            else if (i->first == ALTERNATE_EXCHANGE) alternateExchange = i->second.asString();
            //treat everything else as extension properties
            else extensions[i->first] = i->second;
        }
<<<<<<< HEAD
        framing::FieldTable arguments;
        amqp_0_10::translate(extensions, arguments);

        std::pair<boost::shared_ptr<Queue>, bool> result =
            createQueue(name, durable, autodelete, 0, alternateExchange, arguments, userId, connectionId);
=======
        QueueSettings settings(durable, autodelete);
        Variant::Map unused;
        settings.populate(extensions, unused);
        qpid::amqp_0_10::translate(unused, settings.storeSettings);
        //TODO: unused doesn't take store settings into account... so can't yet implement strict
        QPID_LOG(debug, "Broker did not use the following settings (store module may): " << unused);

        std::pair<boost::shared_ptr<Queue>, bool> result =
            createQueue(name, settings, 0, alternateExchange, userId, connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (!result.second) {
            throw ObjectAlreadyExists(name);
        }
    } else if (type == TYPE_EXCHANGE || type == TYPE_TOPIC) {
        bool durable(false);
<<<<<<< HEAD
=======
        bool autodelete(false);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        std::string exchangeType("topic");
        std::string alternateExchange;
        Variant::Map extensions;
        for (Variant::Map::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            // extract durable, auto-delete and alternate-exchange properties
            if (i->first == DURABLE) durable = i->second;
<<<<<<< HEAD
=======
            else if (i->first == AUTO_DELETE) autodelete = i->second;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            else if (i->first == EXCHANGE_TYPE) exchangeType = i->second.asString();
            else if (i->first == ALTERNATE_EXCHANGE) alternateExchange = i->second.asString();
            //treat everything else as extension properties
            else extensions[i->first] = i->second;
        }
        framing::FieldTable arguments;
<<<<<<< HEAD
        amqp_0_10::translate(extensions, arguments);

        try {
            std::pair<boost::shared_ptr<Exchange>, bool> result =
                createExchange(name, exchangeType, durable, alternateExchange, arguments, userId, connectionId);
=======
        qpid::amqp_0_10::translate(extensions, arguments);

        try {
            std::pair<boost::shared_ptr<Exchange>, bool> result =
                createExchange(name, exchangeType, durable, autodelete, alternateExchange, arguments, userId, connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            if (!result.second) {
                throw ObjectAlreadyExists(name);
            }
        } catch (const UnknownExchangeTypeException&) {
            throw Exception(QPID_MSG("Invalid exchange type: " << exchangeType));
        }
    } else if (type == TYPE_BINDING) {
        BindingIdentifier binding(name);
        std::string exchangeType("topic");
        Variant::Map extensions;
        for (Variant::Map::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            // extract durable, auto-delete and alternate-exchange properties
            if (i->first == EXCHANGE_TYPE) exchangeType = i->second.asString();
            //treat everything else as extension properties
            else extensions[i->first] = i->second;
        }
        framing::FieldTable arguments;
<<<<<<< HEAD
        amqp_0_10::translate(extensions, arguments);

        bind(binding.queue, binding.exchange, binding.key, arguments, userId, connectionId);
=======
        qpid::amqp_0_10::translate(extensions, arguments);

        bind(binding.queue, binding.exchange, binding.key, arguments, 0, userId, connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    } else if (type == TYPE_LINK) {

        QPID_LOG (debug, "createObject: Link; name=" << name << "; args=" << properties );

        if (name.compare(0, QPID_NAME_PREFIX.length(), QPID_NAME_PREFIX) == 0) {
            QPID_LOG(error, "Link name='" << name << "' cannot use the reserved prefix '" << QPID_NAME_PREFIX << "'");
            throw ReservedObjectName(name);
        }

        std::string host;
        uint16_t port = 0;
        std::string transport = TCP_TRANSPORT;
        bool durable = false;
        std::string authMech, username, password;

        for (Variant::Map::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            if (i->first == HOST) host = i->second.asString();
            else if (i->first == PORT) port = i->second.asUint16();
            else if (i->first == TRANSPORT) transport = i->second.asString();
            else if (i->first == DURABLE) durable = bool(i->second);
            else if (i->first == AUTH_MECHANISM) authMech = i->second.asString();
            else if (i->first == USERNAME) username = i->second.asString();
            else if (i->first == PASSWORD) password = i->second.asString();
            else {
                // TODO: strict checking here
            }
        }

<<<<<<< HEAD
        if (!getProtocolFactory(transport)) {
=======
        if (!getTransportInfo(transport).connectorFactory) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            QPID_LOG(error, "Transport '" << transport << "' not supported.");
            throw UnsupportedTransport(transport);
        }

        std::pair<boost::shared_ptr<Link>, bool> rc;
        rc = links.declare(name, host, port, transport, durable, authMech, username, password);
        if (!rc.first) {
            QPID_LOG (error, "Failed to create Link object, name=" << name << " remote=" << host << ":" << port <<
                      "; transport=" << transport << "; durable=" << (durable?"T":"F") << "; authMech=\"" << authMech << "\"");
            throw InvalidParameter(name);
        }
        if (!rc.second) {
            QPID_LOG (error, "Failed to create a new Link object, name=" << name << " already exists.");
            throw ObjectAlreadyExists(name);
        }

    } else if (type == TYPE_BRIDGE) {

        QPID_LOG (debug, "createObject: Bridge; name=" << name << "; args=" << properties );

        if (name.compare(0, QPID_NAME_PREFIX.length(), QPID_NAME_PREFIX) == 0) {
            QPID_LOG(error, "Bridge name='" << name << "' cannot use the reserved prefix '" << QPID_NAME_PREFIX << "'");
            throw ReservedObjectName(name);
        }

        std::string linkName;
        std::string src;
        std::string dest;
        std::string key;
        std::string id;
        std::string excludes;
        std::string queueName;
        bool durable = false;
        bool srcIsQueue = false;
        bool srcIsLocal = false;
        bool dynamic = false;
        uint16_t sync = 0;
<<<<<<< HEAD
=======
        uint32_t credit = LinkRegistry::INFINITE_CREDIT;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        for (Variant::Map::const_iterator i = properties.begin(); i != properties.end(); ++i) {

            if (i->first == LINK) linkName = i->second.asString();
            else if (i->first == SRC) src = i->second.asString();
            else if (i->first == DEST) dest = i->second.asString();
            else if (i->first == KEY) key = i->second.asString();
            else if (i->first == TAG) id = i->second.asString();
            else if (i->first == EXCLUDES) excludes = i->second.asString();
            else if (i->first == SRC_IS_QUEUE) srcIsQueue = bool(i->second);
            else if (i->first == SRC_IS_LOCAL) srcIsLocal = bool(i->second);
            else if (i->first == DYNAMIC) dynamic = bool(i->second);
            else if (i->first == SYNC) sync = i->second.asUint16();
<<<<<<< HEAD
=======
            else if (i->first == CREDIT) credit = i->second.asUint32();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            else if (i->first == DURABLE) durable = bool(i->second);
            else if (i->first == QUEUE_NAME) queueName = i->second.asString();
            else {
                // TODO: strict checking here
            }
        }

        boost::shared_ptr<Link> link;
        if (linkName.empty() || !(link = links.getLink(linkName))) {
            QPID_LOG(error, "Link '" << linkName << "' not found; bridge create failed.");
            throw InvalidParameter(name);
        }
        std::pair<Bridge::shared_ptr, bool> rc =
          links.declare(name, *link, durable, src, dest, key, srcIsQueue, srcIsLocal, id, excludes,
<<<<<<< HEAD
                        dynamic, sync,
=======
                        dynamic, sync, credit,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                        0,
                        queueName);

        if (!rc.first) {
            QPID_LOG (error, "Failed to create Bridge object, name=" << name << " link=" << linkName <<
                      "; src=" << src << "; dest=" << dest << "; key=" << key);
            throw InvalidParameter(name);
        }
        if (!rc.second) {
            QPID_LOG (error, "Failed to create a new Bridge object, name=" << name << " already exists.");
            throw ObjectAlreadyExists(name);
        }
    } else {
        throw UnknownObjectType(type);
    }
}

void Broker::deleteObject(const std::string& type, const std::string& name,
<<<<<<< HEAD
                          const Variant::Map& options, const ConnectionState* context)
=======
                          const Variant::Map& options, const Connection* context)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::string userId;
    std::string connectionId;
    if (context) {
        userId = context->getUserId();
<<<<<<< HEAD
        connectionId = context->getUrl();
    }
    QPID_LOG (debug, "Broker::delete(" << type << ", " << name << "," << options << ")");
    if (type == TYPE_QUEUE) {
        deleteQueue(name, userId, connectionId);
=======
        connectionId = context->getMgmtId();
    }
    QPID_LOG (debug, "Broker::delete(" << type << ", " << name << "," << options << ")");
    if (objectFactory.deleteObject(*this, type, name, options, userId, connectionId)) {
        QPID_LOG (debug, "Broker::delete(" << type << ", " << name << "," << options << ") handled by registered factory");
    } else if (type == TYPE_QUEUE) {
        // extract ifEmpty and ifUnused from options
    	bool ifUnused = false, ifEmpty = false;
        for (Variant::Map::const_iterator i = options.begin(); i != options.end(); ++i) {
            if (i->first == IF_UNUSED) ifUnused = i->second.asBool();
            else if (i->first == IF_EMPTY) ifEmpty = i->second.asBool();
        }
        deleteQueue(name, userId, connectionId,
                    boost::bind(&Broker::checkDeleteQueue, this, _1, ifUnused, ifEmpty));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else if (type == TYPE_EXCHANGE || type == TYPE_TOPIC) {
        deleteExchange(name, userId, connectionId);
    } else if (type == TYPE_BINDING) {
        BindingIdentifier binding(name);
<<<<<<< HEAD
        unbind(binding.queue, binding.exchange, binding.key, userId, connectionId);
=======
        unbind(binding.queue, binding.exchange, binding.key, 0, userId, connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else if (type == TYPE_LINK) {
        boost::shared_ptr<Link> link = links.getLink(name);
        if (link) {
            link->close();
        }
    } else if (type == TYPE_BRIDGE) {
        boost::shared_ptr<Bridge> bridge = links.getBridge(name);
        if (bridge) {
            bridge->close();
        }
    } else {
        throw UnknownObjectType(type);
    }
<<<<<<< HEAD

=======
}

void Broker::checkDeleteQueue(Queue::shared_ptr queue, bool ifUnused, bool ifEmpty)
{
    if(ifEmpty && queue->getMessageCount() > 0) {
        throw qpid::framing::PreconditionFailedException(QPID_MSG("Cannot delete queue "
                                                                  << queue->getName() << "; queue not empty"));
    } else if(ifUnused && queue->getConsumerCount() > 0) {
        throw qpid::framing::PreconditionFailedException(QPID_MSG("Cannot delete queue "
                                                                  << queue->getName() << "; queue in use"));
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

Manageable::status_t Broker::queryObject(const std::string& type,
                                         const std::string& name,
                                         Variant::Map& results,
<<<<<<< HEAD
                                         const ConnectionState* context)
=======
                                         const Connection* context)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::string userId;
    std::string connectionId;
    if (context) {
        userId = context->getUserId();
<<<<<<< HEAD
        connectionId = context->getUrl();
=======
        connectionId = context->getMgmtId();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    QPID_LOG (debug, "Broker::query(" << type << ", " << name << ")");

    if (type == TYPE_QUEUE)
        return queryQueue( name, userId, connectionId, results );

    if (type == TYPE_EXCHANGE ||
        type == TYPE_TOPIC ||
        type == TYPE_BINDING)
        return Manageable::STATUS_NOT_IMPLEMENTED;

    throw UnknownObjectType(type);
}

Manageable::status_t Broker::queryQueue( const std::string& name,
                                         const std::string& userId,
                                         const std::string& /*connectionId*/,
                                         Variant::Map& results )
{
    (void) results;
    if (acl) {
        if (!acl->authorise(userId, acl::ACT_ACCESS, acl::OBJ_QUEUE, name, NULL) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue query request from " << userId));
    }

    boost::shared_ptr<Queue> q(queues.find(name));
    if (!q) {
        QPID_LOG(error, "Query failed: queue not found, name=" << name);
        return Manageable::STATUS_UNKNOWN_OBJECT;
    }
    q->query( results );
    return Manageable::STATUS_OK;;
}

Manageable::status_t Broker::getTimestampConfig(bool& receive,
<<<<<<< HEAD
                                                const ConnectionState* context)
=======
                                                const Connection* context)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::string name;   // none needed for broker
    std::string userId = context->getUserId();
    if (acl && !acl->authorise(userId, acl::ACT_ACCESS, acl::OBJ_BROKER, name, NULL))  {
        throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied broker timestamp get request from " << userId));
    }
<<<<<<< HEAD
    receive = config.timestampRcvMsgs;
=======
    receive = timestampRcvMsgs;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    return Manageable::STATUS_OK;
}

Manageable::status_t Broker::setTimestampConfig(const bool receive,
<<<<<<< HEAD
                                                const ConnectionState* context)
=======
                                                const Connection* context)
>>>>>>> 3bbfc42... Imported Upstream version 0.32
{
    std::string name;   // none needed for broker
    std::string userId = context->getUserId();
    if (acl && !acl->authorise(userId, acl::ACT_UPDATE, acl::OBJ_BROKER, name, NULL)) {
        throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied broker timestamp set request from " << userId));
    }
<<<<<<< HEAD
    config.timestampRcvMsgs = receive;
    QPID_LOG(notice, "Receive message timestamping is " << ((config.timestampRcvMsgs) ? "ENABLED." : "DISABLED."));
=======
    timestampRcvMsgs = receive;
    QPID_LOG(notice, "Receive message timestamping is " << ((timestampRcvMsgs) ? "ENABLED." : "DISABLED."));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    return Manageable::STATUS_OK;
}

void Broker::setLogLevel(const std::string& level)
{
    QPID_LOG(notice, "Changing log level to " << level);
    std::vector<std::string> selectors;
    split(selectors, level, ", ");
    qpid::log::Logger::instance().reconfigure(selectors);
}

std::string Broker::getLogLevel()
{
    std::string level;
<<<<<<< HEAD
    const std::vector<std::string>& selectors = qpid::log::Logger::instance().getOptions().selectors;
    for (std::vector<std::string>::const_iterator i = selectors.begin(); i != selectors.end(); ++i) {
        if (i != selectors.begin()) level += std::string(",");
        level += *i;
=======
    std::string sep("");
    const std::vector<std::string>& selectors = qpid::log::Logger::instance().getOptions().selectors;
    for (std::vector<std::string>::const_iterator i = selectors.begin(); i != selectors.end(); ++i) {
        level += sep + *i;
        sep = ",";
    }
    const std::vector<std::string>& disselectors = qpid::log::Logger::instance().getOptions().deselectors;
    for (std::vector<std::string>::const_iterator i = disselectors.begin(); i != disselectors.end(); ++i) {
        level += sep + "!" + *i;
        sep = ",";
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    return level;
}

<<<<<<< HEAD
boost::shared_ptr<ProtocolFactory> Broker::getProtocolFactory(const std::string& name) const {
    ProtocolFactoryMap::const_iterator i
        = name.empty() ? protocolFactories.begin() : protocolFactories.find(name);
    if (i == protocolFactories.end()) return boost::shared_ptr<ProtocolFactory>();
=======
void Broker::setLogHiresTimestamp(bool enabled)
{
    QPID_LOG(notice, "Changing log hires timestamp to " << enabled);
    qpid::log::Logger::instance().setHiresTimestamp(enabled);
}

bool Broker::getLogHiresTimestamp()
{
    return qpid::log::Logger::instance().getHiresTimestamp();
}


Manageable::status_t Broker::queueRedirect(const std::string& srcQueue,
                                           const std::string& tgtQueue,
                                           const Connection* context)
{
    Queue::shared_ptr srcQ(queues.find(srcQueue));
    if (!srcQ) {
        QPID_LOG(error, "Queue redirect failed: source queue not found: "
            << srcQueue);
        return Manageable::STATUS_UNKNOWN_OBJECT;
    }

    if (!tgtQueue.empty()) {
        // NonBlank target queue creates partnership
        Queue::shared_ptr tgtQ(queues.find(tgtQueue));
        if (!tgtQ) {
            QPID_LOG(error, "Queue redirect failed: target queue not found: "
                << tgtQueue);
            return Manageable::STATUS_UNKNOWN_OBJECT;
        }

        if (srcQueue.compare(tgtQueue) == 0) {
            QPID_LOG(error, "Queue redirect source queue: "
                << tgtQueue << " cannot be its own target");
            return Manageable::STATUS_USER;
        }

        if (srcQ->isAutoDelete()) {
            QPID_LOG(error, "Queue redirect source queue: "
                << srcQueue << " is autodelete and can not be part of redirect");
            return Manageable::STATUS_USER;
        }

        if (tgtQ->isAutoDelete()) {
            QPID_LOG(error, "Queue redirect target queue: "
                << tgtQueue << " is autodelete and can not be part of redirect");
            return Manageable::STATUS_USER;
        }

        if (srcQ->getRedirectPeer()) {
            QPID_LOG(error, "Queue redirect source queue: "
                << srcQueue << " is already redirected");
            return Manageable::STATUS_USER;
        }

        if (tgtQ->getRedirectPeer()) {
            QPID_LOG(error, "Queue redirect target queue: "
                << tgtQueue << " is already redirected");
            return Manageable::STATUS_USER;
        }

        if (acl) {
            std::map<acl::Property, std::string> params;
            params.insert(make_pair(acl::PROP_QUEUENAME, tgtQ->getName()));
            if (!acl->authorise((context)?context->getUserId():"", acl::ACT_REDIRECT, acl::OBJ_QUEUE, srcQ->getName(), &params))
                throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied redirect request from " << ((context)?context->getUserId():"(uknown)")));
        }

        // Start the backup overflow partnership
        srcQ->setRedirectPeer(tgtQ, true);
        tgtQ->setRedirectPeer(srcQ, false);

        // Set management state
        srcQ->setMgmtRedirectState(tgtQueue, true, true);
        tgtQ->setMgmtRedirectState(srcQueue, true, false);

        // Management event
        if (managementAgent.get()) {
            managementAgent->raiseEvent(_qmf::EventQueueRedirect(srcQueue, tgtQueue));
        }

        QPID_LOG(info, "Queue redirect complete. queue: "
            << srcQueue << " target queue: " << tgtQueue);
        return Manageable::STATUS_OK;
    } else {
        // Blank target queue destroys partnership
        Queue::shared_ptr tgtQ(srcQ->getRedirectPeer());
        if (!tgtQ) {
            QPID_LOG(error, "Queue redirect source queue: "
                << srcQueue << " is not in redirected");
            return Manageable::STATUS_USER;
        }

        if (!srcQ->isRedirectSource()) {
            QPID_LOG(error, "Queue redirect source queue: "
                << srcQueue << " is not a redirect source");
            return Manageable::STATUS_USER;
        }

        if (acl) {
            std::map<acl::Property, std::string> params;
            params.insert(make_pair(acl::PROP_QUEUENAME, tgtQ->getName()));
            if (!acl->authorise((context)?context->getUserId():"", acl::ACT_REDIRECT, acl::OBJ_QUEUE, srcQ->getName(), &params))
                throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied redirect request from " << ((context)?context->getUserId():"(uknown)")));
        }

        queueRedirectDestroy(srcQ, tgtQ, true);

        return Manageable::STATUS_OK;
    }
}


void Broker::queueRedirectDestroy(Queue::shared_ptr srcQ,
                                  Queue::shared_ptr tgtQ,
                                  bool moveMsgs) {
    QPID_LOG(notice, "Queue redirect destroyed. queue: " << srcQ->getName()
        << " target queue: " << tgtQ->getName());

    tgtQ->setMgmtRedirectState(empty, false, false);
    srcQ->setMgmtRedirectState(empty, false, false);

    if (moveMsgs) {
        // TODO: this 'move' works in the static case but has no
        // actual locking that does what redirect needs when
        // there is a lot of traffic in flight.
        tgtQ->move(srcQ, 0);
    }

    Queue::shared_ptr np;

    tgtQ->setRedirectPeer(np, false);
    srcQ->setRedirectPeer(np, false);

    if (managementAgent.get()) {
        managementAgent->raiseEvent(_qmf::EventQueueRedirectCancelled(srcQ->getName(), tgtQ->getName()));
    }
}

const Broker::TransportInfo& Broker::getTransportInfo(const std::string& name) const {
    static TransportInfo nullTransportInfo;
    TransportMap::const_iterator i
        = name.empty() ? transportMap.begin() : transportMap.find(name);
    if (i == transportMap.end()) return nullTransportInfo;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    else return i->second;
}

uint16_t Broker::getPort(const std::string& name) const  {
<<<<<<< HEAD
    boost::shared_ptr<ProtocolFactory> factory = getProtocolFactory(name);
    if (factory) {
        return factory->getPort();
=======
    if (int p = getTransportInfo(name).port) {
        return p;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else {
        throw NoSuchTransportException(QPID_MSG("No such transport: '" << name << "'"));
    }
}

<<<<<<< HEAD
void Broker::registerProtocolFactory(const std::string& name, ProtocolFactory::shared_ptr protocolFactory) {
    protocolFactories[name] = protocolFactory;
=======
bool Broker::shouldListen(std::string transport) {
    return disabledListeningTransports.count(transport)==0;
}

void Broker::disableListening(std::string transport) {
    disabledListeningTransports.insert(transport);
}

void Broker::registerTransport(const std::string& name, boost::shared_ptr<TransportAcceptor> a, boost::shared_ptr<TransportConnector> c, uint16_t p) {
    transportMap[name] = TransportInfo(a, c, p);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    Url::addProtocol(name);
}

void Broker::accept() {
<<<<<<< HEAD
    for (ProtocolFactoryMap::const_iterator i = protocolFactories.begin(); i != protocolFactories.end(); i++) {
        i->second->accept(poller, factory.get());
=======
    unsigned accepting = 0;
    for (TransportMap::const_iterator i = transportMap.begin(); i != transportMap.end(); i++) {
        if (i->second.acceptor) {
            i->second.acceptor->accept(poller, &protocolRegistry);
            ++accepting;
        }
    }
    if ( accepting==0 ) {
        throw Exception(QPID_MSG("Failed to start broker: No transports are listening for incoming connections"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}

void Broker::connect(
<<<<<<< HEAD
    const std::string& host, const std::string& port, const std::string& transport,
    boost::function2<void, int, std::string> failed,
    sys::ConnectionCodec::Factory* f)
{
    boost::shared_ptr<ProtocolFactory> pf = getProtocolFactory(transport);
    if (pf) pf->connect(poller, host, port, f ? f : factory.get(), failed);
    else throw NoSuchTransportException(QPID_MSG("Unsupported transport type: " << transport));
}

void Broker::connect(
    const Url& url,
    boost::function2<void, int, std::string> failed,
    sys::ConnectionCodec::Factory* f)
{
    url.throwIfEmpty();
    const Address& addr=url[0];
    connect(addr.host, boost::lexical_cast<std::string>(addr.port), addr.protocol, failed, f);
}

uint32_t Broker::queueMoveMessages(
     const std::string& srcQueue,
     const std::string& destQueue,
     uint32_t  qty,
     const Variant::Map& filter)
{
  Queue::shared_ptr src_queue = queues.find(srcQueue);
  if (!src_queue)
    return 0;
  Queue::shared_ptr dest_queue = queues.find(destQueue);
  if (!dest_queue)
    return 0;

  return src_queue->move(dest_queue, qty, &filter);
=======
    const std::string& name,
    const std::string& host, const std::string& port, const std::string& transport,
    boost::function2<void, int, std::string> failed)
{
    connect(name, host, port, transport, &protocolRegistry, failed);
}

void Broker::connect(
    const std::string& name,
    const std::string& host, const std::string& port, const std::string& transport,
    sys::ConnectionCodec::Factory* f, boost::function2<void, int, std::string> failed)
{
    boost::shared_ptr<TransportConnector> tcf = getTransportInfo(transport).connectorFactory;
    if (tcf) tcf->connect(poller, name, host, port, f, failed);
    else throw NoSuchTransportException(QPID_MSG("Unsupported transport type: " << transport));
}

int32_t Broker::queueMoveMessages(
     const std::string& srcQueue,
     const std::string& destQueue,
     uint32_t  qty,
     const Variant::Map& filter,
     const Connection* context)
{
    Queue::shared_ptr src_queue = queues.find(srcQueue);
    if (!src_queue)
        return -1;
    Queue::shared_ptr dest_queue = queues.find(destQueue);
    if (!dest_queue)
        return -1;

    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_QUEUENAME, dest_queue->getName()));
        if (!acl->authorise((context)?context->getUserId():"", acl::ACT_MOVE, acl::OBJ_QUEUE, src_queue->getName(), &params))
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied move request from " << ((context)?context->getUserId():"(uknown)")));
    }

    return (int32_t) src_queue->move(dest_queue, qty, &filter);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}


boost::shared_ptr<sys::Poller> Broker::getPoller() { return poller; }

std::vector<Url>
Broker::getKnownBrokersImpl()
{
    return knownBrokers;
}

<<<<<<< HEAD
bool Broker::deferDeliveryImpl(const std::string& ,
                               const boost::intrusive_ptr<Message>& )
{ return false; }

void Broker::setClusterTimer(std::auto_ptr<sys::Timer> t) {
    clusterTimer = t;
    queueCleaner.setTimer(clusterTimer.get());
    dtxManager.setTimer(*clusterTimer.get());
}

const std::string Broker::TCP_TRANSPORT("tcp");


std::pair<boost::shared_ptr<Queue>, bool> Broker::createQueue(
    const std::string& name,
    bool durable,
    bool autodelete,
    const OwnershipToken* owner,
    const std::string& alternateExchange,
    const qpid::framing::FieldTable& arguments,
    const std::string& userId,
    const std::string& connectionId)
{
    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_ALTERNATE, alternateExchange));
        params.insert(make_pair(acl::PROP_DURABLE, durable ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_EXCLUSIVE, owner ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_AUTODELETE, autodelete ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_POLICYTYPE, arguments.getAsString("qpid.policy_type")));
        params.insert(make_pair(acl::PROP_MAXQUEUECOUNT, boost::lexical_cast<string>(arguments.getAsInt("qpid.max_count"))));
        params.insert(make_pair(acl::PROP_MAXQUEUESIZE, boost::lexical_cast<string>(arguments.getAsInt64("qpid.max_size"))));

        if (!acl->authorise(userId,acl::ACT_CREATE,acl::OBJ_QUEUE,name,&params) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue create request from " << userId));
=======
bool Broker::deferDeliveryImpl(const std::string&, const Message&)
{ return false; }

const std::string Broker::TCP_TRANSPORT("tcp");

std::pair<boost::shared_ptr<Queue>, bool> Broker::createQueue(
    const std::string& name,
    const QueueSettings& constSettings,
    const OwnershipToken* owner,
    const std::string& alternateExchange,
    const std::string& userId,
    const std::string& connectionId)
{
    QueueSettings settings(constSettings); // So we can modify them
    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_ALTERNATE, alternateExchange));
        params.insert(make_pair(acl::PROP_DURABLE, settings.durable ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_EXCLUSIVE, owner ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_AUTODELETE, settings.autodelete ? _TRUE : _FALSE));
        params.insert(make_pair(acl::PROP_POLICYTYPE, settings.getLimitPolicy()));
        params.insert(make_pair(acl::PROP_PAGING, settings.paging ? _TRUE : _FALSE));
        if (settings.paging) {
            params.insert(make_pair(acl::PROP_MAXPAGES, boost::lexical_cast<string>(settings.maxPages ? settings.maxPages : DEFAULT_MAX_PAGES)));
            params.insert(make_pair(acl::PROP_MAXPAGEFACTOR, boost::lexical_cast<string>(settings.pageFactor ? settings.pageFactor : DEFAULT_PAGE_FACTOR)));
        }
        if (settings.maxDepth.hasCount())
            params.insert(make_pair(acl::PROP_MAXQUEUECOUNT, boost::lexical_cast<string>(settings.maxDepth.getCount() ? settings.maxDepth.getCount() : std::numeric_limits<uint64_t>::max())));
        if (settings.maxDepth.hasSize())
            params.insert(make_pair(acl::PROP_MAXQUEUESIZE, boost::lexical_cast<string>(settings.maxDepth.getSize() ? settings.maxDepth.getSize() :  std::numeric_limits<uint64_t>::max())));
        else
            params.insert(make_pair(acl::PROP_MAXQUEUESIZE, boost::lexical_cast<string>(config.queueLimit)));
        if (settings.durable) {
            params.insert(make_pair(acl::PROP_MAXFILECOUNT, boost::lexical_cast<string>(settings.maxFileCount ? settings.maxFileCount : 8)));
            params.insert(make_pair(acl::PROP_MAXFILESIZE, boost::lexical_cast<string>(settings.maxFileSize ? settings.maxFileSize : 24)));
        }

        if (!acl->authorise(userId,acl::ACT_CREATE,acl::OBJ_QUEUE,name,&params) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue create request from " << userId));

        if (!queues.find(name))
            if (!acl->approveCreateQueue(userId,name) )
                throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue create request from " << userId));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }

    Exchange::shared_ptr alternate;
    if (!alternateExchange.empty()) {
        alternate = exchanges.get(alternateExchange);
        if (!alternate) throw framing::NotFoundException(QPID_MSG("Alternate exchange does not exist: " << alternateExchange));
    }

<<<<<<< HEAD
    std::pair<Queue::shared_ptr, bool> result = queues.declare(name, durable, autodelete, owner, alternate, arguments);
    if (result.second) {
        //add default binding:
        result.first->bind(exchanges.getDefault(), name);

        if (managementAgent.get()) {
            //TODO: debatable whether we should raise an event here for
            //create when this is a 'declare' event; ideally add a create
            //event instead?
            managementAgent->raiseEvent(
                _qmf::EventQueueDeclare(connectionId, userId, name,
                                        durable, owner, autodelete, alternateExchange,
                                        ManagementAgent::toMap(arguments),
                                        "created"));
        }
=======
    // Identify queues that won't survive a failover: exclusive, auto-delete with no delay.
    if (owner && settings.autodelete && !settings.autoDeleteDelay)
        settings.isTemporary = true;

    std::pair<Queue::shared_ptr, bool> result =
        queues.declare(name, settings, alternate, false/*recovering*/,
                       owner, connectionId, userId);
    if (result.second) {
        //add default binding:
        result.first->bind(exchanges.getDefault(), name);
        QPID_LOG_CAT(debug, model, "Create queue. name:" << name
            << " user:" << userId
            << " rhost:" << connectionId
            << " durable:" << (settings.durable ? "T" : "F")
            << " owner:" << owner
            << " autodelete:" << (settings.autodelete ? "T" : "F")
            << " alternateExchange:" << alternateExchange );
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    return result;
}

void Broker::deleteQueue(const std::string& name, const std::string& userId,
                         const std::string& connectionId, QueueFunctor check)
{
<<<<<<< HEAD
    if (acl && !acl->authorise(userId,acl::ACT_DELETE,acl::OBJ_QUEUE,name,NULL)) {
        throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue delete request from " << userId));
    }

    Queue::shared_ptr queue = queues.find(name);
    if (queue) {
        if (check) check(queue);
        queues.destroy(name);
=======
    QPID_LOG_CAT(debug, model, "Deleting queue. name:" << name
                 << " user:" << userId
                 << " rhost:" << connectionId
    );
    Queue::shared_ptr queue = queues.find(name);
    if (queue) {
        if (acl) {
            std::map<acl::Property, std::string> params;
            boost::shared_ptr<Exchange> altEx = queue->getAlternateExchange();
            params.insert(make_pair(acl::PROP_ALTERNATE, (altEx) ? altEx->getName() : "" ));
            params.insert(make_pair(acl::PROP_DURABLE, queue->isDurable() ? _TRUE : _FALSE));
            params.insert(make_pair(acl::PROP_EXCLUSIVE, queue->hasExclusiveOwner() ? _TRUE : _FALSE));
            params.insert(make_pair(acl::PROP_AUTODELETE, queue->isAutoDelete() ? _TRUE : _FALSE));
            params.insert(make_pair(acl::PROP_POLICYTYPE, queue->getSettings().getLimitPolicy()));

            if (!acl->authorise(userId,acl::ACT_DELETE,acl::OBJ_QUEUE,name,&params) )
                throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied queue delete request from " << userId));
        }
        if (check) check(queue);
        if (acl)
            acl->recordDestroyQueue(name);
        Queue::shared_ptr peerQ(queue->getRedirectPeer());
        if (peerQ)
            queueRedirectDestroy(queue->isRedirectSource() ? queue : peerQ,
                                 queue->isRedirectSource() ? peerQ : queue,
                                 false);
        queues.destroy(name, connectionId, userId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        queue->destroyed();
    } else {
        throw framing::NotFoundException(QPID_MSG("Delete failed. No such queue: " << name));
    }
<<<<<<< HEAD

    if (managementAgent.get())
        managementAgent->raiseEvent(_qmf::EventQueueDelete(connectionId, userId, name));

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

std::pair<Exchange::shared_ptr, bool> Broker::createExchange(
    const std::string& name,
    const std::string& type,
    bool durable,
<<<<<<< HEAD
=======
    bool autodelete,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    const std::string& alternateExchange,
    const qpid::framing::FieldTable& arguments,
    const std::string& userId,
    const std::string& connectionId)
{
    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_TYPE, type));
        params.insert(make_pair(acl::PROP_ALTERNATE, alternateExchange));
        params.insert(make_pair(acl::PROP_DURABLE, durable ? _TRUE : _FALSE));
<<<<<<< HEAD
=======
        params.insert(make_pair(acl::PROP_AUTODELETE, autodelete ? _TRUE : _FALSE));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (!acl->authorise(userId,acl::ACT_CREATE,acl::OBJ_EXCHANGE,name,&params) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied exchange create request from " << userId));
    }

    Exchange::shared_ptr alternate;
    if (!alternateExchange.empty()) {
        alternate = exchanges.get(alternateExchange);
        if (!alternate) throw framing::NotFoundException(QPID_MSG("Alternate exchange does not exist: " << alternateExchange));
    }

    std::pair<Exchange::shared_ptr, bool> result;
<<<<<<< HEAD
    result = exchanges.declare(name, type, durable, arguments);
    if (result.second) {
        if (alternate) {
            result.first->setAlternate(alternate);
            alternate->incAlternateUsers();
        }
        if (durable) {
            store->create(*result.first, arguments);
        }
        if (managementAgent.get()) {
            //TODO: debatable whether we should raise an event here for
            //create when this is a 'declare' event; ideally add a create
            //event instead?
            managementAgent->raiseEvent(_qmf::EventExchangeDeclare(connectionId,
                                                         userId,
                                                         name,
                                                         type,
                                                         alternateExchange,
                                                         durable,
                                                         false,
                                                         ManagementAgent::toMap(arguments),
                                                         "created"));
        }
=======
    result = exchanges.declare(
        name, type, durable, autodelete, arguments, alternate, connectionId, userId);
    if (result.second) {
        if (durable) {
            store->create(*result.first, arguments);
        }
        QPID_LOG_CAT(debug, model, "Create exchange. name:" << name
            << " user:" << userId
            << " rhost:" << connectionId
            << " type:" << type
            << " alternateExchange:" << alternateExchange
            << " durable:" << (durable ? "T" : "F")
            << " autodelete:" << (autodelete ? "T" : "F"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
    return result;
}

void Broker::deleteExchange(const std::string& name, const std::string& userId,
                           const std::string& connectionId)
{
<<<<<<< HEAD
    if (acl) {
        if (!acl->authorise(userId,acl::ACT_DELETE,acl::OBJ_EXCHANGE,name,NULL) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied exchange delete request from " << userId));
    }

=======
    QPID_LOG_CAT(debug, model, "Deleting exchange. name:" << name
        << " user:" << userId
        << " rhost:" << connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (name.empty()) {
        throw framing::InvalidArgumentException(QPID_MSG("Delete not allowed for default exchange"));
    }
    Exchange::shared_ptr exchange(exchanges.get(name));
    if (!exchange) throw framing::NotFoundException(QPID_MSG("Delete failed. No such exchange: " << name));
<<<<<<< HEAD
    if (exchange->inUseAsAlternate()) throw framing::NotAllowedException(QPID_MSG("Exchange in use as alternate-exchange."));
    if (exchange->isDurable()) store->destroy(*exchange);
    if (exchange->getAlternate()) exchange->getAlternate()->decAlternateUsers();
    exchanges.destroy(name);

    if (managementAgent.get())
        managementAgent->raiseEvent(_qmf::EventExchangeDelete(connectionId, userId, name));

=======

    if (acl) {
        std::map<acl::Property, std::string> params;
        Exchange::shared_ptr altEx = exchange->getAlternate();
        params.insert(make_pair(acl::PROP_TYPE, exchange->getType()));
        params.insert(make_pair(acl::PROP_ALTERNATE, (altEx) ? altEx->getName() : "" ));
        params.insert(make_pair(acl::PROP_DURABLE, exchange->isDurable() ? _TRUE : _FALSE));

        if (!acl->authorise(userId,acl::ACT_DELETE,acl::OBJ_EXCHANGE,name,&params) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied exchange delete request from " << userId));
    }

    if (exchange->inUseAsAlternate()) throw framing::NotAllowedException(QPID_MSG("Cannot delete " << name <<", in use as alternate-exchange."));
    if (exchange->isDurable()) store->destroy(*exchange);
    if (exchange->getAlternate()) exchange->getAlternate()->decAlternateUsers();
    exchanges.destroy(name, connectionId,  userId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void Broker::bind(const std::string& queueName,
                  const std::string& exchangeName,
                  const std::string& key,
                  const qpid::framing::FieldTable& arguments,
<<<<<<< HEAD
=======
                  const OwnershipToken* owner,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                  const std::string& userId,
                  const std::string& connectionId)
{
    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_QUEUENAME, queueName));
        params.insert(make_pair(acl::PROP_ROUTINGKEY, key));

        if (!acl->authorise(userId,acl::ACT_BIND,acl::OBJ_EXCHANGE,exchangeName,&params))
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied exchange bind request from " << userId));
    }
    if (exchangeName.empty()) {
        throw framing::InvalidArgumentException(QPID_MSG("Bind not allowed for default exchange"));
    }

    Queue::shared_ptr queue = queues.find(queueName);
    Exchange::shared_ptr exchange = exchanges.get(exchangeName);
    if (!queue) {
        throw framing::NotFoundException(QPID_MSG("Bind failed. No such queue: " << queueName));
    } else if (!exchange) {
        throw framing::NotFoundException(QPID_MSG("Bind failed. No such exchange: " << exchangeName));
<<<<<<< HEAD
    } else {
        if (queue->bind(exchange, key, arguments)) {
            getConfigurationObservers().bind(exchange, queue, key, arguments);
=======
    } else if (queue->hasExclusiveOwner() && !queue->isExclusiveOwner(owner)) {
        throw framing::ResourceLockedException(QPID_MSG("Cannot bind queue "
                                               << queue->getName() << "; it is exclusive to another session"));
    } else {
        if (queue->bind(exchange, key, arguments)) {
            getBrokerObservers().bind(exchange, queue, key, arguments);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            if (managementAgent.get()) {
                managementAgent->raiseEvent(_qmf::EventBind(connectionId, userId, exchangeName,
                                                  queueName, key, ManagementAgent::toMap(arguments)));
            }
<<<<<<< HEAD
=======
            QPID_LOG_CAT(debug, model, "Create binding. exchange:" << exchangeName
                << " queue:" << queueName
                << " key:" << key
                << " arguments:" << arguments
                << " user:" << userId
                << " rhost:" << connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        }
    }
}

void Broker::unbind(const std::string& queueName,
                    const std::string& exchangeName,
                    const std::string& key,
<<<<<<< HEAD
=======
                    const OwnershipToken* owner,
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                    const std::string& userId,
                    const std::string& connectionId)
{
    if (acl) {
        std::map<acl::Property, std::string> params;
        params.insert(make_pair(acl::PROP_QUEUENAME, queueName));
        params.insert(make_pair(acl::PROP_ROUTINGKEY, key));
        if (!acl->authorise(userId,acl::ACT_UNBIND,acl::OBJ_EXCHANGE,exchangeName,&params) )
            throw framing::UnauthorizedAccessException(QPID_MSG("ACL denied exchange unbind request from " << userId));
    }
    if (exchangeName.empty()) {
        throw framing::InvalidArgumentException(QPID_MSG("Unbind not allowed for default exchange"));
    }
    Queue::shared_ptr queue = queues.find(queueName);
    Exchange::shared_ptr exchange = exchanges.get(exchangeName);
    if (!queue) {
        throw framing::NotFoundException(QPID_MSG("Unbind failed. No such queue: " << queueName));
    } else if (!exchange) {
        throw framing::NotFoundException(QPID_MSG("Unbind failed. No such exchange: " << exchangeName));
<<<<<<< HEAD
=======
    } else if (queue->hasExclusiveOwner() && !queue->isExclusiveOwner(owner)) {
        throw framing::ResourceLockedException(QPID_MSG("Cannot unbind queue "
                                               << queue->getName() << "; it is exclusive to another session"));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else {
        if (exchange->unbind(queue, key, 0)) {
            if (exchange->isDurable() && queue->isDurable()) {
                store->unbind(*exchange, *queue, key, qpid::framing::FieldTable());
            }
<<<<<<< HEAD
            getConfigurationObservers().unbind(
=======
            getBrokerObservers().unbind(
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                exchange, queue, key, framing::FieldTable());
            if (managementAgent.get()) {
                managementAgent->raiseEvent(_qmf::EventUnbind(connectionId, userId, exchangeName, queueName, key));
            }
<<<<<<< HEAD
=======
            QPID_LOG_CAT(debug, model, "Delete binding. exchange:" << exchangeName
                << " queue:" << queueName
                << " key:" << key
                << " user:" << userId
                << " rhost:" << connectionId);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        }
    }
}

// FIXME aconway 2012-04-27: access to linkClientProperties is
// not properly thread safe, you could lose fields if 2 threads
// attempt to add a field concurrently.

framing::FieldTable Broker::getLinkClientProperties() const {
    sys::Mutex::ScopedLock l(linkClientPropertiesLock);
    return linkClientProperties;
}

void Broker::setLinkClientProperties(const framing::FieldTable& ft) {
    sys::Mutex::ScopedLock l(linkClientPropertiesLock);
    linkClientProperties = ft;
}

}} // namespace qpid::broker

