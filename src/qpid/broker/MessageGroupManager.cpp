<<<<<<< HEAD
/*
=======
 /*
>>>>>>> 3bbfc42... Imported Upstream version 0.32
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

#include "qpid/broker/MessageGroupManager.h"
<<<<<<< HEAD

#include "qpid/broker/Queue.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/FieldValue.h"
=======
#include "qpid/broker/Message.h"
#include "qpid/broker/Messages.h"
#include "qpid/broker/MessageDeque.h"
#include "qpid/broker/QueueSettings.h"
#include "qpid/framing/Array.h"
#include "qpid/framing/DeliveryProperties.h"
#include "qpid/framing/FieldTable.h"
#include "qpid/framing/FieldValue.h"
#include "qpid/framing/TypeCode.h"
#include "qpid/types/Variant.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include "qpid/log/Statement.h"
#include "qpid/types/Variant.h"

using namespace qpid::broker;

namespace {
    const std::string GROUP_QUERY_KEY("qpid.message_group_queue");
    const std::string GROUP_HEADER_KEY("group_header_key");
    const std::string GROUP_STATE_KEY("group_state");
    const std::string GROUP_ID_KEY("group_id");
    const std::string GROUP_MSG_COUNT("msg_count");
    const std::string GROUP_TIMESTAMP("timestamp");
    const std::string GROUP_CONSUMER("consumer");
}


const std::string MessageGroupManager::qpidMessageGroupKey("qpid.group_header_key");
const std::string MessageGroupManager::qpidSharedGroup("qpid.shared_msg_group");
const std::string MessageGroupManager::qpidMessageGroupTimestamp("qpid.group_timestamp");


/** return an iterator to the message at position, or members.end() if not found */
MessageGroupManager::GroupState::MessageFifo::iterator
MessageGroupManager::GroupState::findMsg(const qpid::framing::SequenceNumber &position)
{
    MessageState mState(position);
    MessageFifo::iterator found = std::lower_bound(members.begin(), members.end(), mState);
    return (found->position == position) ? found : members.end();
}

void MessageGroupManager::unFree( const GroupState& state )
{
    GroupFifo::iterator pos = freeGroups.find( state.members.front().position );
    assert( pos != freeGroups.end() && pos->second == &state );
    freeGroups.erase( pos );
}

void MessageGroupManager::own( GroupState& state, const std::string& owner )
{
    state.owner = owner;
    unFree( state );
}

void MessageGroupManager::disown( GroupState& state )
{
    state.owner.clear();
    assert(state.members.size());
    assert(freeGroups.find(state.members.front().position) == freeGroups.end());
    freeGroups[state.members.front().position] = &state;
}

