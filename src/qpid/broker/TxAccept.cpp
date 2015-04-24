/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
<<<<<<< HEAD
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
=======
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
>>>>>>> 3bbfc42... Imported Upstream version 0.32
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */
#include "qpid/broker/TxAccept.h"
<<<<<<< HEAD
=======
#include "qpid/broker/TransactionObserver.h"
#include "qpid/broker/Queue.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/log/Statement.h"

using std::bind1st;
using std::bind2nd;
using std::mem_fun_ref;
using namespace qpid::broker;
using qpid::framing::SequenceSet;
using qpid::framing::SequenceNumber;

<<<<<<< HEAD
TxAccept::RangeOp::RangeOp(const AckRange& r) : range(r) {}

void TxAccept::RangeOp::prepare(TransactionContext* ctxt)
{
    for_each(range.start, range.end, bind(&DeliveryRecord::dequeue, _1, ctxt));
}

void TxAccept::RangeOp::commit()
{
    for_each(range.start, range.end, bind(&DeliveryRecord::committed, _1));
    for_each(range.start, range.end, bind(&DeliveryRecord::setEnded, _1));
}

TxAccept::RangeOps::RangeOps(DeliveryRecords& u) : unacked(u) {} 

void TxAccept::RangeOps::operator()(SequenceNumber start, SequenceNumber end)
{
    ranges.push_back(RangeOp(DeliveryRecord::findRange(unacked, start, end)));
}

void TxAccept::RangeOps::prepare(TransactionContext* ctxt)
{
    std::for_each(ranges.begin(), ranges.end(), bind(&RangeOp::prepare, _1, ctxt));
}

void TxAccept::RangeOps::commit()
{
    std::for_each(ranges.begin(), ranges.end(), bind(&RangeOp::commit, _1));
    //now remove if isRedundant():
    if (!ranges.empty()) {
        DeliveryRecords::iterator begin = ranges.front().range.start;
        DeliveryRecords::iterator end = ranges.back().range.end;
        DeliveryRecords::iterator removed = remove_if(begin, end, mem_fun_ref(&DeliveryRecord::isRedundant));
        unacked.erase(removed, end);
    }
}

TxAccept::TxAccept(const SequenceSet& _acked, DeliveryRecords& _unacked) : 
    acked(_acked), unacked(_unacked), ops(unacked) 
{
    //populate the ops
    acked.for_each(ops);
}

bool TxAccept::prepare(TransactionContext* ctxt) throw()
{
    try{
        ops.prepare(ctxt);
=======

TxAccept::TxAccept(const SequenceSet& _acked, DeliveryRecords& _unacked) :
    acked(_acked), unacked(_unacked)
{}

void TxAccept::each(boost::function<void(DeliveryRecord&)> f) {
    DeliveryRecords::iterator dr = unacked.begin();
    SequenceSet::iterator seq = acked.begin();
    while(dr != unacked.end() && seq != acked.end()) {
        if (dr->getId() == *seq) {
            f(*dr);
            ++dr;
            ++seq;
        }
        else if (dr->getId() < *seq) ++dr;
        else if (dr->getId() > *seq) ++seq;
    }
}

bool TxAccept::prepare(TransactionContext* ctxt) throw()
{
    try{
        each(bind(&DeliveryRecord::dequeue, _1, ctxt));
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        return true;
    }catch(const std::exception& e){
        QPID_LOG(error, "Failed to prepare: " << e.what());
        return false;
    }catch(...){
        QPID_LOG(error, "Failed to prepare");
        return false;
    }
}

<<<<<<< HEAD
void TxAccept::commit() throw() 
{
    try {
        ops.commit();
=======
void TxAccept::commit() throw()
{
    try {
        each(bind(&DeliveryRecord::committed, _1));
        each(bind(&DeliveryRecord::setEnded, _1));
        //now remove if isRedundant():
        if (!acked.empty()) {
            AckRange r = DeliveryRecord::findRange(unacked, acked.front(), acked.back());
            DeliveryRecords::iterator removed =
                remove_if(r.start, r.end, mem_fun_ref(&DeliveryRecord::isRedundant));
            unacked.erase(removed, r.end);
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } catch (const std::exception& e) {
        QPID_LOG(error, "Failed to commit: " << e.what());
    } catch(...) {
        QPID_LOG(error, "Failed to commit (unknown error)");
    }
}

void TxAccept::rollback() throw() {}
<<<<<<< HEAD
=======

namespace {
void callObserverDR(boost::shared_ptr<TransactionObserver> observer, DeliveryRecord& dr) {
    observer->dequeue(dr.getQueue(), dr.getMessageId(), dr.getReplicationId());
}
} // namespace

void TxAccept::callObserver(const ObserverPtr& observer) {
    each(boost::bind(&callObserverDR, observer, _1));
}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
