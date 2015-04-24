#!/usr/bin/env python
<<<<<<< HEAD

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

import os, signal, sys, time, imp, re, subprocess, glob, random, logging, shutil, math, unittest
import traceback
<<<<<<< HEAD
from qpid.messaging import Message, NotFound, ConnectionError, ReceiverError, Connection, Timeout, Disposition, REJECTED
from qpid.datatypes import uuid4
from brokertest import *
from threading import Thread, Lock, Condition
from logging import getLogger, WARN, ERROR, DEBUG, INFO
from qpidtoollibs import BrokerAgent
from uuid import UUID

log = getLogger(__name__)

class QmfAgent(object):
    """Access to a QMF broker agent."""
    def __init__(self, address, **kwargs):
        self._connection = Connection.establish(
            address, client_properties={"qpid.ha-admin":1}, **kwargs)
        self._agent = BrokerAgent(self._connection)
        assert self._agent.getHaBroker(), "HA module not loaded in broker at: %s"%(address)

    def __getattr__(self, name):
        a = getattr(self._agent, name)
        return a

class Credentials(object):
    """SASL credentials: username, password, and mechanism"""
    def __init__(self, username, password, mechanism):
        (self.username, self.password, self.mechanism) = (username, password, mechanism)

    def __str__(self): return "Credentials%s"%(self.tuple(),)

    def tuple(self): return (self.username, self.password, self.mechanism)

    def add_user(self, url): return "%s/%s@%s"%(self.username, self.password, url)

class HaBroker(Broker):
    """Start a broker with HA enabled
    @param client_cred: (user, password, mechanism) for admin clients started by the HaBroker.
    """
    def __init__(self, test, args=[], brokers_url=None, ha_cluster=True, ha_replicate="all",
                 client_credentials=None, **kwargs):
        assert BrokerTest.ha_lib, "Cannot locate HA plug-in"
        args = copy(args)
        args += ["--load-module", BrokerTest.ha_lib,
                 "--log-enable=debug+:ha::",
                 # FIXME aconway 2012-02-13: workaround slow link failover.
                 "--link-maintenace-interval=0.1",
                 "--ha-cluster=%s"%ha_cluster]
        if ha_replicate is not None:
            args += [ "--ha-replicate=%s"%ha_replicate ]
        if brokers_url: args += [ "--ha-brokers-url", brokers_url ]
        Broker.__init__(self, test, args, **kwargs)
        self.qpid_ha_path=os.path.join(os.getenv("PYTHON_COMMANDS"), "qpid-ha")
        assert os.path.exists(self.qpid_ha_path)
        self.qpid_config_path=os.path.join(os.getenv("PYTHON_COMMANDS"), "qpid-config")
        assert os.path.exists(self.qpid_config_path)
        getLogger().setLevel(ERROR) # Hide expected WARNING log messages from failover.
        self.qpid_ha_script=import_script(self.qpid_ha_path)
        self._agent = None
        self.client_credentials = client_credentials

    def __str__(self): return Broker.__str__(self)

    def qpid_ha(self, args):
        cred = self.client_credentials
        url = self.host_port()
        if cred:
            url =cred.add_user(url)
            args = args + ["--sasl-mechanism", cred.mechanism]
        self.qpid_ha_script.main_except(["", "-b", url]+args)

    def promote(self): self.qpid_ha(["promote"])
    def set_client_url(self, url): self.qpid_ha(["set", "--public-url", url])
    def set_brokers_url(self, url): self.qpid_ha(["set", "--brokers-url", url])
    def replicate(self, from_broker, queue): self.qpid_ha(["replicate", from_broker, queue])

    def agent(self):
        if not self._agent:
            cred = self.client_credentials
            if cred:
                self._agent = QmfAgent(cred.add_user(self.host_port()), sasl_mechanisms=cred.mechanism)
            else:
                self._agent = QmfAgent(self.host_port())
        return self._agent

    def ha_status(self):
        hb = self.agent().getHaBroker()
        hb.update()
        return hb.status

    def wait_status(self, status):
        def try_get_status():
            # Ignore ConnectionError, the broker may not be up yet.
            try:
                self._status = self.ha_status()
                return self._status == status;
            except ConnectionError: return False
        assert retry(try_get_status, timeout=20), "%s %r != %r"%(self, self._status, status)

    # FIXME aconway 2012-05-01: do direct python call to qpid-config code.
    def qpid_config(self, args):
        assert subprocess.call(
            [self.qpid_config_path, "--broker", self.host_port()]+args) == 0

    def config_replicate(self, from_broker, queue):
        self.qpid_config(["add", "queue", "--start-replica", from_broker, queue])

    def config_declare(self, queue, replication):
        self.qpid_config(["add", "queue", queue, "--replicate", replication])

    def connect_admin(self, **kwargs):
        cred = self.client_credentials
        if cred:
            return Broker.connect(
                self, client_properties={"qpid.ha-admin":1},
                username=cred.username, password=cred.password, sasl_mechanisms=cred.mechanism,
                **kwargs)
        else:
            return Broker.connect(self, client_properties={"qpid.ha-admin":1}, **kwargs)

    def wait_backup(self, address):
        """Wait for address to become valid on a backup broker."""
        bs = self.connect_admin().session()
        try: wait_address(bs, address)
        finally: bs.connection.close()

    def assert_browse(self, queue, expected, **kwargs):
        """Verify queue contents by browsing."""
        bs = self.connect().session()
        try:
            wait_address(bs, queue)
            assert_browse_retry(bs, queue, expected, **kwargs)
        finally: bs.connection.close()

    def assert_browse_backup(self, queue, expected, **kwargs):
        """Combines wait_backup and assert_browse_retry."""
        bs = self.connect_admin().session()
        try:
            wait_address(bs, queue)
            assert_browse_retry(bs, queue, expected, **kwargs)
        finally: bs.connection.close()

    def assert_connect_fail(self):
        try:
            self.connect()
            self.test.fail("Expected ConnectionError")
        except ConnectionError: pass

    def try_connect(self):
        try: return self.connect()
        except ConnectionError: return None

class HaCluster(object):
    _cluster_count = 0

    def __init__(self, test, n, promote=True, **kwargs):
        """Start a cluster of n brokers"""
        self.test = test
        self.kwargs = kwargs
        self._brokers = []
        self.id = HaCluster._cluster_count
        self.broker_id = 0
        HaCluster._cluster_count += 1
        for i in xrange(n): self.start(False)
        self.update_urls()
        self[0].promote()

    def next_name(self):
        name="cluster%s-%s"%(self.id, self.broker_id)
        self.broker_id += 1
        return name

    def start(self, update_urls=True, args=[]):
        """Start a new broker in the cluster"""
        b = HaBroker(self.test, name=self.next_name(), **self.kwargs)
        self._brokers.append(b)
        if update_urls: self.update_urls()
        return b

    def update_urls(self):
        self.url = ",".join([b.host_port() for b in self])
        if len(self) > 1:          # No failover addresses on a 1 cluster.
            for b in self: b.set_brokers_url(self.url)

    def connect(self, i):
        """Connect with reconnect_urls"""
        return self[i].connect(reconnect=True, reconnect_urls=self.url.split(","))

    def kill(self, i, promote_next=True):
        """Kill broker i, promote broker i+1"""
        self[i].expect = EXPECT_EXIT_FAIL
        self[i].kill()
        if promote_next: self[(i+1) % len(self)].promote()

    def restart(self, i):
        """Start a broker with the same port, name and data directory. It will get
        a separate log file: foo.n.log"""
        b = self._brokers[i]
        self._brokers[i] = HaBroker(
            self.test, name=b.name, port=b.port(), brokers_url=self.url,
            **self.kwargs)

    def bounce(self, i, promote_next=True):
        """Stop and restart a broker in a cluster."""
        self.kill(i, promote_next)
        self.restart(i)

    # Behave like a list of brokers.
    def __len__(self): return len(self._brokers)
    def __getitem__(self,index): return self._brokers[index]
    def __iter__(self): return self._brokers.__iter__()

def wait_address(session, address):
    """Wait for an address to become valid."""
    def check():
        try:
            session.sender(address)
            return True
        except NotFound: return False
    assert retry(check), "Timed out waiting for address %s"%(address)

def valid_address(session, address):
    """Test if an address is valid"""
    try:
        session.receiver(address)
        return True
    except NotFound: return False

class ReplicationTests(BrokerTest):
=======
from qpid.datatypes import uuid4, UUID
from brokertest import *
from ha_test import *
from threading import Thread, Lock, Condition
from logging import getLogger, WARN, ERROR, DEBUG, INFO
from qpidtoollibs import BrokerAgent, EventHelper

log = getLogger(__name__)

class HaBrokerTest(BrokerTest):
    """Base class for HA broker tests"""

class ReplicationTests(HaBrokerTest):
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    """Correctness tests for  HA replication."""

    def test_replication(self):
        """Test basic replication of configuration and messages before and
        after backup has connected"""

<<<<<<< HEAD
        def queue(name, replicate):
            return "%s;{create:always,node:{x-declare:{arguments:{'qpid.replicate':%s}}}}"%(name, replicate)

        def exchange(name, replicate, bindq):
            return"%s;{create:always,node:{type:topic,x-declare:{arguments:{'qpid.replicate':%s}, type:'fanout'},x-bindings:[{exchange:'%s',queue:'%s'}]}}"%(name, replicate, name, bindq)
        def setup(p, prefix, primary):
            """Create config, send messages on the primary p"""
            s = p.sender(queue(prefix+"q1", "all"))
            for m in ["a", "b", "1"]: s.send(Message(m))
            # Test replication of dequeue
            self.assertEqual(p.receiver(prefix+"q1").fetch(timeout=0).content, "a")
            p.acknowledge()
            p.sender(queue(prefix+"q2", "configuration")).send(Message("2"))
            p.sender(queue(prefix+"q3", "none")).send(Message("3"))
            p.sender(exchange(prefix+"e1", "all", prefix+"q1")).send(Message("4"))
            p.sender(exchange(prefix+"e2", "all", prefix+"q2")).send(Message("5"))
            # Test  unbind
            p.sender(queue(prefix+"q4", "all")).send(Message("6"))
            s3 = p.sender(exchange(prefix+"e4", "all", prefix+"q4"))
            s3.send(Message("7"))
            # Use old connection to unbind
            us = primary.connect_old().session(str(uuid4()))
            us.exchange_unbind(exchange=prefix+"e4", binding_key="", queue=prefix+"q4")
            p.sender(prefix+"e4").send(Message("drop1")) # Should be dropped
            # Need a marker so we can wait till sync is done.
            p.sender(queue(prefix+"x", "configuration"))
=======
        def setup(prefix, primary):
            """Create config, send messages on the primary p"""
            a = primary.agent

            def queue(name, replicate):
                a.addQueue(name, options={'qpid.replicate':replicate})
                return name

            def exchange(name, replicate, bindq, key):
                a.addExchange("fanout", name, options={'qpid.replicate':replicate})
                a.bind(name, bindq, key)
                return name

            # Test replication of messages
            p = primary.connect().session()
            s = p.sender(queue(prefix+"q1", "all"))
            for m in ["a", "b", "1"]: s.send(qm.Message(m))
            # Test replication of dequeue
            self.assertEqual(p.receiver(prefix+"q1").fetch(timeout=0).content, "a")
            p.acknowledge()

            p.sender(queue(prefix+"q2", "configuration")).send(qm.Message("2"))
            p.sender(queue(prefix+"q3", "none")).send(qm.Message("3"))
            p.sender(exchange(prefix+"e1", "all", prefix+"q1", "key1")).send(qm.Message("4"))
            p.sender(exchange(prefix+"e2", "configuration", prefix+"q2", "key2")).send(qm.Message("5"))
            # Test  unbind
            p.sender(queue(prefix+"q4", "all")).send(qm.Message("6"))
            s3 = p.sender(exchange(prefix+"e4", "all", prefix+"q4", "key4"))
            s3.send(qm.Message("7"))
            a.unbind(prefix+"e4", prefix+"q4", "key4")
            p.sender(prefix+"e4").send(qm.Message("drop1")) # Should be dropped

            # Test replication of deletes
            queue(prefix+"dq", "all")
            exchange(prefix+"de", "all", prefix+"dq", "")
            a.delQueue(prefix+"dq")
            a.delExchange(prefix+"de")

            # Need a marker so we can wait till sync is done.
            queue(prefix+"x", "configuration")
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        def verify(b, prefix, p):
            """Verify setup was replicated to backup b"""
            # Wait for configuration to replicate.