<<<<<<< HEAD
MessageGroupManager::GroupState& MessageGroupManager::findGroup( const QueuedMessage& qm )
{
    uint32_t thisMsg = qm.position.getValue();
=======
MessageGroupManager::GroupState& MessageGroupManager::findGroup( const Message& m )
{
    uint32_t thisMsg = m.getSequence().getValue();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (cachedGroup && lastMsg == thisMsg) {
        hits++;
        return *cachedGroup;
    }

<<<<<<< HEAD
    std::string group = defaultGroupId;
    const qpid::framing::FieldTable* headers = qm.payload->getApplicationHeaders();
    if (headers) {
        qpid::framing::FieldTable::ValuePtr id = headers->get( groupIdHeader );
        if (id && id->convertsTo<std::string>()) {
            std::string tmp = id->get<std::string>();
            if (!tmp.empty())   // empty group is reserved
                group = tmp;
        }
    }
=======
    std::string group = m.getPropertyAsString(groupIdHeader);
    if (group.empty()) group = defaultGroupId; //empty group is reserved
>>>>>>> 3bbfc42... Imported Upstream version 0.32

    if (cachedGroup && group == lastGroup) {
        hits++;
        lastMsg = thisMsg;
        return *cachedGroup;
    }

    misses++;

    GroupState& found = messageGroups[group];
    if (found.group.empty())
        found.group = group;    // new group, assign name
    lastMsg = thisMsg;
    lastGroup = group;
    cachedGroup = &found;
    return found;
}


<<<<<<< HEAD
void MessageGroupManager::enqueued( const QueuedMessage& qm )
{
    // @todo KAG optimization - store reference to group state in QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(qm);
    GroupState::MessageState mState(qm.position);
=======
void MessageGroupManager::enqueued( const Message& m )
{
    // @todo KAG optimization - store reference to group state in QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(m);
    GroupState::MessageState mState(m.getSequence());
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    state.members.push_back(mState);
    uint32_t total = state.members.size();
    QPID_LOG( trace, "group queue " << qName <<
              ": added message to group id=" << state.group << " total=" << total );
    if (total == 1) {
        // newly created group, no owner
<<<<<<< HEAD
        assert(freeGroups.find(qm.position) == freeGroups.end());
        freeGroups[qm.position] = &state;
=======
        assert(freeGroups.find(m.getSequence()) == freeGroups.end());
        freeGroups[m.getSequence()] = &state;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }
}


<<<<<<< HEAD
void MessageGroupManager::acquired( const QueuedMessage& qm )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(qm);
    GroupState::MessageFifo::iterator m = state.findMsg(qm.position);
    assert(m != state.members.end());
    m->acquired = true;
=======
void MessageGroupManager::acquired( const Message& m )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(m);
    GroupState::MessageFifo::iterator gm = state.findMsg(m.getSequence());
    assert(gm != state.members.end());
    gm->acquired = true;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    state.acquired += 1;
    QPID_LOG( trace, "group queue " << qName <<
              ": acquired message in group id=" << state.group << " acquired=" << state.acquired );
}


<<<<<<< HEAD
void MessageGroupManager::requeued( const QueuedMessage& qm )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(qm);
    assert( state.acquired != 0 );
    state.acquired -= 1;
    GroupState::MessageFifo::iterator m = state.findMsg(qm.position);
    assert(m != state.members.end());
    m->acquired = false;
=======
void MessageGroupManager::requeued( const Message& m )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(m);
    assert( state.acquired != 0 );
    state.acquired -= 1;
    GroupState::MessageFifo::iterator i = state.findMsg(m.getSequence());
    assert(i != state.members.end());
    i->acquired = false;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    if (state.acquired == 0 && state.owned()) {
        QPID_LOG( trace, "group queue " << qName <<
                  ": consumer name=" << state.owner << " released group id=" << state.group);
        disown(state);
    }
    QPID_LOG( trace, "group queue " << qName <<
              ": requeued message to group id=" << state.group << " acquired=" << state.acquired );
}


