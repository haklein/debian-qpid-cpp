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
#ifndef _DtxWorkRecord_
#define _DtxWorkRecord_

#include "qpid/broker/BrokerImportExport.h"
#include "qpid/broker/DtxBuffer.h"
<<<<<<< HEAD
#include "qpid/broker/DtxTimeout.h"
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/broker/TransactionalStore.h"

#include "qpid/framing/amqp_types.h"
#include "qpid/sys/Mutex.h"

#include <algorithm>
#include <functional>
#include <vector>

#include <boost/intrusive_ptr.hpp>

namespace qpid {
namespace broker {

<<<<<<< HEAD
=======
struct DtxTimeout;

>>>>>>> 3bbfc42... Imported Upstream version 0.32
/**
 * Represents the work done under a particular distributed transaction
 * across potentially multiple channels. Identified by a xid. Allows
 * that work to be prepared, committed and rolled-back.
 */
class DtxWorkRecord
{
<<<<<<< HEAD
    typedef std::vector<DtxBuffer::shared_ptr> Work;
=======
    typedef std::vector<boost::intrusive_ptr<DtxBuffer> >Work;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    const std::string xid;
    TransactionalStore* const store;
    bool completed;
    bool rolledback;
    bool prepared;
    bool expired;
    boost::intrusive_ptr<DtxTimeout> timeout;
    Work work;
    std::auto_ptr<TPCTransactionContext> txn;
    qpid::sys::Mutex lock;

    bool check();
    void abort();
    bool prepare(TransactionContext* txn);
public:
    QPID_BROKER_EXTERN DtxWorkRecord(const std::string& xid,
                                     TransactionalStore* const store);
    QPID_BROKER_EXTERN ~DtxWorkRecord();
    QPID_BROKER_EXTERN bool prepare();
    QPID_BROKER_EXTERN bool commit(bool onePhase);
    QPID_BROKER_EXTERN void rollback();
<<<<<<< HEAD
    QPID_BROKER_EXTERN void add(DtxBuffer::shared_ptr ops);
    void recover(std::auto_ptr<TPCTransactionContext> txn, DtxBuffer::shared_ptr ops);
    void timedout();
    void setTimeout(boost::intrusive_ptr<DtxTimeout> t) { timeout = t; }
    boost::intrusive_ptr<DtxTimeout> getTimeout() { return timeout; }
=======
    QPID_BROKER_EXTERN void add(boost::intrusive_ptr<DtxBuffer> ops);
    void recover(std::auto_ptr<TPCTransactionContext> txn, boost::intrusive_ptr<DtxBuffer> ops);
    void timedout();
    void setTimeout(boost::intrusive_ptr<DtxTimeout> t);
    boost::intrusive_ptr<DtxTimeout> getTimeout();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    std::string getXid() const { return xid; }
    bool isCompleted() const { return completed; }
    bool isRolledback() const { return rolledback; }
    bool isPrepared() const { return prepared; }
    bool isExpired() const { return expired; }
<<<<<<< HEAD

    // Used by cluster update;
    size_t size() const { return work.size(); }
    DtxBuffer::shared_ptr operator[](size_t i) const;
    uint32_t getTimeout() const { return timeout? timeout->timeout : 0; }
    size_t indexOf(const DtxBuffer::shared_ptr&);
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}} // qpid::broker

#endif