<<<<<<< HEAD
            wait_address(b, prefix+"x");
=======
            wait_address(b.connection, prefix+"x");
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            self.assert_browse_retry(b, prefix+"q1", ["b", "1", "4"])

            self.assertEqual(p.receiver(prefix+"q1").fetch(timeout=0).content, "b")
            p.acknowledge()
            self.assert_browse_retry(b, prefix+"q1", ["1", "4"])

            self.assert_browse_retry(b, prefix+"q2", []) # configuration only
<<<<<<< HEAD
            assert not valid_address(b, prefix+"q3")
            b.sender(prefix+"e1").send(Message(prefix+"e1")) # Verify binds with replicate=all
            self.assert_browse_retry(b, prefix+"q1", ["1", "4", prefix+"e1"])
            b.sender(prefix+"e2").send(Message(prefix+"e2")) # Verify binds with replicate=configuration
            self.assert_browse_retry(b, prefix+"q2", [prefix+"e2"])

            b.sender(prefix+"e4").send(Message("drop2")) # Verify unbind.
            self.assert_browse_retry(b, prefix+"q4", ["6","7"])

        primary = HaBroker(self, name="primary")
        primary.promote()
        p = primary.connect().session()

        # Create config, send messages before starting the backup, to test catch-up replication.
        setup(p, "1", primary)
        backup  = HaBroker(self, name="backup", brokers_url=primary.host_port())
        # Create config, send messages after starting the backup, to test steady-state replication.
        setup(p, "2", primary)

        # Verify the data on the backup
        b = backup.connect_admin().session()
        verify(b, "1", p)
        verify(b, "2", p)
        # Test a series of messages, enqueue all then dequeue all.
        s = p.sender(queue("foo","all"))
        wait_address(b, "foo")
        msgs = [str(i) for i in range(10)]
        for m in msgs: s.send(Message(m))
        self.assert_browse_retry(p, "foo", msgs)
        self.assert_browse_retry(b, "foo", msgs)
        r = p.receiver("foo")
        for m in msgs: self.assertEqual(m, r.fetch(timeout=0).content)
        p.acknowledge()
        self.assert_browse_retry(p, "foo", [])
        self.assert_browse_retry(b, "foo", [])

        # Another series, this time verify each dequeue individually.
        for m in msgs: s.send(Message(m))
        self.assert_browse_retry(p, "foo", msgs)
        self.assert_browse_retry(b, "foo", msgs)
        for i in range(len(msgs)):
            self.assertEqual(msgs[i], r.fetch(timeout=0).content)
            p.acknowledge()
            self.assert_browse_retry(p, "foo", msgs[i+1:])
            self.assert_browse_retry(b, "foo", msgs[i+1:])
=======
            assert not valid_address(b.connection, prefix+"q3")

            # Verify exchange with replicate=all
            b.sender(prefix+"e1/key1").send(qm.Message(prefix+"e1"))
            self.assert_browse_retry(b, prefix+"q1", ["1", "4", prefix+"e1"])

            # Verify exchange with replicate=configuration
            b.sender(prefix+"e2/key2").send(qm.Message(prefix+"e2"))
            self.assert_browse_retry(b, prefix+"q2", [prefix+"e2"])

            b.sender(prefix+"e4/key4").send(qm.Message("drop2")) # Verify unbind.
            self.assert_browse_retry(b, prefix+"q4", ["6","7"])

            # Verify deletes
            assert not valid_address(b.connection, prefix+"dq")
            assert not valid_address(b.connection, prefix+"de")

        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            cluster = HaCluster(self, 2)
            primary = cluster[0]
            backup  = cluster[1]

            # Send messages before re-starting the backup, test catch-up replication.
            cluster.kill(1, promote_next=False, final=False)
            setup("1", primary)
            cluster.restart(1)

            # Send messages after re-starting the backup, to test steady-state replication.
            setup("2", primary)

            p = primary.connect().session()

            # Verify the data on the backup
            b = backup.connect_admin().session()
            verify(b, "1", p)
            verify(b, "2", p)
            # Test a series of messages, enqueue all then dequeue all.
            primary.agent.addQueue("foo")
            s = p.sender("foo")
            wait_address(b.connection, "foo")
            msgs = [str(i) for i in range(10)]
            for m in msgs: s.send(qm.Message(m))
            self.assert_browse_retry(p, "foo", msgs)
            self.assert_browse_retry(b, "foo", msgs)
            r = p.receiver("foo")
            for m in msgs: self.assertEqual(m, r.fetch(timeout=0).content)
            p.acknowledge()
            self.assert_browse_retry(p, "foo", [])
            self.assert_browse_retry(b, "foo", [])

            # Another series, this time verify each dequeue individually.
            for m in msgs: s.send(qm.Message(m))
            self.assert_browse_retry(p, "foo", msgs)
            self.assert_browse_retry(b, "foo", msgs)
            for i in range(len(msgs)):
                self.assertEqual(msgs[i], r.fetch(timeout=0).content)
                p.acknowledge()
                self.assert_browse_retry(p, "foo", msgs[i+1:])
                self.assert_browse_retry(b, "foo", msgs[i+1:])
        finally: l.restore()
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_sync(self):
        primary = HaBroker(self, name="primary")
        primary.promote()
        p = primary.connect().session()
        s = p.sender("q;{create:always}")
        for m in [str(i) for i in range(0,10)]: s.send(m)
        s.sync()
        backup1 = HaBroker(self, name="backup1", brokers_url=primary.host_port())
        for m in [str(i) for i in range(10,20)]: s.send(m)
        s.sync()
        backup2 = HaBroker(self, name="backup2", brokers_url=primary.host_port())
        for m in [str(i) for i in range(20,30)]: s.send(m)
        s.sync()

        msgs = [str(i) for i in range(30)]
        b1 = backup1.connect_admin().session()
<<<<<<< HEAD
        wait_address(b1, "q");
        self.assert_browse_retry(b1, "q", msgs)
        b2 = backup2.connect_admin().session()
        wait_address(b2, "q");
        self.assert_browse_retry(b2, "q", msgs)

    def test_send_receive(self):
        """Verify sequence numbers of messages sent by qpid-send"""
        brokers = HaCluster(self, 3)
        sender = self.popen(
            ["qpid-send",
             "--broker", brokers[0].host_port(),
             "--address", "q;{create:always}",
             "--messages=1000",
             "--content-string=x"
             ])
        receiver = self.popen(
            ["qpid-receive",
             "--broker", brokers[0].host_port(),
             "--address", "q;{create:always}",
             "--messages=990",
             "--timeout=10"
             ])
        self.assertEqual(sender.wait(), 0)
        self.assertEqual(receiver.wait(), 0)
        expect = [long(i) for i in range(991, 1001)]
        sn = lambda m: m.properties["sn"]
        brokers[1].assert_browse_backup("q", expect, transform=sn)
        brokers[2].assert_browse_backup("q", expect, transform=sn)

    def test_failover_python(self):
        """Verify that backups rejects connections and that fail-over works in python client"""
        primary = HaBroker(self, name="primary", expect=EXPECT_EXIT_FAIL)
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        # Check that backup rejects normal connections
        try:
            backup.connect().session()
            self.fail("Expected connection to backup to fail")
        except ConnectionError: pass
        # Check that admin connections are allowed to backup.
        backup.connect_admin().close()

        # Test discovery: should connect to primary after reject by backup
        c = backup.connect(reconnect_urls=[primary.host_port(), backup.host_port()], reconnect=True)
        s = c.session()
        sender = s.sender("q;{create:always}")
        backup.wait_backup("q")
        sender.send("foo")
        primary.kill()
        assert retry(lambda: not is_running(primary.pid))
        backup.promote()
        sender.send("bar")
        self.assert_browse_retry(s, "q", ["foo", "bar"])
        c.close()

    def test_failover_cpp(self):
        """Verify that failover works in the C++ client."""
        primary = HaBroker(self, name="primary", expect=EXPECT_EXIT_FAIL)
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        url="%s,%s"%(primary.host_port(), backup.host_port())
        primary.connect().session().sender("q;{create:always}")
        backup.wait_backup("q")

        sender = NumberedSender(primary, url=url, queue="q", failover_updates = False)
        receiver = NumberedReceiver(primary, url=url, queue="q", failover_updates = False)
        receiver.start()
        sender.start()
        backup.wait_backup("q")
        assert retry(lambda: receiver.received > 10) # Wait for some messages to get thru

        primary.kill()
        assert retry(lambda: not is_running(primary.pid)) # Wait for primary to die
        backup.promote()
        n = receiver.received       # Make sure we are still running
        assert retry(lambda: receiver.received > n + 10)
=======
        backup1.assert_browse_backup("q", msgs)
        backup2.assert_browse_backup("q", msgs)

    def test_send_receive(self):
        """Verify sequence numbers of messages sent by qpid-send"""
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            brokers = HaCluster(self, 3)
            sender = self.popen(
                ["qpid-send",
                 "--broker", brokers[0].host_port(),
                 "--address", "q;{create:always}",
                 "--messages=1000",
                 "--content-string=x",
                 "--connection-options={%s}"%self.protocol_option()
                 ])
            receiver = self.popen(
                ["qpid-receive",
                 "--broker", brokers[0].host_port(),
                 "--address", "q;{create:always}",
                 "--messages=990",
                 "--timeout=10",
                 "--connection-options={%s}"%self.protocol_option()
                 ])
            self.assertEqual(sender.wait(), 0)
            self.assertEqual(receiver.wait(), 0)
            expect = [long(i) for i in range(991, 1001)]
            sn = lambda m: m.properties["sn"]
            brokers[1].assert_browse_backup("q", expect, transform=sn)
            brokers[2].assert_browse_backup("q", expect, transform=sn)
        finally: l.restore()

    def test_failover_python(self):
        """Verify that backups rejects connections and that fail-over works in python client"""
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            primary  = HaBroker(self, name="primary")
            primary.promote()
            backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
            # Check that backup rejects normal connections
            try:
                backup.connect().session()
                self.fail("Expected connection to backup to fail")
            except qm.ConnectionError: pass
            # Check that admin connections are allowed to backup.
            backup.connect_admin().close()

            # Test discovery: should connect to primary after reject by backup
            c = backup.connect(reconnect_urls=[primary.host_port(), backup.host_port()],
                               reconnect=True)
            s = c.session()
            sender = s.sender("q;{create:always}")
            sender.send("foo", sync=True)
            s.sync()
            primary.kill()
            assert retry(lambda: not is_running(primary.pid))
            backup.promote()
            sender.send("bar")
            self.assert_browse_retry(s, "q", ["foo", "bar"])
            c.close()
        finally: l.restore()


    def test_heartbeat_python(self):
        """Verify that a python client with a heartbeat specified disconnects
        from a stalled broker and does not hang indefinitely."""

        broker = Broker(self)
        broker_addr = broker.host_port()

        # Case 1: Connect before stalling the broker, use the connection after stalling.
        c = qm.Connection(broker_addr, heartbeat=1)
        c.open()
        os.kill(broker.pid, signal.SIGSTOP) # Stall the broker

        def make_sender(): c.session().sender("foo")
        self.assertRaises(qm.ConnectionError, make_sender)

        # Case 2: Connect to a stalled broker
        c = qm.Connection(broker_addr, heartbeat=1)
        self.assertRaises(qm.ConnectionError, c.open)

        # Case 3: Re-connect to a stalled broker.
        broker2 = Broker(self)
        c = qm.Connection(broker2.host_port(), heartbeat=1, reconnect_limit=1,
                          reconnect=True, reconnect_urls=[broker_addr],
                          reconnect_log=False) # Hide expected warnings
        c.open()
        broker2.kill()          # Cause re-connection to broker
        self.assertRaises(qm.ConnectionError, make_sender)

    def test_failover_cpp(self):
        """Verify that failover works in the C++ client."""
        cluster = HaCluster(self, 2)
        cluster[0].connect().session().sender("q;{create:always}")
        cluster[1].wait_backup("q")
        # FIXME aconway 2014-02-21: using 0-10, there is a failover problem with 1.0
        sender = NumberedSender(cluster[0], url=cluster.url, queue="q",
                                connection_options="reconnect:true,protocol:'amqp0-10'")
        receiver = NumberedReceiver(cluster[0], url=cluster.url, queue="q",
                                    connection_options="reconnect:true,protocol:'amqp0-10'")
        receiver.start()
        sender.start()
        assert retry(lambda: receiver.received > 10) # Wait for some messages to get thru
        cluster.kill(0)
        n = receiver.received
        assert retry(lambda: receiver.received > n + 10) # Verify we are still going
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        sender.stop()
        receiver.stop()

    def test_backup_failover(self):
        """Verify that a backup broker fails over and recovers queue state"""
        brokers = HaCluster(self, 3)
        brokers[0].connect().session().sender("q;{create:always}").send("a")
