#ifndef QPID_MESSAGING_SENDERIMPL_H
#define QPID_MESSAGING_SENDERIMPL_H

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
#include "qpid/RefCounted.h"
<<<<<<< HEAD
=======
#include "qpid/sys/IntegerTypes.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

namespace qpid {
namespace messaging {

<<<<<<< HEAD
=======
class Address;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
class Message;
class Session;

class SenderImpl : public virtual qpid::RefCounted
{
  public:
    virtual ~SenderImpl() {}
    virtual void send(const Message& message, bool sync) = 0;
    virtual void close() = 0;
    virtual void setCapacity(uint32_t) = 0;
    virtual uint32_t getCapacity() = 0;
    virtual uint32_t getUnsettled() = 0;
    virtual const std::string& getName() const = 0;
    virtual Session getSession() const = 0;
<<<<<<< HEAD
=======
    virtual Address getAddress() const = 0;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
  private:
};
}} // namespace qpid::messaging

#endif  /*!QPID_MESSAGING_SENDERIMPL_H*/
