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

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
<<<<<<< HEAD
=======
#include <qpid/messaging/Message_io.h>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>

#include <iostream>

#include "OptionParser.h"

using namespace qpid::messaging;
using namespace qpid::types;

struct Options : OptionParser
{
    std::string url;
    std::string address;
    std::string connectionOptions;
    int timeout;
    bool forever;
    int count;

    Options()
        : OptionParser("Usage: drain [OPTIONS] ADDRESS", "Drains messages from the specified address"),
          url("127.0.0.1"),
          timeout(0),
          forever(false),
          count(0)
    {
        add("broker,b", url, "url of broker to connect to");
        add("timeout,t", timeout, "timeout in seconds to wait before exiting");
        add("forever,f", forever, "ignore timeout and wait forever");
        add("connection-options", connectionOptions, "connection options string in the form {name1:value1, name2:value2}");
        add("count,c", count, "number of messages to read before exiting");
    }

    Duration getTimeout()
    {
        if (forever) return Duration::FOREVER;
        else return timeout*Duration::SECOND;
    }

    int getCount()
    {
        return count;
    }
    
    bool checkAddress()
    {
        if (getArguments().empty()) {
            error("Address is required");
            return false;
        } else {
            address = getArguments()[0];
            return true;
        }
    }
};

int main(int argc, char** argv)
{
    Options options;
    if (options.parse(argc, argv) && options.checkAddress()) {
<<<<<<< HEAD
        Connection connection(options.url, options.connectionOptions);
        try {
=======
        Connection connection;
        try {
            connection = Connection(options.url, options.connectionOptions);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            connection.open();
            Session session = connection.createSession();
            Receiver receiver = session.createReceiver(options.address);
            Duration timeout = options.getTimeout();
            int count = options.getCount();
            Message message;
            int i = 0;
<<<<<<< HEAD
            
            while (receiver.fetch(message, timeout)) {
                std::cout << "Message(properties=" << message.getProperties() << ", content='" ;
                if (message.getContentType() == "amqp/map") {
                    Variant::Map map;
                    decode(message, map);
                    std::cout << map;
                } else {
                    std::cout << message.getContent();
                }
                std::cout  << "')" << std::endl;
=======

            while (receiver.fetch(message, timeout)) {
                std::cout << message << std::endl;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
                session.acknowledge();
                if (count && (++i == count))
                    break;
            }
            receiver.close();
<<<<<<< HEAD
            session.close();            
            connection.close();
            return 0;
        } catch(const std::exception& error) {
            std::cout << error.what() << std::endl;
            connection.close();
        }
    }
    return 1;   
=======
            session.close();
            connection.close();
            return 0;
        } catch(const std::exception& error) {
            std::cout << "Error: " << error.what() << std::endl;
            connection.close();
        }
    }
    return 1;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}