<<<<<<< HEAD
        for b in brokers[1:]: b.assert_browse_backup("q", ["a"], msg=b)
        brokers[0].expect = EXPECT_EXIT_FAIL
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        brokers.kill(0)
        brokers[1].connect().session().sender("q").send("b")
        brokers[2].assert_browse_backup("q", ["a","b"])
        s = brokers[1].connect().session()
        self.assertEqual("a", s.receiver("q").fetch().content)
        s.acknowledge()
        brokers[2].assert_browse_backup("q", ["b"])

<<<<<<< HEAD
=======
    def test_empty_backup_failover(self):
        """Verify that a new primary becomes active with no queues.
        Regression test for QPID-5430"""
        brokers = HaCluster(self, 3)
        brokers.kill(0)
        brokers[1].wait_status("active")

>>>>>>> 3bbfc42... Imported Upstream version 0.32
    def test_qpid_config_replication(self):
        """Set up replication via qpid-config"""
        brokers = HaCluster(self,2)
        brokers[0].config_declare("q","all")
        brokers[0].connect().session().sender("q").send("foo")
        brokers[1].assert_browse_backup("q", ["foo"])

    def test_standalone_queue_replica(self):
        """Test replication of individual queues outside of cluster mode"""
<<<<<<< HEAD
        getLogger().setLevel(ERROR) # Hide expected WARNING log messages from failover.
        primary = HaBroker(self, name="primary", ha_cluster=False)
        pc = primary.connect()
        ps = pc.session().sender("q;{create:always}")
        pr = pc.session().receiver("q;{create:always}")
        backup = HaBroker(self, name="backup", ha_cluster=False)
        br = backup.connect().session().receiver("q;{create:always}")

        # Set up replication with qpid-ha
        backup.replicate(primary.host_port(), "q")
        ps.send("a")
        backup.assert_browse_backup("q", ["a"])
        ps.send("b")
        backup.assert_browse_backup("q", ["a", "b"])
        self.assertEqual("a", pr.fetch().content)
        pr.session.acknowledge()
        backup.assert_browse_backup("q", ["b"])
=======
        primary = HaBroker(self, name="primary", ha_cluster=False,
                           args=["--ha-queue-replication=yes"]);
        pc = primary.connect()
        ps = pc.session().sender("q;{create:always}")
        pr = pc.session().receiver("q;{create:always}")
        backup = HaBroker(self, name="backup", ha_cluster=False,
                          args=["--ha-queue-replication=yes"])
        bs = backup.connect().session()
        br = bs.receiver("q;{create:always}")

        def srange(*args): return [str(i) for i in xrange(*args)]

        for m in srange(3): ps.send(m)
        # Set up replication with qpid-ha
        backup.replicate(primary.host_port(), "q")
        backup.assert_browse_backup("q", srange(3))
        for m in srange(3,6): ps.send(str(m))
        backup.assert_browse_backup("q", srange(6))
        self.assertEqual("0", pr.fetch().content)
        pr.session.acknowledge()
        backup.assert_browse_backup("q", srange(1,6))
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        # Set up replication with qpid-config
        ps2 = pc.session().sender("q2;{create:always}")
        backup.config_replicate(primary.host_port(), "q2");
        ps2.send("x")
        backup.assert_browse_backup("q2", ["x"])


<<<<<<< HEAD
    def test_queue_replica_failover(self):
        """Test individual queue replication from a cluster to a standalone backup broker, verify it fails over."""
        cluster = HaCluster(self, 2)
        primary = cluster[0]
        pc = cluster.connect(0)
        ps = pc.session().sender("q;{create:always}")
        pr = pc.session().receiver("q;{create:always}")
        backup = HaBroker(self, name="backup", ha_cluster=False)
        br = backup.connect().session().receiver("q;{create:always}")
        backup.replicate(cluster.url, "q")
        ps.send("a")
        backup.assert_browse_backup("q", ["a"])
        cluster.bounce(0)
        backup.assert_browse_backup("q", ["a"])
        ps.send("b")
        backup.assert_browse_backup("q", ["a", "b"])
        cluster.bounce(1)
        self.assertEqual("a", pr.fetch().content)
        pr.session.acknowledge()
        backup.assert_browse_backup("q", ["b"])

    def test_lvq(self):
        """Verify that we replicate to an LVQ correctly"""
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        s = primary.connect().session().sender("lvq; {create:always, node:{x-declare:{arguments:{'qpid.last_value_queue_key':lvq-key}}}}")
        def send(key,value): s.send(Message(content=value,properties={"lvq-key":key}))
        for kv in [("a","a-1"),("b","b-1"),("a","a-2"),("a","a-3"),("c","c-1"),("c","c-2")]:
            send(*kv)
        backup.assert_browse_backup("lvq", ["b-1", "a-3", "c-2"])
        send("b","b-2")
        backup.assert_browse_backup("lvq", ["a-3", "c-2", "b-2"])
        send("c","c-3")
        backup.assert_browse_backup("lvq", ["a-3", "b-2", "c-3"])
        send("d","d-1")
        backup.assert_browse_backup("lvq", ["a-3", "b-2", "c-3", "d-1"])

    def test_ring(self):
        """Test replication with the ring queue policy"""
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        s = primary.connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':ring, 'qpid.max_count':5}}}}")
        for i in range(10): s.send(Message(str(i)))
        backup.assert_browse_backup("q", [str(i) for i in range(5,10)])

    def test_reject(self):
        """Test replication with the reject queue policy"""
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        s = primary.connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':reject, 'qpid.max_count':5}}}}")
        try:
            for i in range(10): s.send(Message(str(i)), sync=False)
        except qpid.messaging.exceptions.TargetCapacityExceeded: pass
        backup.assert_browse_backup("q", [str(i) for i in range(0,5)])
        # Detach, don't close as there is a broken session
        s.session.connection.detach()

    def test_priority(self):
        """Verify priority queues replicate correctly"""
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        session = primary.connect().session()
        s = session.sender("priority-queue; {create:always, node:{x-declare:{arguments:{'qpid.priorities':10}}}}")
        priorities = [8,9,5,1,2,2,3,4,9,7,8,9,9,2]
        for p in priorities: s.send(Message(priority=p))
        # Can't use browse_backup as browser sees messages in delivery order not priority.
        backup.wait_backup("priority-queue")
        r = backup.connect_admin().session().receiver("priority-queue")
=======
    def test_standalone_queue_replica_failover(self):
        """Test individual queue replication from a cluster to a standalone
        backup broker, verify it fails over."""
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            cluster = HaCluster(self, 2)
            primary = cluster[0]
            pc = cluster.connect(0)
            ps = pc.session().sender("q;{create:always}")
            pr = pc.session().receiver("q;{create:always}")
            backup = HaBroker(self, name="backup", ha_cluster=False,
                              args=["--ha-queue-replication=yes"])
            br = backup.connect().session().receiver("q;{create:always}")
            backup.replicate(cluster.url, "q")
            ps.send("a")
            ps.sync()
            backup.assert_browse_backup("q", ["a"])
            cluster.bounce(0)
            backup.assert_browse_backup("q", ["a"])
            ps.send("b")
            backup.assert_browse_backup("q", ["a", "b"])
            cluster[0].wait_status("ready")
            cluster.bounce(1)
            # FIXME aconway 2014-02-20: pr does not fail over with 1.0/swig
            if qm == qpid_messaging:
                print "WARNING: Skipping SWIG client failover bug"
                return
            self.assertEqual("a", pr.fetch().content)
            pr.session.acknowledge()
            backup.assert_browse_backup("q", ["b"])
            pc.close()
            br.close()
        finally: l.restore()

    def test_lvq(self):
        """Verify that we replicate to an LVQ correctly"""
        cluster = HaCluster(self, 2)
        s = cluster[0].connect().session().sender("lvq; {create:always, node:{x-declare:{arguments:{'qpid.last_value_queue_key':lvq-key}}}}")

        def send(key,value,expect):
            s.send(qm.Message(content=value,properties={"lvq-key":key}))
            cluster[1].assert_browse_backup("lvq", expect)

        send("a", "a-1", ["a-1"])
        send("b", "b-1", ["a-1", "b-1"])
        send("a", "a-2", ["b-1", "a-2"])
        send("a", "a-3", ["b-1", "a-3"])
        send("c", "c-1", ["b-1", "a-3", "c-1"])
        send("c", "c-2", ["b-1", "a-3", "c-2"])
        send("b", "b-2", ["a-3", "c-2", "b-2"])
        send("c", "c-3", ["a-3", "b-2", "c-3"])
        send("d", "d-1", ["a-3", "b-2", "c-3", "d-1"])

    def test_ring(self):
        """Test replication with the ring queue policy"""
        """Verify that we replicate to an LVQ correctly"""
        cluster = HaCluster(self, 2)
        s = cluster[0].connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':ring, 'qpid.max_count':5}}}}")
        for i in range(10): s.send(qm.Message(str(i)))
        cluster[1].assert_browse_backup("q", [str(i) for i in range(5,10)])

    def test_reject(self):
        """Test replication with the reject queue policy"""
        cluster = HaCluster(self, 2)
        primary, backup = cluster
        s = primary.connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':reject, 'qpid.max_count':5}}}}")
        try:
            for i in range(10): s.send(qm.Message(str(i)), sync=False)
        except qm.LinkError: pass
        backup.assert_browse_backup("q", [str(i) for i in range(0,5)])
        try: s.session.connection.close()
        except: pass            # Expect exception from broken session

    def test_priority(self):
        """Verify priority queues replicate correctly"""
        cluster = HaCluster(self, 2)
        session = cluster[0].connect().session()
        s = session.sender("priority-queue; {create:always, node:{x-declare:{arguments:{'qpid.priorities':10}}}}")
        priorities = [8,9,5,1,2,2,3,4,9,7,8,9,9,2]
        for p in priorities: s.send(qm.Message(priority=p))
        # Can't use browse_backup as browser sees messages in delivery order not priority.
        cluster[1].wait_backup("priority-queue")
        r = cluster[1].connect_admin().session().receiver("priority-queue")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        received = [r.fetch().priority for i in priorities]
        self.assertEqual(sorted(priorities, reverse=True), received)

    def test_priority_fairshare(self):
        """Verify priority queues replicate correctly"""
<<<<<<< HEAD
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
=======
        cluster = HaCluster(self, 2)
        primary, backup = cluster
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        session = primary.connect().session()
        levels = 8
        priorities = [4,5,3,7,8,8,2,8,2,8,8,16,6,6,6,6,6,6,8,3,5,8,3,5,5,3,3,8,8,3,7,3,7,7,7,8,8,8,2,3]
        limits={7:0,6:4,5:3,4:2,3:2,2:2,1:2}
        limit_policy = ",".join(["'qpid.fairshare':5"] + ["'qpid.fairshare-%s':%s"%(i[0],i[1]) for i in limits.iteritems()])
        s = session.sender("priority-queue; {create:always, node:{x-declare:{arguments:{'qpid.priorities':%s, %s}}}}"%(levels,limit_policy))
<<<<<<< HEAD
        messages = [Message(content=str(uuid4()), priority = p) for p in priorities]
=======
        messages = [qm.Message(content=str(uuid4()), priority = p) for p in priorities]
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        for m in messages: s.send(m)
        backup.wait_backup(s.target)
        r = backup.connect_admin().session().receiver("priority-queue")
        received = [r.fetch().content for i in priorities]
        sort = sorted(messages, key=lambda m: priority_level(m.priority, levels), reverse=True)
        fair = [m.content for m in fairshare(sort, lambda l: limits.get(l,0), levels)]
        self.assertEqual(received, fair)

    def test_priority_ring(self):
<<<<<<< HEAD
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup = HaBroker(self, name="backup", brokers_url=primary.host_port())
        s = primary.connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':ring, 'qpid.max_count':5, 'qpid.priorities':10}}}}")
        priorities = [8,9,5,1,2,2,3,4,9,7,8,9,9,2]
        for p in priorities: s.send(Message(priority=p))

        # FIXME aconway 2012-02-22: there is a bug in priority ring
        # queues that allows a low priority message to displace a high
        # one. The following commented-out assert_browse is for the
        # correct result, the uncommented one is for the actualy buggy
        # result.  See https://issues.apache.org/jira/browse/QPID-3866
        #
        # expect = sorted(priorities,reverse=True)[0:5]
        expect = [9,9,9,9,2]