<<<<<<< HEAD
void MessageGroupManager::dequeued( const QueuedMessage& qm )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(qm);
    GroupState::MessageFifo::iterator m = state.findMsg(qm.position);
    assert(m != state.members.end());
    if (m->acquired) {
=======
void MessageGroupManager::dequeued( const Message& m )
{
    // @todo KAG  avoid lookup: retrieve direct reference to group state from QueuedMessage
    // issue: const-ness??
    GroupState& state = findGroup(m);
    GroupState::MessageFifo::iterator i = state.findMsg(m.getSequence());
    assert(i != state.members.end());
    if (i->acquired) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        assert( state.acquired != 0 );
        state.acquired -= 1;
    }

    // special case if qm is first (oldest) message in the group:
    // may need to re-insert it back on the freeGroups list, as the index will change
    bool reFreeNeeded = false;
<<<<<<< HEAD
    if (m == state.members.begin()) {
=======
    if (i == state.members.begin()) {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        if (!state.owned()) {
            // will be on the freeGroups list if mgmt is dequeueing rather than a consumer!
            // if on freelist, it is indexed by first member, which is about to be removed!
            unFree(state);
            reFreeNeeded = true;
        }
        state.members.pop_front();
    } else {
<<<<<<< HEAD
        state.members.erase(m);
=======
        state.members.erase(i);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    }

    uint32_t total = state.members.size();
    QPID_LOG( trace, "group queue " << qName <<
              ": dequeued message from group id=" << state.group << " total=" << total );

    if (total == 0) {
        QPID_LOG( trace, "group queue " << qName << ": deleting group id=" << state.group);
        if (cachedGroup == &state) {
            cachedGroup = 0;
        }
        std::string key(state.group);
        messageGroups.erase( key );
    } else if (state.acquired == 0 && state.owned()) {
        QPID_LOG( trace, "group queue " << qName <<
                  ": consumer name=" << state.owner << " released group id=" << state.group);
        disown(state);
<<<<<<< HEAD
=======
        MessageDeque* md = dynamic_cast<MessageDeque*>(&messages);
        if (md) {
            md->resetCursors();
        } else {
            QPID_LOG(warning, "Could not reset cursors for message group, unexpected container type");
        }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    } else if (reFreeNeeded) {
        disown(state);
    }
}

MessageGroupManager::~MessageGroupManager()
{
    QPID_LOG( debug, "group queue " << qName << " cache results: hits=" << hits << " misses=" << misses );
}
<<<<<<< HEAD
bool MessageGroupManager::nextConsumableMessage( Consumer::shared_ptr& c, QueuedMessage& next )
{
    if (!messages.size())
        return false;

    next.position = c->getPosition();
    if (!freeGroups.empty()) {
        const framing::SequenceNumber& nextFree = freeGroups.begin()->first;
        if (nextFree <= next.position) {  // take oldest free
            next.position = nextFree;
            --next.position;
        }
    }

    while (messages.browse( next.position, next, true )) {
        GroupState& group = findGroup(next);
        if (!group.owned()) {
            //TODO: make acquire more efficient when we already have the message in question
            if (group.members.front().position == next.position && messages.acquire(next.position, next)) {    // only take from head!
                return true;
            }
            QPID_LOG(debug, "Skipping " << next.position << " since group " << group.group
                     << "'s head message still pending. pos=" << group.members.front().position);
        } else if (group.owner == c->getName() && messages.acquire(next.position, next)) {
            return true;
        }
    }
    return false;
}


bool MessageGroupManager::allocate(const std::string& consumer, const QueuedMessage& qm)
{
    // @todo KAG avoid lookup: retrieve direct reference to group state from QueuedMessage
    GroupState& state = findGroup(qm);

    if (!state.owned()) {
        own( state, consumer );
        QPID_LOG( trace, "group queue " << qName <<
                  ": consumer name=" << consumer << " has acquired group id=" << state.group);
        return true;
    }
    return state.owner == consumer;
}

bool MessageGroupManager::nextBrowsableMessage( Consumer::shared_ptr& c, QueuedMessage& next )
{
    // browse: allow access to any available msg, regardless of group ownership (?ok?)
    return messages.browse(c->getPosition(), next, false);
=======

bool MessageGroupManager::acquire(const std::string& consumer, Message& m)
{
    if (m.getState() == AVAILABLE) {
        // @todo KAG avoid lookup: retrieve direct reference to group state from QueuedMessage
        GroupState& state = findGroup(m);

        if (!state.owned()) {
            own( state, consumer );
            QPID_LOG( trace, "group queue " << qName <<
                      ": consumer name=" << consumer << " has acquired group id=" << state.group);
        }
        if (state.owner == consumer) {
            m.setState(ACQUIRED);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

void MessageGroupManager::query(qpid::types::Variant::Map& status) const
{
    /** Add a description of the current state of the message groups for this queue.
        FORMAT:
        { "qpid.message_group_queue":
            { "group_header_key" : "<KEY>",
              "group_state" :
                   [ { "group_id"  : "<name>",
                       "msg_count" : <int>,
                       "timestamp" : <absTime>,
                       "consumer"  : <consumer name> },
                     {...} // one for each known group
                   ]
            }
        }
    **/

    assert(status.find(GROUP_QUERY_KEY) == status.end());
    qpid::types::Variant::Map state;
    qpid::types::Variant::List groups;

    state[GROUP_HEADER_KEY] = groupIdHeader;
    for (GroupMap::const_iterator g = messageGroups.begin();
         g != messageGroups.end(); ++g) {
        qpid::types::Variant::Map info;
        info[GROUP_ID_KEY] = g->first;
        info[GROUP_MSG_COUNT] = (uint64_t) g->second.members.size();
        // set the timestamp to the arrival timestamp of the oldest (HEAD) message, if present
        info[GROUP_TIMESTAMP] = 0;
        if (g->second.members.size() != 0) {
<<<<<<< HEAD
            QueuedMessage qm;
            if (messages.find(g->second.members.front().position, qm) &&
                qm.payload &&
                qm.payload->hasProperties<framing::DeliveryProperties>()) {
                info[GROUP_TIMESTAMP] = qm.payload->getProperties<framing::DeliveryProperties>()->getTimestamp();
=======
            Message* m = messages.find(g->second.members.front().position, 0);
            if (m && m->getTimestamp()) {
                info[GROUP_TIMESTAMP] = m->getTimestamp();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            }
        }
        info[GROUP_CONSUMER] = g->second.owner;
        groups.push_back(info);
    }
    state[GROUP_STATE_KEY] = groups;
    status[GROUP_QUERY_KEY] = state;
}


boost::shared_ptr<MessageGroupManager> MessageGroupManager::create( const std::string& qName,
                                                                    Messages& messages,
<<<<<<< HEAD
                                                                    const qpid::framing::FieldTable& settings )
{
    boost::shared_ptr<MessageGroupManager> empty;

    if (settings.isSet(qpidMessageGroupKey)) {

        // @todo: remove once "sticky" consumers are supported - see QPID-3347
        if (!settings.isSet(qpidSharedGroup)) {
            QPID_LOG( error, "Only shared groups are supported in this version of the broker. Use '--shared-groups' in qpid-config." );
            return empty;
        }

        std::string headerKey = settings.getAsString(qpidMessageGroupKey);
        if (headerKey.empty()) {
            QPID_LOG( error, "A Message Group header key must be configured, queue=" << qName);
            return empty;
        }
        unsigned int timestamp = settings.getAsInt(qpidMessageGroupTimestamp);

        boost::shared_ptr<MessageGroupManager> manager( new MessageGroupManager( headerKey, qName, messages, timestamp ) );

        QPID_LOG( debug, "Configured Queue '" << qName <<
                  "' for message grouping using header key '" << headerKey << "'" <<
                  " (timestamp=" << timestamp << ")");
        return manager;
    }
    return empty;
=======
                                                                    const QueueSettings& settings )
{
    boost::shared_ptr<MessageGroupManager> manager( new MessageGroupManager( settings.groupKey, qName, messages, settings.addTimestamp ) );
    QPID_LOG( debug, "Configured Queue '" << qName <<
              "' for message grouping using header key '" << settings.groupKey << "'" <<
              " (timestamp=" << settings.addTimestamp << ")");
    return manager;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

std::string MessageGroupManager::defaultGroupId;
void MessageGroupManager::setDefaults(const std::string& groupId)   // static
{
    defaultGroupId = groupId;
}

<<<<<<< HEAD
/** Cluster replication:

   state map format:

   { "group-state": [ {"name": <group-name>,
                       "owner": <consumer-name>-or-empty,
                       "acquired-ct": <acquired count>,
                       "positions": [Seqnumbers, ... ]},
                      {...}
                    ]
   }
*/

=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
namespace {
    const std::string GROUP_NAME("name");
    const std::string GROUP_OWNER("owner");
    const std::string GROUP_ACQUIRED_CT("acquired-ct");
    const std::string GROUP_POSITIONS("positions");
    const std::string GROUP_ACQUIRED_MSGS("acquired-msgs");
    const std::string GROUP_STATE("group-state");
}

<<<<<<< HEAD

/** Runs on UPDATER to snapshot current state */
void MessageGroupManager::getState(qpid::framing::FieldTable& state ) const
{
    using namespace qpid::framing;
    state.clear();
    framing::Array groupState(TYPE_CODE_MAP);
    for (GroupMap::const_iterator g = messageGroups.begin();
         g != messageGroups.end(); ++g) {

        framing::FieldTable group;
        group.setString(GROUP_NAME, g->first);
        group.setString(GROUP_OWNER, g->second.owner);
        group.setInt(GROUP_ACQUIRED_CT, g->second.acquired);
        framing::Array positions(TYPE_CODE_UINT32);
        framing::Array acquiredMsgs(TYPE_CODE_BOOLEAN);
        for (GroupState::MessageFifo::const_iterator p = g->second.members.begin();
             p != g->second.members.end(); ++p) {
            positions.push_back(framing::Array::ValuePtr(new IntegerValue( p->position )));
            acquiredMsgs.push_back(framing::Array::ValuePtr(new BoolValue( p->acquired )));
        }
        group.setArray(GROUP_POSITIONS, positions);
        group.setArray(GROUP_ACQUIRED_MSGS, acquiredMsgs);
        groupState.push_back(framing::Array::ValuePtr(new FieldTableValue(group)));
    }
    state.setArray(GROUP_STATE, groupState);

    QPID_LOG(debug, "Queue \"" << qName << "\": replicating message group state, key=" << groupIdHeader);
}


/** called on UPDATEE to set state from snapshot */
void MessageGroupManager::setState(const qpid::framing::FieldTable& state)
{
    using namespace qpid::framing;
    messageGroups.clear();
    freeGroups.clear();
    cachedGroup = 0;

    framing::Array groupState(TYPE_CODE_MAP);

    bool ok = state.getArray(GROUP_STATE, groupState);
    if (!ok) {
        QPID_LOG(error, "Unable to find message group state information for queue \"" <<
                 qName << "\": cluster inconsistency error!");
        return;
    }

    for (framing::Array::const_iterator g = groupState.begin();
         g != groupState.end(); ++g) {
        framing::FieldTable group;
        ok = framing::getEncodedValue<FieldTable>(*g, group);
        if (!ok) {
            QPID_LOG(error, "Invalid message group state information for queue \"" <<
                     qName << "\": table encoding error!");
            return;
        }
        MessageGroupManager::GroupState state;
        if (!group.isSet(GROUP_NAME) || !group.isSet(GROUP_OWNER) || !group.isSet(GROUP_ACQUIRED_CT)) {
            QPID_LOG(error, "Invalid message group state information for queue \"" <<
                     qName << "\": fields missing error!");
            return;
        }
        state.group = group.getAsString(GROUP_NAME);
        state.owner = group.getAsString(GROUP_OWNER);
        state.acquired = group.getAsInt(GROUP_ACQUIRED_CT);
        framing::Array positions(TYPE_CODE_UINT32);
        ok = group.getArray(GROUP_POSITIONS, positions);
        if (!ok) {
            QPID_LOG(error, "Invalid message group state information for queue \"" <<
                     qName << "\": position encoding error!");
            return;
        }
        framing::Array acquiredMsgs(TYPE_CODE_BOOLEAN);
        ok = group.getArray(GROUP_ACQUIRED_MSGS, acquiredMsgs);
        if (!ok || positions.count() != acquiredMsgs.count()) {
            QPID_LOG(error, "Invalid message group state information for queue \"" <<
                     qName << "\": acquired flag encoding error!");
            return;
        }

        Array::const_iterator a = acquiredMsgs.begin();
        for (Array::const_iterator p = positions.begin(); p != positions.end(); ++p) {
            GroupState::MessageState mState((*p)->getIntegerValue<uint32_t, 4>());
            mState.acquired = (*a++)->getIntegerValue<bool>();
            state.members.push_back(mState);
        }

        messageGroups[state.group] = state;
        if (!state.owned()) {
            assert(state.members.size());
            freeGroups[state.members.front().position] = &messageGroups[state.group];
        }
    }

    QPID_LOG(debug, "Queue \"" << qName << "\": message group state replicated, key =" << groupIdHeader)
}
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