=======
        cluster = HaCluster(self, 2)
        primary, backup = cluster
        s = primary.connect().session().sender("q; {create:always, node:{x-declare:{arguments:{'qpid.policy_type':ring, 'qpid.max_count':5, 'qpid.priorities':10}}}}")
        priorities = [8,9,5,1,2,2,3,4,9,7,8,9,9,2]
        for p in priorities: s.send(qm.Message(priority=p))
        expect = sorted(priorities,reverse=True)[0:5]
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        primary.assert_browse("q", expect, transform=lambda m: m.priority)
        backup.assert_browse_backup("q", expect, transform=lambda m: m.priority)

    def test_backup_acquired(self):
        """Verify that acquired messages are backed up, for all queue types."""
        class Test:
            def __init__(self, queue, arguments, expect):
                self.queue = queue
                self.address = "%s;{create:always,node:{x-declare:{arguments:{%s}}}}"%(
<<<<<<< HEAD
                    self.queue, ",".join(arguments + ["'qpid.replicate':all"]))
=======
                    self.queue, ",".join(arguments))
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                self.expect = [str(i) for i in expect]

            def send(self, connection):
                """Send messages, then acquire one but don't acknowledge"""
                s = connection.session()
                for m in range(10): s.sender(self.address).send(str(m))
                s.receiver(self.address).fetch()

<<<<<<< HEAD
            def wait(self, brokertest, backup):
                backup.wait_backup(self.queue)

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            def verify(self, brokertest, backup):
                backup.assert_browse_backup(self.queue, self.expect, msg=self.queue)

        tests = [
            Test("plain",[],range(10)),
            Test("ring", ["'qpid.policy_type':ring", "'qpid.max_count':5"], range(5,10)),
            Test("priority",["'qpid.priorities':10"], range(10)),
            Test("fairshare", ["'qpid.priorities':10,'qpid.fairshare':5"], range(10)),
            Test("lvq", ["'qpid.last_value_queue_key':lvq-key"], [9])
            ]

<<<<<<< HEAD
        primary  = HaBroker(self, name="primary")
        primary.promote()
        backup1 = HaBroker(self, name="backup1", brokers_url=primary.host_port())
        c = primary.connect()
        for t in tests: t.send(c) # Send messages, leave one unacknowledged.

        backup2 = HaBroker(self, name="backup2", brokers_url=primary.host_port())
        # Wait for backups to catch up.
        for t in tests:
            t.wait(self, backup1)
            t.wait(self, backup2)
        # Verify acquired message was replicated
        for t in tests: t.verify(self, backup1)
        for t in tests: t.verify(self, backup2)
=======
        cluster = HaCluster(self, 3)
        cluster.kill(2, final=False) # restart after messages are sent to test catch-up

        c = cluster[0].connect()
        for t in tests: t.send(c) # Send messages, leave one unacknowledged.

        cluster.restart(2)
        cluster[2].wait_status("ready")

        # Verify acquired message was replicated
        for t in tests: t.verify(self, cluster[1])
        for t in tests: t.verify(self, cluster[2])
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_replicate_default(self):
        """Make sure we don't replicate if ha-replicate is unspecified or none"""
        cluster1 = HaCluster(self, 2, ha_replicate=None)
<<<<<<< HEAD
        c1 = cluster1[0].connect().session().sender("q;{create:always}")
        cluster2 = HaCluster(self, 2, ha_replicate="none")
        cluster2[0].connect().session().sender("q;{create:always}")
        time.sleep(.1)               # Give replication a chance.
        try:
            cluster1[1].connect_admin().session().receiver("q")
            self.fail("Excpected no-such-queue exception")
        except NotFound: pass
        try:
            cluster2[1].connect_admin().session().receiver("q")
            self.fail("Excpected no-such-queue exception")
        except NotFound: pass
=======
        cluster1[1].wait_status("ready")
        c1 = cluster1[0].connect().session().sender("q;{create:always}")
        cluster2 = HaCluster(self, 2, ha_replicate="none")
        cluster2[1].wait_status("ready")
        cluster2[0].connect().session().sender("q;{create:always}")
        time.sleep(.1)               # Give replication a chance.
        # Expect queues not to be found
        self.assertRaises(qm.NotFound, cluster1[1].connect_admin().session().receiver, "q")
        self.assertRaises(qm.NotFound, cluster2[1].connect_admin().session().receiver, "q")

    def test_replicate_binding(self):
        """Verify that binding replication can be disabled"""
        cluster = HaCluster(self, 2)
        primary, backup = cluster[0], cluster[1]
        ps = primary.connect().session()
        a = primary.agent
        a.addExchange("fanout", "ex")
        a.addQueue("q")
        a.bind("ex", "q", options={'qpid.replicate':'none'})
        backup.wait_backup("q")

        primary.kill()
        assert retry(lambda: not is_running(primary.pid)) # Wait for primary to die
        backup.promote()
        bs = backup.connect_admin().session()
        bs.sender("ex").send(qm.Message("msg"))
        self.assert_browse_retry(bs, "q", [])
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_invalid_replication(self):
        """Verify that we reject an attempt to declare a queue with invalid replication value."""
        cluster = HaCluster(self, 1, ha_replicate="all")
<<<<<<< HEAD
        try:
            c = cluster[0].connect().session().sender("q;{create:always, node:{x-declare:{arguments:{'qpid.replicate':XXinvalidXX}}}}")
            self.fail("Expected ConnectionError")
        except ConnectionError: pass
=======
        self.assertRaises(Exception, cluster[0].connect().session().sender,
                          "q;{create:always, node:{x-declare:{arguments:{'qpid.replicate':XXinvalidXX}}}}")
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_exclusive_queue(self):
        """Ensure that we can back-up exclusive queues, i.e. the replicating
        subscriptions are exempt from the exclusivity"""
        cluster = HaCluster(self, 2)
        def test(addr):
            c = cluster[0].connect()
            q = addr.split(";")[0]
            r = c.session().receiver(addr)
<<<<<<< HEAD
            try: c.session().receiver(addr); self.fail("Expected exclusive exception")
            except ReceiverError: pass
            s = c.session().sender(q).send(q)
            cluster[1].assert_browse_backup(q, [q])
        test("excl_sub;{create:always, link:{x-subscribe:{exclusive:True}}}");
        test("excl_queue;{create:always, node:{x-declare:{exclusive:True}}}")

    def test_auto_delete_exclusive(self):
        """Verify that we ignore auto-delete, exclusive, non-auto-delete-timeout queues"""
        cluster = HaCluster(self,2)
        s = cluster[0].connect().session()
        s.receiver("exad;{create:always,node:{x-declare:{exclusive:True,auto-delete:True}}}")
        s.receiver("ex;{create:always,node:{x-declare:{exclusive:True}}}")
        s.receiver("ad;{create:always,node:{x-declare:{auto-delete:True}}}")
        s.receiver("time;{create:always,node:{x-declare:{exclusive:True,auto-delete:True,arguments:{'qpid.auto_delete_timeout':1}}}}")
        s.receiver("q;{create:always}")

        s = cluster[1].connect_admin().session()
        cluster[1].wait_backup("q")
        assert not valid_address(s, "exad")
        assert valid_address(s, "ex")
        assert valid_address(s, "ad")
        assert valid_address(s, "time")
=======
            self.assertRaises(qm.LinkError, c.session().receiver, addr)
            s = c.session().sender(q).send(q)
            cluster[1].assert_browse_backup(q, [q])
        test("excl_queue;{create:always, node:{x-declare:{exclusive:True}}}")
        if qm == qpid.messaging: # FIXME aconway 2014-02-20: swig client no exclusive subscribe
            test("excl_sub;{create:always, link:{x-subscribe:{exclusive:True}}}");

    def test_auto_delete_exclusive(self):
        """Verify that we ignore auto-delete, exclusive, non-auto-delete-timeout queues"""
        cluster = HaCluster(self, 2)
        s0 = cluster[0].connect().session()
        s0.receiver("exad;{create:always,node:{x-declare:{exclusive:True,auto-delete:True}}}")
        s0.receiver("ex;{create:always,node:{x-declare:{exclusive:True}}}")
        ad = s0.receiver("ad;{create:always,node:{x-declare:{auto-delete:True}}}")
        s0.receiver("time;{create:always,node:{x-declare:{exclusive:True,auto-delete:True,arguments:{'qpid.auto_delete_timeout':1}}}}")
        s0.receiver("q;{create:always}")

        s1 = cluster[1].connect_admin().session()
        cluster[1].wait_backup("q")
        assert not valid_address(s1.connection, "exad")
        assert valid_address(s1.connection, "ex")
        assert valid_address(s1.connection, "ad")
        assert valid_address(s1.connection, "time")

        # Verify that auto-delete queues are not kept alive by
        # replicating subscriptions
        ad.close()
        s0.sync()
        assert not valid_address(s0.connection, "ad")
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_broker_info(self):
        """Check that broker information is correctly published via management"""
        cluster = HaCluster(self, 3)

<<<<<<< HEAD
        for broker in cluster:  # Make sure HA system-id matches broker's
            qmf = broker.agent().getHaBroker()
            self.assertEqual(qmf.systemId, UUID(broker.agent().getBroker().systemRef))

        cluster_ports = map(lambda b: b.port(), cluster)
        cluster_ports.sort()
        def ports(qmf):
            qmf.update()
            return sorted(map(lambda b: b["port"], qmf.members))
        # Check that all brokers have the same membership as the cluster
        for broker in cluster:
            qmf = broker.agent().getHaBroker()
            assert retry(lambda: cluster_ports == ports(qmf), 1), "%s != %s on %s"%(cluster_ports, ports(qmf), broker)
        # Add a new broker, check it is updated everywhere
        b = cluster.start()
        cluster_ports.append(b.port())
        cluster_ports.sort()
        for broker in cluster:
            qmf = broker.agent().getHaBroker()
            assert retry(lambda: cluster_ports == ports(qmf), 1), "%s != %s"%(cluster_ports, ports(qmf))

    def test_auth(self):
        """Verify that authentication does not interfere with replication."""
        # FIXME aconway 2012-07-09: generate test sasl config portably for cmake
=======
        def ha_broker(broker):
            ha_broker = broker.agent.getHaBroker();
            ha_broker.update()
            return ha_broker

        for broker in cluster:  # Make sure HA system-id matches broker's
            self.assertEqual(ha_broker(broker).systemId, UUID(broker.agent.getBroker().systemRef))

        # Check that all brokers have the same membership as the cluster
        def check_ids(broker):
            cluster_ids = set([ ha_broker(b).systemId for b in cluster])
            broker_ids = set([m["system-id"] for m in  ha_broker(broker).members])
            assert retry(lambda: cluster_ids == broker_ids, 1), "%s != %s on %s"%(cluster_ids, broker_ids, broker)

        for broker in cluster: check_ids(broker)

        # Add a new broker, check it is updated everywhere
        b = cluster.start()
        for broker in cluster: check_ids(broker)

    def test_auth(self):
        """Verify that authentication does not interfere with replication."""
        # TODO aconway 2012-07-09: generate test sasl config portably for cmake
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        sasl_config=os.path.join(self.rootdir, "sasl_config")
        if not os.path.exists(sasl_config):
            print "WARNING: Skipping test, SASL test configuration %s not found."%sasl_config
            return
        acl=os.path.join(os.getcwd(), "policy.acl")
        aclf=file(acl,"w")
<<<<<<< HEAD
        # Verify that replication works with auth=yes and HA user has at least the following
        # privileges:
        aclf.write("""
=======
        # Minimum set of privileges required for the HA user.
        aclf.write("""
# HA user
>>>>>>> 3bbfc42... Imported Upstream version 0.32
acl allow zag@QPID access queue
acl allow zag@QPID create queue
acl allow zag@QPID consume queue
acl allow zag@QPID delete queue
acl allow zag@QPID access exchange
acl allow zag@QPID create exchange
acl allow zag@QPID bind exchange
acl allow zag@QPID publish exchange
acl allow zag@QPID delete exchange
acl allow zag@QPID access method
acl allow zag@QPID create link
<<<<<<< HEAD
=======
acl allow zag@QPID access query
# Normal user
acl allow zig@QPID all all
>>>>>>> 3bbfc42... Imported Upstream version 0.32
acl deny all all
 """)
        aclf.close()
        cluster = HaCluster(
            self, 2,
            args=["--auth", "yes", "--sasl-config", sasl_config,
<<<<<<< HEAD
                  "--acl-file", acl, "--load-module", os.getenv("ACL_LIB"),
                  "--ha-username=zag", "--ha-password=zag", "--ha-mechanism=PLAIN"
                  ],
            client_credentials=Credentials("zag", "zag", "PLAIN"))
        s0 = cluster[0].connect(username="zag", password="zag").session();
        s0.receiver("q;{create:always}")
        s0.receiver("ex;{create:always,node:{type:topic,x-declare:{type:'fanout'},x-bindings:[{exchange:'ex',queue:'q'}]}}")
        cluster[1].wait_backup("q")
        cluster[1].wait_backup("ex")
        s1 = cluster[1].connect_admin().session(); # Uses Credentials above.
        s1.sender("ex").send("foo");
        self.assertEqual(s1.receiver("q").fetch().content, "foo")
=======
                  "--acl-file", acl,
                  "--ha-username=zag", "--ha-password=zag", "--ha-mechanism=PLAIN"
                  ],
            client_credentials=Credentials("zag", "zag", "PLAIN"))
        c = cluster[0].connect(username="zig", password="zig")
        s0 = c.session();
        a = cluster[0].agent
        a.addQueue("q")
        a.addExchange("fanout", "ex")
        a.bind("ex", "q", "")
        s0.sender("ex").send("foo");

        # Transactions should be done over the tx_protocol
        c = cluster[0].connect(protocol=self.tx_protocol, username="zig", password="zig")
        s1 = c.session(transactional=True)
        s1.sender("ex").send("foo-tx");
        cluster[1].assert_browse_backup("q", ["foo"])
        s1.commit()
        cluster[1].assert_browse_backup("q", ["foo", "foo-tx"])
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_alternate_exchange(self):
        """Verify that alternate-exchange on exchanges and queues is propagated
        to new members of a cluster. """
        cluster = HaCluster(self, 2)
        s = cluster[0].connect().session()
        # altex exchange: acts as alternate exchange
<<<<<<< HEAD
        s.sender("altex;{create:always,node:{type:topic,x-declare:{type:'fanout'}}}")
        # altq queue bound to altex, collect re-routed messages.
        s.sender("altq;{create:always,node:{x-bindings:[{exchange:'altex',queue:altq}]}}")
        # 0ex exchange with alternate-exchange altex and no queues bound
        s.sender("0ex;{create:always,node:{type:topic, x-declare:{type:'direct', alternate-exchange:'altex'}}}")
        # create queue q with alternate-exchange altex
        s.sender("q;{create:always,node:{type:queue, x-declare:{alternate-exchange:'altex'}}}")
        # create a bunch of exchanges to ensure we don't clean up prematurely if the
        # response comes in multiple fragments.
        for i in xrange(200): s.sender("00ex%s;{create:always,node:{type:topic}}"%i)

        def verify(broker):
            s = broker.connect().session()
            # Verify unmatched message goes to ex's alternate.
            s.sender("0ex").send("foo")
=======
        a = cluster[0].agent
        a.addExchange("fanout", "altex")
        # altq queue bound to altex, collect re-routed messages.
        a.addQueue("altq")
        a.bind("altex", "altq", "")
        # ex exchange with alternate-exchange altex and no queues bound
        a.addExchange("direct", "ex", {"alternate-exchange":"altex"})
        # create queue q with alternate-exchange altex
        a.addQueue("q", {"alternate-exchange":"altex"})
        # create a bunch of exchanges to ensure we don't clean up prematurely if the
        # response comes in multiple fragments.
        for i in xrange(200): s.sender("ex.%s;{create:always,node:{type:topic}}"%i)

        def verify(broker):
            c = broker.connect()
            s = c.session()
            # Verify unmatched message goes to ex's alternate.
            s.sender("ex").send("foo")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            altq = s.receiver("altq")
            self.assertEqual("foo", altq.fetch(timeout=0).content)
            s.acknowledge()
            # Verify rejected message goes to q's alternate.
            s.sender("q").send("bar")
            msg = s.receiver("q").fetch(timeout=0)
            self.assertEqual("bar", msg.content)
<<<<<<< HEAD
            s.acknowledge(msg, Disposition(REJECTED)) # Reject the message
            self.assertEqual("bar", altq.fetch(timeout=0).content)
            s.acknowledge()

        # Sanity check: alternate exchanges on original broker
        verify(cluster[0])
        # Check backup that was connected during setup.
        cluster[1].wait_backup("0ex")
        cluster[1].wait_backup("q")
        cluster.bounce(0)
        verify(cluster[1])
        # Check a newly started backup.
        cluster.start()
        cluster[2].wait_backup("0ex")
=======
            s.acknowledge(msg, qm.Disposition(qm.REJECTED)) # Reject the message
            self.assertEqual("bar", altq.fetch(timeout=0).content)
            s.acknowledge()
            c.close()

        # Sanity check: alternate exchanges on original broker
        verify(cluster[0])
        a = cluster[0].agent
        # Altex is in use as an alternate exchange, we should get an exception
        self.assertRaises(Exception, a.delExchange, "altex")
        # Check backup that was connected during setup.
        cluster[1].wait_status("ready")
        cluster[1].wait_backup("ex")
        cluster[1].wait_backup("q")
        cluster.bounce(0)
        verify(cluster[1])

        # Check a newly started backup.
        cluster.start()
        cluster[2].wait_status("ready")
        cluster[2].wait_backup("ex")
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        cluster[2].wait_backup("q")
        cluster.bounce(1)
        verify(cluster[2])

<<<<<<< HEAD
=======
        # Check that alt-exchange in-use count is replicated
        a = cluster[2].agent
        self.assertRaises(Exception, a.delExchange, "altex")
        a.delQueue("q")
        self.assertRaises(Exception, a.delExchange, "altex")
        a.delExchange("ex")
        a.delExchange("altex")

    def test_priority_reroute(self):
        """Regression test for QPID-4262, rerouting messages from a priority queue
        to itself causes a crash"""
        cluster = HaCluster(self, 2)
        primary = cluster[0]
        session = primary.connect().session()
        a = primary.agent
        a.addQueue("pq", {'qpid.priorities':10})
        a.bind("amq.fanout", "pq")
        s = session.sender("pq")
        for m in xrange(100): s.send(qm.Message(str(m), priority=m%10))
        pq =  QmfAgent(primary.host_port()).getQueue("pq")
        pq.reroute(request=0, useAltExchange=False, exchange="amq.fanout")
        # Verify that consuming is in priority order
        expect = [str(10*i+p) for p in xrange(9,-1,-1) for i in xrange(0,10) ]
        actual = [m.content for m in primary.get_messages("pq", 100)]
        self.assertEqual(expect, actual)

    def test_delete_missing_response(self):
        """Check that a backup correctly deletes leftover queues and exchanges that are
        missing from the initial reponse set."""
        # This test is a bit contrived, we set up the situation on backup brokers
        # and then promote one.
        cluster = HaCluster(self, 2, promote=False)

        # cluster[0] Will be the primary
        s = cluster[0].connect_admin().session()
        s.sender("q1;{create:always}")
        s.sender("e1;{create:always, node:{type:topic}}")

        # cluster[1] will be the backup, has extra queues/exchanges
        xdecl = "x-declare:{arguments:{'qpid.replicate':'all'}}"
        node = "node:{%s}"%(xdecl)
        s = cluster[1].connect_admin().session()
        s.sender("q1;{create:always, %s}"%(node))
        s.sender("q2;{create:always, %s}"%(node))
        s.sender("e1;{create:always, node:{type:topic, %s}}"%(xdecl))
        s.sender("e2;{create:always, node:{type:topic, %s}}"%(xdecl))
        for a in ["q1", "q2", "e1", "e2"]: cluster[1].wait_backup(a)

        cluster[0].promote()
        # Verify the backup deletes the surplus queue and exchange
        cluster[1].wait_status("ready")
        s = cluster[1].connect_admin().session()
        self.assertRaises(qm.NotFound, s.receiver, ("q2"));
        self.assertRaises(qm.NotFound, s.receiver, ("e2"));


    def test_delete_qpid_4285(self):
        """Regression test for QPID-4285: on deleting a queue it gets stuck in a
        partially deleted state and causes replication errors."""
        cluster = HaCluster(self,2)
        s = cluster[0].connect().session()
        s.receiver("q;{create:always}")
        cluster[1].wait_backup("q")
        cluster.kill(0)       # Make the backup take over.
        s = cluster[1].connect().session()
        cluster[1].agent.delQueue("q") # Delete q on new primary
        self.assertRaises(qm.NotFound, s.receiver, "q")
        assert not cluster[1].agent.getQueue("q") # Should not be in QMF

    def test_auto_delete_failover(self):
        """Test auto-delete queues. Verify that:
        - queues auto-deleted on the primary are deleted on the backup.
        - auto-delete queues with/without timeout are deleted after a failover correctly
        - auto-delete queues never used (subscribe to) to are not deleted
        - messages are correctly routed to the alternate exchange.
        """
        cluster = HaCluster(self, 3)
        s = cluster[0].connect().session()
        a = cluster[0].agent

        def setup(q, timeout=None):
            # Create alternate exchange, auto-delete queue and queue bound to alt. ex.
            a.addExchange("fanout", q+"-altex")
            args = {"auto-delete":True, "alternate-exchange":q+"-altex"}
            if timeout is not None: args['qpid.auto_delete_timeout'] = timeout
            a.addQueue(q, args)
            a.addQueue(q+"-altq")
            a.bind("%s-altex"%q, "%s-altq"%q)

        for args in [["q1"],["q2",0],["q3",1],["q4"],["q5"]]: setup(*args)
        receivers = []
        for i in xrange(1,5):   # Don't use q5
            q = "q%s"%i
            receivers.append(s.receiver(q)) # Subscribe
            qs = s.sender(q); qs.send(q); qs.close() # Send q name as message

        receivers[3].close()    # Trigger auto-delete for q4
        for b in cluster[1:3]: b.wait_no_queue("q4") # Verify deleted on backups

        cluster[0].kill(final=False) # Kill primary
        cluster[2].promote()
        cluster.restart(0)
        cluster[2].wait_queue("q3") # Not yet auto-deleted, 1 sec timeout.
        for b in cluster:
            for q in ["q%s"%i for i in xrange(1,5)]:
                b.wait_no_queue(q,timeout=2, msg=str(b))      # auto-deleted
                b.assert_browse_backup("%s-altq"%q, [q]) # Routed to alternate
        cluster[2].wait_queue("q5") # Not auto-deleted, never subscribed
        cluster[2].connect().session().receiver("q5").close()
        cluster[2].wait_no_queue("q5")

    def test_auto_delete_close(self):
        """Verify auto-delete queues are deleted on backup if auto-deleted
        on primary"""
        cluster=HaCluster(self, 2)

        # Create altex to use as alternate exchange, with altq bound to it
        a = cluster[0].agent
        a.addExchange("fanout", "altex")
        a.addQueue("altq", {"auto-delete":True})
        a.bind("altex", "altq")

        p = cluster[0].connect().session()
        r = p.receiver("adq1;{create:always,node:{x-declare:{auto-delete:True,alternate-exchange:'altex'}}}")
        s = p.sender("adq1")
        for m in ["aa","bb","cc"]: s.send(m)
        s.close()
        cluster[1].wait_queue("adq1")
        r.close()               # trigger auto-delete of adq1
        cluster[1].wait_no_queue("adq1")
        cluster[1].assert_browse_backup("altq", ["aa","bb","cc"])

    def test_expired(self):
        """Regression test for QPID-4379: HA does not properly handle expired messages"""
        # Race between messages expiring and HA replicating consumer.
        cluster = HaCluster(self, 2)
        s = cluster[0].connect().session().sender("q;{create:always}", capacity=2)
        def send_ttl_messages():
            for i in xrange(100): s.send(qm.Message(str(i), ttl=0.001))
        send_ttl_messages()
        cluster.start()
        send_ttl_messages()

    def test_stale_response(self):
        """Check for race condition where a stale response is processed after an
        event for the same queue/exchange """
        cluster = HaCluster(self, 2)
        s = cluster[0].connect().session()
        s.sender("keep;{create:always}") # Leave this queue in place.
        for i in xrange(100):
            q = "deleteme%s"%(i)
            cluster[0].agent.addQueue(q)
            cluster[0].agent.delQueue(q)
        # It is possible for the backup to attempt to subscribe after the queue
        # is deleted. This is not an error, but is logged as an error on the primary.
        # The backup does not log this as an error so we only check the backup log for errors.
        cluster[1].assert_log_clean()

    def test_missed_recreate(self):
        """If a queue or exchange is destroyed and one with the same name re-created
        while a backup is disconnected, the backup should also delete/recreate
        the object when it re-connects"""
        cluster = HaCluster(self, 3)
        sn = cluster[0].connect().session()
        # Create a queue with messages
        s = sn.sender("qq;{create:always}")
        msgs = [str(i) for i in xrange(3)]
        for m in msgs: s.send(m)
        cluster[1].assert_browse_backup("qq", msgs)
        cluster[2].assert_browse_backup("qq", msgs)
        # Set up an exchange with a binding.
        a = cluster[0].agent
        a.addExchange("fanout", "xx")
        a.addQueue("xxq")
        a.bind("xx", "xxq", "xxq")
        cluster[1].wait_address("xx")
        self.assertEqual(cluster[1].agent.getExchange("xx").values["bindingCount"], 1)
        cluster[2].wait_address("xx")
        self.assertEqual(cluster[2].agent.getExchange("xx").values["bindingCount"], 1)

        # Simulate the race by re-creating the objects before promoting the new primary
        cluster.kill(0, promote_next=False)
        xdecl = "x-declare:{arguments:{'qpid.replicate':'all'}}"
        node = "node:{%s}"%(xdecl)
        sn = cluster[1].connect_admin().session()
        a = cluster[1].agent
        a.delQueue("qq", if_empty=False)
        s = sn.sender("qq;{create:always, %s}"%(node))
        s.send("foo")
        a.delExchange("xx")
        sn.sender("xx;{create:always,node:{type:topic,%s}}"%(xdecl))
        cluster[1].promote()
        cluster[1].wait_status("active")
        # Verify we are not still using the old objects on cluster[2]
        cluster[2].assert_browse_backup("qq", ["foo"])
        cluster[2].wait_address("xx")
        self.assertEqual(cluster[2].agent.getExchange("xx").values["bindingCount"], 0)

    def test_resource_limit_bug(self):
        """QPID-5666 Regression test: Incorrect resource limit exception for queue creation."""
        cluster = HaCluster(self, 3)
        qs = ["q%s"%i for i in xrange(10)]
        a = cluster[0].agent
        a.addQueue("q")
        cluster.kill(0)
        cluster[1].promote()
        cluster[1].wait_status("active")
        a = cluster[1].agent
        a.delQueue("q")
        a.addQueue("q")

>>>>>>> 3bbfc42... Imported Upstream version 0.32
def fairshare(msgs, limit, levels):
    """
    Generator to return prioritised messages in expected order for a given fairshare limit
    """
    count = 0
    last_priority = None
    postponed = []
    while msgs or postponed:
        if not msgs:
            msgs = postponed
            count = 0
            last_priority = None
<<<<<<< HEAD
            postponed = []
=======
            postponed = [ ]
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        msg = msgs.pop(0)
        if last_priority and priority_level(msg.priority, levels) == last_priority:
            count += 1
        else:
            last_priority = priority_level(msg.priority, levels)
            count = 1
        l = limit(last_priority)
        if (l and count > l):
            postponed.append(msg)
        else:
            yield msg
    return

def priority_level(value, levels):
    """
    Method to determine which of a distinct number of priority levels
    a given value falls into.
    """
    offset = 5-math.ceil(levels/2.0)
    return min(max(value - offset, 0), levels-1)

<<<<<<< HEAD
class LongTests(BrokerTest):
=======
class LongTests(HaBrokerTest):
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    """Tests that can run for a long time if -DDURATION=<minutes> is set"""

    def duration(self):
        d = self.config.defines.get("DURATION")
        if d: return float(d)*60
        else: return 3                  # Default is to be quick

    def test_failover_send_receive(self):
        """Test failover with continuous send-receive"""
        brokers = HaCluster(self, 3)

        # Start sender and receiver threads
        n = 10
<<<<<<< HEAD
        senders = [NumberedSender(brokers[0], max_depth=1024, failover_updates=False,
                                 queue="test%s"%(i)) for i in xrange(n)]
        receivers = [NumberedReceiver(brokers[0], sender=senders[i],
                                      failover_updates=False,
                                      queue="test%s"%(i)) for i in xrange(n)]
=======
        senders = [
            NumberedSender(
                brokers[0], url=brokers.url,max_depth=50,
                queue="test%s"%(i), args=["--capacity=10"]) for i in xrange(n)]

        receivers = [
            NumberedReceiver(
                brokers[0], url=brokers.url, sender=senders[i],
                queue="test%s"%(i), args=["--capacity=10"]) for i in xrange(n)]

>>>>>>> 3bbfc42... Imported Upstream version 0.32
        for r in receivers: r.start()
        for s in senders: s.start()

        def wait_passed(r, n):
            """Wait for receiver r to pass n"""
            def check():
                r.check()       # Verify no exceptions
<<<<<<< HEAD
                return r.received > n
            assert retry(check), "Stalled %s at %s"%(r.queue, n)
=======
                return r.received > n + 100
            assert retry(check), "Stalled %s waiting for %s, sent %s"%(
                r.queue, n, [s for s in senders if s.queue==r.queue][0].sent)
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        for r in receivers: wait_passed(r, 0)

        # Kill and restart brokers in a cycle:
        endtime = time.time() + self.duration()
        i = 0
<<<<<<< HEAD
        try:
            while time.time() < endtime or i < 3: # At least 3 iterations
                for s in senders: s.sender.assert_running()
                for r in receivers: r.receiver.assert_running()
                checkpoint = [ r.received for r in receivers ]
                # Don't kill primary till it is active and the next
                # backup is ready, otherwise we can lose messages.
                brokers[i%3].wait_status("active")
                brokers[(i+1)%3].wait_status("ready")
                brokers.bounce(i%3)
                i += 1
                map(wait_passed, receivers, checkpoint) # Wait for all receivers
        except:
            traceback.print_exc()
            raise
        finally:
            for s in senders: s.stop()
            for r in receivers: r.stop()
            dead = []
            for i in xrange(3):
                if not brokers[i].is_running(): dead.append(i)
                brokers.kill(i, False)
            if dead: raise Exception("Brokers not running: %s"%dead)

class RecoveryTests(BrokerTest):
=======
        primary = 0
        try:
            try:
                while time.time() < endtime or i < 3: # At least 3 iterations
                    # Precondition: All 3 brokers running,
                    # primary = index of promoted primary
                    # one or two backups are running,
                    for s in senders: s.sender.assert_running()
                    for r in receivers: r.receiver.assert_running()
                    checkpoint = [ r.received+10 for r in receivers ]
                    victim = random.choice([0,1,2,primary]) # Give the primary a better chance.
                    if victim == primary:
                        # Don't kill primary till it is active and the next
                        # backup is ready, otherwise we can lose messages.
                        brokers[victim].wait_status("active")
                        next = (victim+1)%3
                        brokers[next].wait_status("ready")
                        brokers.bounce(victim) # Next one is promoted
                        primary = next
                    else:
                        brokers.bounce(victim, promote_next=False)

                    # Make sure we are not stalled
                    map(wait_passed, receivers, checkpoint)
                    # Run another checkpoint to ensure things work in this configuration
                    checkpoint = [ r.received+10 for r in receivers ]
                    map(wait_passed, receivers, checkpoint)
                    i += 1
            except:
                traceback.print_exc()
                raise
        finally:
            for s in senders: s.stop()
            for r in receivers: r.stop()
            dead = filter(lambda b: not b.is_running(), brokers)
            if dead: raise Exception("Brokers not running: %s"%dead)

    def test_tx_send_receive(self):
        brokers = HaCluster(self, 3)
        sender = self.popen(
            ["qpid-send",
             "--broker", brokers[0].host_port(),
             "--address", "q;{create:always}",
             "--messages=1000",
             "--tx=10"
             # TODO aconway 2014-02-21: can't use amqp1.0 for transactions yet
             ])
        receiver = self.popen(
            ["qpid-receive",
             "--broker", brokers[0].host_port(),
             "--address", "q;{create:always}",
             "--messages=990",
             "--timeout=10",
             "--tx=10"
             # TODO aconway 2014-02-21: can't use amqp1.0 for transactions yet
             ])
        self.assertEqual(sender.wait(), 0)
        self.assertEqual(receiver.wait(), 0)
        expect = [long(i) for i in range(991, 1001)]
        sn = lambda m: m.properties["sn"]
        brokers[0].assert_browse("q", expect, transform=sn)
        brokers[1].assert_browse_backup("q", expect, transform=sn)
        brokers[2].assert_browse_backup("q", expect, transform=sn)


    def test_qmf_order(self):
        """QPID 4402:  HA QMF events can be out of order.
        This test mimics the test described in the JIRA. Two threads repeatedly
        declare the same auto-delete queue and close their connection.
        """
        broker = Broker(self)
        class Receiver(Thread):
            def __init__(self, qname):
                Thread.__init__(self)
                self.qname = qname
                self.stopped = False

            def run(self):
                while not self.stopped:
                    self.connection = broker.connect()
                    try:
                        self.connection.session().receiver(
                            self.qname+";{create:always,node:{x-declare:{auto-delete:True}}}")
                    except qm.NotFound: pass # Can occur occasionally, not an error.
                    try: self.connection.close()
                    except: pass

        class QmfObject(object):
            """Track existance of an object and validate QMF events"""
            def __init__(self, type_name, name_field, name):
                self.type_name, self.name_field, self.name = type_name, name_field, name
                self.exists = False

            def qmf_event(self, event):
                content = event.content[0]
                event_type = content['_schema_id']['_class_name']
                values = content['_values']
                if event_type == self.type_name+"Declare" and values[self.name_field] == self.name:
                    disp = values['disp']
                    log.debug("Event %s: disp=%s exists=%s"%(
                            event_type, values['disp'], self.exists))
                    if self.exists: assert values['disp'] == 'existing'
                    else: assert values['disp'] == 'created'
                    self.exists = True
                elif event_type == self.type_name+"Delete" and values[self.name_field] == self.name:
                    log.debug("Event %s: exists=%s"%(event_type, self.exists))
                    assert self.exists
                    self.exists = False

        # Verify order of QMF events.
        helper = EventHelper()
        r = broker.connect().session().receiver(helper.eventAddress())
        threads = [Receiver("qq"), Receiver("qq")]
        for t in threads: t.start()
        queue = QmfObject("queue", "qName", "qq")
        finish = time.time() + self.duration()
        try:
            while time.time() < finish:
                queue.qmf_event(r.fetch())
        finally:
            for t in threads: t.stopped = True; t.join()

    def test_max_queues(self):
        """Verify that we behave properly if we try to exceed the max number
        of replicated queues - currently limited by the max number of channels
        in the replication link"""
        # This test is very slow (3 mins), skip it unless duration() > 1 minute.
        if self.duration() < 60: return
        # This test is written in C++ for speed, it takes a long time
        # to create 64k queues in python. See ha_test_max_queues.cpp.
        cluster = HaCluster(self, 2)
        test = self.popen(["ha_test_max_queues", cluster[0].host_port()])
        self.assertEqual(test.wait(), 0)

class RecoveryTests(HaBrokerTest):
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    """Tests for recovery after a failure."""

    def test_queue_hold(self):
        """Verify that the broker holds queues without sufficient backup,
        i.e. does not complete messages sent to those queues."""

<<<<<<< HEAD
        # We don't want backups to time out for this test, set long timeout.
        cluster = HaCluster(self, 4, args=["--ha-backup-timeout=100000"]);
        # Wait for the primary to be ready
        cluster[0].wait_status("active")
        # Create a queue before the failure.
        s1 = cluster.connect(0).session().sender("q1;{create:always}")
        for b in cluster: b.wait_backup("q1")
        for i in xrange(100): s1.send(str(i))
        # Kill primary and 2 backups
        for i in [0,1,2]: cluster.kill(i, False)
        cluster[3].promote()    # New primary, backups will be 1 and 2
        cluster[3].wait_status("recovering")

        def assertSyncTimeout(s):
            try:
                s.sync(timeout=.01)
                self.fail("Expected Timeout exception")
            except Timeout: pass

        # Create a queue after the failure
        s2 = cluster.connect(3).session().sender("q2;{create:always}")

        # Verify that messages sent are not completed
        for i in xrange(100,200): s1.send(str(i), sync=False); s2.send(str(i), sync=False)
        assertSyncTimeout(s1)
        self.assertEqual(s1.unsettled(), 100)
        assertSyncTimeout(s2)
        self.assertEqual(s2.unsettled(), 100)

        # Verify we can receive even if sending is on hold:
        cluster[3].assert_browse("q1", [str(i) for i in range(100)+range(100,200)])

        # Restart backups, verify queues are released only when both backups are up
        cluster.restart(1)
        assertSyncTimeout(s1)
        self.assertEqual(s1.unsettled(), 100)
        assertSyncTimeout(s2)
        self.assertEqual(s2.unsettled(), 100)
        self.assertEqual(cluster[3].ha_status(), "recovering")
        cluster.restart(2)

        # Verify everything is up to date and active
        def settled(sender): sender.sync(); return sender.unsettled() == 0;
        assert retry(lambda: settled(s1)), "Unsetttled=%s"%(s1.unsettled())
        assert retry(lambda: settled(s2)), "Unsetttled=%s"%(s2.unsettled())
        cluster[1].assert_browse_backup("q1", [str(i) for i in range(100)+range(100,200)])
        cluster[1].assert_browse_backup("q2", [str(i) for i in range(100,200)])
        cluster[3].wait_status("active"),
        s1.session.connection.close()
        s2.session.connection.close()
=======
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            # We don't want backups to time out for this test, set long timeout.
            cluster = HaCluster(self, 4, args=["--ha-backup-timeout=120"]);
            # Wait for the primary to be ready
            cluster[0].wait_status("active")
            for b in cluster[1:4]: b.wait_status("ready")
            # Create a queue before the failure.
            # FIXME aconway 2014-02-20: SWIG client doesn't respect sync=False
            s1 = cluster.connect(0, native=True).session().sender("q1;{create:always}")
            for b in cluster: b.wait_backup("q1")
            for i in xrange(10): s1.send(str(i), timeout=0.1)

            # Kill primary and 2 backups
            cluster[3].wait_status("ready")
            for i in [0,1,2]: cluster.kill(i, promote_next=False, final=False)
            cluster[3].promote()    # New primary, backups will be 1 and 2
            cluster[3].wait_status("recovering")

            def assertSyncTimeout(s):
                self.assertRaises(qpid.messaging.Timeout, s.sync, timeout=.01)

            # Create a queue after the failure
            # FIXME aconway 2014-02-20: SWIG client doesn't respect sync=False
            s2 = cluster.connect(3, native=True).session().sender("q2;{create:always}")

            # Verify that messages sent are not completed
            for i in xrange(10,20):
                s1.send(str(i), sync=False, timeout=0.1);
                s2.send(str(i), sync=False, timeout=0.1)

            assertSyncTimeout(s1)
            self.assertEqual(s1.unsettled(), 10)
            assertSyncTimeout(s2)
            self.assertEqual(s2.unsettled(), 10)

            # Verify we can receive even if sending is on hold:
            cluster[3].assert_browse("q1", [str(i) for i in range(10)])

            # Restart backups, verify queues are released only when both backups are up
            cluster.restart(1)
            assertSyncTimeout(s1)
            self.assertEqual(s1.unsettled(), 10)
            assertSyncTimeout(s2)
            self.assertEqual(s2.unsettled(), 10)
            cluster.restart(2)
            cluster.restart(0)

            # Verify everything is up to date and active
            def settled(sender): sender.sync(timeout=1); return sender.unsettled() == 0;
            assert retry(lambda: settled(s1)), "Unsetttled=%s"%(s1.unsettled())
            assert retry(lambda: settled(s2)), "Unsetttled=%s"%(s2.unsettled())
            cluster[1].assert_browse_backup("q1", [str(i) for i in range(10)+range(10,20)])
            cluster[1].assert_browse_backup("q2", [str(i) for i in range(10,20)])
            cluster[3].wait_status("active"),
            s1.session.connection.close()
            s2.session.connection.close()
        finally: l.restore()
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    def test_expected_backup_timeout(self):
        """Verify that we time-out expected backups and release held queues
        after a configured interval. Verify backup is demoted to catch-up,
        but can still rejoin.
        """
        cluster = HaCluster(self, 3, args=["--ha-backup-timeout=0.5"]);
<<<<<<< HEAD
        cluster[0].wait_status("active") # Primary ready
        for b in cluster[1:3]: b.wait_status("ready") # Backups ready
        for i in [0,1]: cluster.kill(i, False)
        cluster[2].promote()    # New primary, backups will be 1 and 2
        cluster[2].wait_status("recovering")
        # Should not go active till the expected backup connects or times out.
        self.assertEqual(cluster[2].ha_status(), "recovering")
        # Messages should be held expected backup times out
        s = cluster[2].connect().session().sender("q;{create:always}")
        for i in xrange(100): s.send(str(i), sync=False)
        # Verify message held initially.
        try: s.sync(timeout=.01); self.fail("Expected Timeout exception")
        except Timeout: pass
        s.sync(timeout=1)      # And released after the timeout.
        self.assertEqual(cluster[2].ha_status(), "active")

if __name__ == "__main__":
    shutil.rmtree("brokertest.tmp", True)
    qpid_ha = os.getenv("QPID_HA_EXEC")
    if  qpid_ha and os.path.exists(qpid_ha):
        os.execvp("qpid-python-test",
                  ["qpid-python-test", "-m", "ha_tests"] + sys.argv[1:])
    else:
        print "Skipping ha_tests, qpid_ha not available"
=======
        for i in [0,1]: cluster.kill(i, False)
        cluster[2].promote()    # New primary, expected backup will be 1
        # Should not go active till the expected backup connects or times out.
        cluster[2].wait_status("recovering")
        # Messages should be held till expected backup times out
        ss = cluster[2].connect().session()
        s = ss.sender("q;{create:always}")
        s.send("foo", sync=False)
        self.assertEqual(s.unsettled(), 1) # Verify message not settled immediately.
        s.sync(timeout=1)                  # And settled after timeout.
        cluster[2].wait_status("active")

    def test_join_ready_cluster(self):
        """If we join a cluster where the primary is dead, the new primary is
        not yet promoted and there are ready backups then we should refuse
        promotion so that one of the ready backups can be chosen."""
        cluster = HaCluster(self, 2)
        cluster[0].wait_status("active")
        cluster[1].wait_status("ready")
        cluster.bounce(0, promote_next=False)
        self.assertRaises(Exception, cluster[0].promote)
        os.kill(cluster[1].pid, signal.SIGSTOP) # Test for timeout if unresponsive.
        cluster.bounce(0, promote_next=False)
        cluster[0].promote()

    def test_stalled_backup(self):
        """Make sure that a stalled backup broker does not stall the primary"""
        cluster = HaCluster(self, 3, args=["--link-heartbeat-interval=1"])
        os.kill(cluster[1].pid, signal.SIGSTOP)
        s = cluster[0].connect().session()
        s.sender("q;{create:always}").send("x")
        self.assertEqual("x", s.receiver("q").fetch(0).content)

class StoreTests(HaBrokerTest):
    """Test for HA with persistence."""

    def check_skip(self):
        if not BrokerTest.store_lib:
            print "WARNING: skipping HA+store tests, no store lib found."
        return not BrokerTest.store_lib

    def test_store_recovery(self):
        """Verify basic store and recover functionality"""
        if self.check_skip(): return
        cluster = HaCluster(self, 1)
        sn = cluster[0].connect().session()
        # Create queue qq, exchange exx and binding between them
        s = sn.sender("qq;{create:always,node:{durable:true}}")
        sk = sn.sender("exx/k;{create:always,node:{type:topic, durable:true, x-declare:{type:'direct'}}}")
        cluster[0].agent.bind("exx", "qq", "k")
        for m in ["foo", "bar", "baz"]: s.send(qm.Message(m, durable=True))
        r = cluster[0].connect().session().receiver("qq")
        self.assertEqual(r.fetch().content, "foo")
        r.session.acknowledge()
        # Sending this message is a hack to flush the dequeue operation on qq.
        s.send(qm.Message("flush", durable=True))

        def verify(broker, x_count):
            sn = broker.connect().session()
            assert_browse(sn, "qq", [ "bar", "baz", "flush" ]+ (x_count)*["x"])
            sn.sender("exx/k").send(qm.Message("x", durable=True))
            assert_browse(sn, "qq", [ "bar", "baz", "flush" ]+ (x_count+1)*["x"])

        verify(cluster[0], 0)   # Sanity check
        cluster.bounce(0)
        cluster[0].wait_status("active")
        verify(cluster[0], 1)   # Loaded from store
        cluster.start()
        cluster[1].wait_status("ready")
        cluster.kill(0)
        cluster[1].wait_status("active")
        verify(cluster[1], 2)
        cluster.bounce(1, promote_next=False)
        cluster[1].promote()
        cluster[1].wait_status("active")
        verify(cluster[1], 3)

    def test_catchup_store(self):
        """Verify that a backup erases queue data from store recovery before
        doing catch-up from the primary."""
        if self.check_skip(): return
        cluster = HaCluster(self, 2, args=['--log-enable=trace+:ha', '--log-enable=trace+:Store'])
        sn = cluster[0].connect(heartbeat=HaBroker.heartbeat).session()
        s1 = sn.sender("q1;{create:always,node:{durable:true}}")
        for m in ["foo","bar"]: s1.send(qm.Message(m, durable=True))
        s2 = sn.sender("q2;{create:always,node:{durable:true}}")
        sk2 = sn.sender("ex/k2;{create:always,node:{type:topic, durable:true, x-declare:{type:'direct'}}}")
        cluster[0].agent.bind("ex", "q2", "k2")
        sk2.send(qm.Message("hello", durable=True))
        # Wait for backup to catch up.
        cluster[1].assert_browse_backup("q1", ["foo","bar"])
        cluster[1].assert_browse_backup("q2", ["hello"])
        # Make changes that the backup doesn't see
        cluster.kill(1, promote_next=False, final=False)
        r1 = cluster[0].connect(heartbeat=HaBroker.heartbeat).session().receiver("q1")
        for m in ["foo", "bar"]: self.assertEqual(r1.fetch().content, m)
        r1.session.acknowledge()
        for m in ["x","y","z"]: s1.send(qm.Message(m, durable=True))
        cluster[0].agent.unbind("ex", "q2", "k2")
        cluster[0].agent.bind("ex", "q1", "k1")
        # Restart both brokers from store to get inconsistent sequence numbering.
        cluster.bounce(0, promote_next=False)
        cluster[0].promote()
        cluster[0].wait_status("active")
        cluster.restart(1)
        cluster[1].wait_status("ready")

        # Verify state
        cluster[0].assert_browse("q1",  ["x","y","z"])
        cluster[1].assert_browse_backup("q1",  ["x","y","z"])

        sn = cluster[0].connect(heartbeat=HaBroker.heartbeat).session()
        sn.sender("ex/k1").send("boo")
        cluster[0].assert_browse_backup("q1", ["x","y","z", "boo"])
        cluster[1].assert_browse_backup("q1", ["x","y","z", "boo"])
        sn.sender("ex/k2").send("hoo") # q2 was unbound so this should be dropped.
        sn.sender("q2").send("end")    # mark the end of the queue for assert_browse
        cluster[0].assert_browse("q2", ["hello", "end"])
        cluster[1].assert_browse_backup("q2", ["hello", "end"])

def open_read(name):
    try:
        f = open(name)
        return f.read()
    finally: f.close()

class TransactionTests(HaBrokerTest):

    def tx_simple_setup(self, cluster, broker=0):
        """Start a transaction, remove messages from queue a, add messages to queue b"""
        c = cluster.connect(broker, protocol=self.tx_protocol)
        # Send messages to a, no transaction.
        sa = c.session().sender("a;{create:always,node:{durable:true}}")
        tx_msgs =  ["x","y","z"]
        for m in tx_msgs: sa.send(qm.Message(content=m, durable=True))
        sa.close()

        # Receive messages from a, in transaction.
        tx = c.session(transactional=True)
        txr = tx.receiver("a")
        tx_msgs2 = [txr.fetch(1).content for i in xrange(3)]
        self.assertEqual(tx_msgs, tx_msgs2)

        # Send messages to b, transactional, mixed with non-transactional.
        sb = c.session().sender("b;{create:always,node:{durable:true}}")
        txs = tx.sender("b")
        msgs = [str(i) for i in xrange(3)]
        for tx_m,m in zip(tx_msgs2, msgs):
            txs.send(tx_m);
            sb.send(m)
        sb.close()
        return tx

    def tx_subscriptions(self, broker):
        """Return list of queue names for tx subscriptions"""
        return [q for q in broker.agent.repsub_queues()
                    if q.startswith("qpid.ha-tx")]

    def test_tx_simple_commit(self):
        cluster = HaCluster(self, 2, test_store=True, wait=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()

        # NOTE: backup does not process transactional dequeues until prepare
        cluster[1].assert_browse_backup("a", ["x","y","z"])
        cluster[1].assert_browse_backup("b", ['0', '1', '2'])

        tx.acknowledge()
        tx.commit()
        tx.sync()
        tx.close()

        for b in cluster:
            self.assert_simple_commit_outcome(b, tx_queues)

        # Verify non-tx dequeue is replicated correctly
        c = cluster.connect(0, protocol=self.tx_protocol)
        r = c.session().receiver("b")
        ri = receiver_iter(r, timeout=1)
        self.assertEqual(['0', '1', '2', 'x', 'y', 'z'], [m.content for m in ri])
        r.session.acknowledge()
        for b in cluster: b.assert_browse_backup("b", [], msg=b)
        c.close()
        tx.connection.close()


    def check_enq_deq(self, cluster, queue, expect):
        for b in cluster:
            q = b.agent.getQueue(queue)
            self.assertEqual(
                (b.name,)+expect,
                (b.name, q.msgTotalEnqueues, q.msgTotalDequeues, q.msgTxnEnqueues, q.msgTxnDequeues))

    def test_tx_enq_notx_deq(self):
        """Verify that a non-tx dequeue of a tx enqueue is replicated correctly"""
        cluster = HaCluster(self, 2, test_store=True)
        c = cluster.connect(0, protocol=self.tx_protocol)

        tx = c.session(transactional=True)
        c.session().sender("qq;{create:always}").send("m1")
        tx.sender("qq;{create:always}").send("tx")
        tx.commit()
        tx.close()
        c.session().sender("qq;{create:always}").send("m2")
        self.check_enq_deq(cluster, 'qq', (3, 0, 1, 0))

        notx = c.session()
        self.assertEqual(['m1', 'tx', 'm2'], [m.content for m in receiver_iter(notx.receiver('qq'))])
        notx.acknowledge()
        self.check_enq_deq(cluster, 'qq', (3, 3, 1, 0))
        for b in cluster: b.assert_browse_backup('qq', [], msg=b)
        for b in cluster: self.assert_tx_clean(b)

    def test_tx_enq_notx_deq_qpid_send(self):
        """Verify that a non-tx dequeue of a tx enqueue is replicated correctly"""
        cluster = HaCluster(self, 2, test_store=True)

        self.popen(
            ['qpid-send', '-a', 'qq;{create:always}', '-b', cluster[0].host_port(), '--tx=1',
             '--content-string=foo']
        ).assert_exit_ok()
        for b in cluster: b.assert_browse_backup('qq', ['foo'], msg=b)
        self.check_enq_deq(cluster, 'qq', (1, 0, 1, 0))

        self.popen(['qpid-receive', '-a', 'qq', '-b', cluster[0].host_port()]).assert_exit_ok()
        self.check_enq_deq(cluster, 'qq', (1, 1, 1, 0))
        for b in cluster: b.assert_browse_backup('qq', [], msg=b)
        for b in cluster: self.assert_tx_clean(b)

    def assert_tx_clean(self, b):
        """Verify that there are no transaction artifacts
        (exchanges, queues, subscriptions) on b."""
        class FunctionCache:    # Call a function and cache the result.
            def __init__(self, f): self.f, self.value = f, None
            def __call__(self): self.value = self.f(); return self.value

        txq= FunctionCache(b.agent.tx_queues)
        assert retry(lambda: not txq()), "%s: unexpected %s"%(b, txq.value)
        txsub = FunctionCache(lambda: self.tx_subscriptions(b))
        assert retry(lambda: not txsub()), "%s: unexpected %s"%(b, txsub.value)
        # TODO aconway 2013-10-15: TX exchanges don't show up in management.

    def assert_simple_commit_outcome(self, b, tx_queues):
        b.assert_browse_backup("a", [], msg=b)
        b.assert_browse_backup("b", ['0', '1', '2', 'x', 'y', 'z'], msg=b)
        # Check for expected actions on the store
        expect = """<enqueue a x>
<enqueue a y>
<enqueue a z>
<begin tx 1>
<dequeue a x tx=1>
<dequeue a y tx=1>
<dequeue a z tx=1>
<commit tx=1>
"""
        self.assertEqual(expect, open_read(b.store_log), msg=b)
        self.assert_tx_clean(b)

    def test_tx_simple_rollback(self):
        cluster = HaCluster(self, 2, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.acknowledge()
        tx.rollback()
        tx.close()              # For clean test.
        for b in cluster: self.assert_simple_rollback_outcome(b, tx_queues)
        tx.connection.close()

    def assert_simple_rollback_outcome(self, b, tx_queues):
        b.assert_browse_backup("a", ["x","y","z"], msg=b)
        b.assert_browse_backup("b", ['0', '1', '2'], msg=b)
        # Check for expected actions on the store
        expect = """<enqueue a x>
<enqueue a y>
<enqueue a z>
"""
        self.assertEqual(open_read(b.store_log), expect, msg=b)
        self.assert_tx_clean(b)

    def test_tx_simple_failure(self):
        """Verify we throw TransactionAborted if there is a store error during a transaction"""
        cluster = HaCluster(self, 3, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.acknowledge()
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            cluster.bounce(0)       # Should cause roll-back
            tx.connection.session() # Wait for reconnect
            for b in cluster: self.assert_simple_rollback_outcome(b, tx_queues)
            self.assertRaises(qm.TransactionAborted, tx.sync)
            self.assertRaises(qm.TransactionAborted, tx.commit)
            try: tx.connection.close()
            except qm.TransactionAborted: pass # Occasionally get exception on close.
            for b in cluster: self.assert_simple_rollback_outcome(b, tx_queues)
        finally: l.restore()

    def test_tx_simple_failover(self):
        """Verify we throw TransactionAborted if there is a fail-over during a transaction"""
        cluster = HaCluster(self, 3, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.acknowledge()
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            cluster.bounce(0)       # Should cause roll-back
            tx.connection.session() # Wait for reconnect
            for b in cluster: self.assert_simple_rollback_outcome(b, tx_queues)
            self.assertRaises(qm.TransactionAborted, tx.sync)
            self.assertRaises(qm.TransactionAborted, tx.commit)
            try: tx.connection.close()
            except qm.TransactionAborted: pass # Occasionally get exception on close.
            for b in cluster: self.assert_simple_rollback_outcome(b, tx_queues)
        finally: l.restore()

    def test_tx_unknown_failover(self):
        """Verify we throw TransactionUnknown if there is a failure during commit"""
        cluster = HaCluster(self, 3, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.acknowledge()
        l = LogLevel(ERROR) # Hide expected WARNING log messages from failover.
        try:
            os.kill(cluster[2].pid, signal.SIGSTOP) # Delay prepare response
            class CommitThread(Thread):
                def run(self):
                    try: tx.commit()
                    except Exception, e:
                        self.error = e
            t = CommitThread()
            t.start()            # Commit in progress
            t.join(timeout=0.01)
            self.assertTrue(t.isAlive())
            cluster.bounce(0)
            os.kill(cluster[2].pid, signal.SIGCONT)
            t.join()
            try: raise t.error
            except qm.TransactionUnknown: pass
            for b in cluster: self.assert_tx_clean(b)
            try: tx.connection.close()
            except TransactionUnknown: pass # Occasionally get exception on close.
        finally: l.restore()

    def test_tx_no_backups(self):
        """Test the special case of a TX where there are no backups"""

        # Test commit
        cluster = HaCluster(self, 1, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.acknowledge()
        tx.commit()
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.close()
        self.assert_simple_commit_outcome(cluster[0], tx_queues)

        # Test rollback
        cluster = HaCluster(self, 1, test_store=True)
        tx = self.tx_simple_setup(cluster)
        tx.sync()
        tx_queues = cluster[0].agent.tx_queues()
        tx.acknowledge()
        tx.rollback()
        tx.sync()
        tx.close()
        self.assert_simple_rollback_outcome(cluster[0], tx_queues)

    def test_tx_backup_fail(self):
        cluster = HaCluster(self, 2, test_store=True, s_args=[[],["--test-store-name=bang"]])
        c = cluster[0].connect(protocol=self.tx_protocol)
        tx = c.session(transactional=True)
        s = tx.sender("q;{create:always,node:{durable:true}}")
        for m in ["foo","TEST_STORE_DO bang: throw","bar"]: s.send(qm.Message(m, durable=True))
        def commit_sync(): tx.commit(); tx.sync()
        self.assertRaises(qm.TransactionAborted, commit_sync)
        for b in cluster: b.assert_browse_backup("q", [])
        self.assertEqual(open_read(cluster[0].store_log), "<begin tx 1>\n<enqueue q foo tx=1>\n<enqueue q TEST_STORE_DO bang: throw tx=1>\n<enqueue q bar tx=1>\n<abort tx=1>\n")
        self.assertEqual(open_read(cluster[1].store_log), "<begin tx 1>\n<enqueue q foo tx=1>\n<enqueue q TEST_STORE_DO bang: throw tx=1>\n<abort tx=1>\n")

    def test_tx_join_leave(self):
        """Test cluster members joining/leaving cluster.
        Also check that tx-queues are cleaned up at end of transaction."""

        cluster = HaCluster(self, 3)

        # Leaving
        tx = cluster[0].connect(protocol=self.tx_protocol).session(transactional=True)
        s = tx.sender("q;{create:always}")
        s.send("a", sync=True)
        self.assertEqual([1,1,1], [len(b.agent.tx_queues()) for b in cluster])
        cluster[1].kill(final=False)
        s.send("b")
        tx.commit()
        tx.connection.close()
        for b in [cluster[0],cluster[2]]:
            self.assert_tx_clean(b)
            b.assert_browse_backup("q", ["a","b"], msg=b)
        # Joining
        tx = cluster[0].connect(protocol=self.tx_protocol).session(transactional=True)
        s = tx.sender("q;{create:always}")
        s.send("foo")
        cluster.restart(1)      # Not a part of the current transaction.
        tx.commit()
        tx.connection.close()
        for b in cluster: self.assert_tx_clean(b)
        # The new member is not in the tx but  receives the results normal replication.
        for b in cluster: b.assert_browse_backup("q", ["a", "b", "foo"], msg=b)

    def test_tx_block_threads(self):
        """Verify that TXs blocked in commit don't deadlock."""
        cluster = HaCluster(self, 2, args=["--worker-threads=2"], test_store=True)
        n = 10                  # Number of concurrent transactions
        sessions = [cluster.connect(0, protocol=self.tx_protocol).session(transactional=True) for i in xrange(n)]
        # Have the store delay the response for 10s
        for s in sessions:
            sn = s.sender("qq;{create:always,node:{durable:true}}")
            sn.send(qm.Message("foo", durable=True))
        self.assertEqual(n, len(cluster[1].agent.tx_queues()))
        threads = [ Thread(target=s.commit) for s in sessions]
        for t in threads: t.start()
        cluster[0].ready(timeout=1) # Check for deadlock
        for b in cluster: b.assert_browse_backup('qq', ['foo']*n)
        for t in threads: t.join()
        for s in sessions: s.connection.close()

    def test_other_tx_tests(self):
        try:
            import qpid_tests.broker_0_10
        except ImportError:
            raise Skipped("Tests not found")

        cluster = HaCluster(self, 3)
        self.popen(["qpid-txtest", "-p%s"%cluster[0].port()]).assert_exit_ok()
        self.popen(["qpid-python-test",
                    "-m", "qpid_tests.broker_0_10",
                    "-b", "localhost:%s"%(cluster[0].port()),
                    "*.tx.*"]).assert_exit_ok()

if __name__ == "__main__":
    qpid_ha_exec = os.getenv("QPID_HA_EXEC")
    if qpid_ha_exec and os.path.isfile(qpid_ha_exec):
        outdir = "ha_tests.tmp"
        shutil.rmtree(outdir, True)
        os.execvp("qpid-python-test",
                ["qpid-python-test", "-m", "ha_tests", "-DOUTDIR=%s"%outdir]
                  + sys.argv[1:])
    else:
        print "Skipping ha_tests, qpid-ha not available"

>>>>>>> 3bbfc42... Imported Upstream version 0.32

