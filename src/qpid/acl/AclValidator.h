#ifndef QPID_ACL_ACLVALIDATOR_H
#define QPID_ACL_ACLVALIDATOR_H


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

#include "qpid/broker/AclModule.h"
#include "qpid/acl/AclData.h"
#include "qpid/sys/IntegerTypes.h"
#include <boost/shared_ptr.hpp>
<<<<<<< HEAD
=======
#include <boost/concept_check.hpp>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <vector>
#include <sstream>

namespace qpid {
namespace acl {

class AclValidator {

    /* Base Property */
<<<<<<< HEAD
   class PropertyType{
=======
    class PropertyType{
>>>>>>> 3bbfc42... Imported Upstream version 0.32

        public:
            virtual ~PropertyType(){};
            virtual bool validate(const std::string& val)=0;
            virtual std::string allowedValues()=0;
<<<<<<< HEAD
   };

   class IntPropertyType : public PropertyType{
=======
    };

    class IntPropertyType : public PropertyType{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            int64_t min;
            int64_t max;

        public:
            IntPropertyType(int64_t min,int64_t max);
            virtual ~IntPropertyType (){};
            virtual bool validate(const std::string& val);
            virtual std::string allowedValues();
<<<<<<< HEAD
   };

   class EnumPropertyType : public PropertyType{
=======
    };

    class EnumPropertyType : public PropertyType{
>>>>>>> 3bbfc42... Imported Upstream version 0.32
            std::vector<std::string> values;

        public:
            EnumPropertyType(std::vector<std::string>& allowed);
            virtual ~EnumPropertyType (){};
            virtual bool validate(const std::string& val);
            virtual std::string allowedValues();
<<<<<<< HEAD
   };

   typedef std::pair<acl::SpecProperty,boost::shared_ptr<PropertyType> > Validator;
   typedef std::map<acl::SpecProperty,boost::shared_ptr<PropertyType> > ValidatorMap;
   typedef ValidatorMap::iterator ValidatorItr;

   ValidatorMap validators;

public:

   void validateRuleSet(std::pair<const std::string, qpid::acl::AclData::ruleSet>& rules);
   void validateRule(qpid::acl::AclData::rule& rule);
   void validateProperty(std::pair<const qpid::acl::SpecProperty, std::string>& prop);
   void validate(boost::shared_ptr<AclData> d);
   AclValidator();
   ~AclValidator();
=======
    };

    typedef std::pair<acl::SpecProperty,boost::shared_ptr<PropertyType> > Validator;
    typedef std::map<acl::SpecProperty,boost::shared_ptr<PropertyType> > ValidatorMap;
    typedef ValidatorMap::iterator ValidatorItr;
    typedef boost::shared_ptr<std::set<Property> >         AllowedProperties    [ACTIONSIZE][OBJECTSIZE];
    typedef boost::shared_ptr<std::vector<AclData::Rule> > AllowedSpecProperties[ACTIONSIZE][OBJECTSIZE];

    ValidatorMap validators;
    AllowedProperties     allowedProperties;
    AllowedSpecProperties allowedSpecProperties;

public:

    void validateRuleSet(std::pair<const std::string, qpid::acl::AclData::ruleSet>& rules);
    void validateRule(qpid::acl::AclData::Rule& rule);
    void validateProperty(std::pair<const qpid::acl::SpecProperty, std::string>& prop);
    void validate(boost::shared_ptr<AclData> d);
    bool validateAllowedProperties(qpid::acl::Action action,
                                   qpid::acl::ObjectType object,
                                   const AclData::specPropertyMap& props,
                                   bool emitLog) const;
    void findPossibleLookupMatch(qpid::acl::Action action,
                                 qpid::acl::ObjectType object,
                                 const AclData::specPropertyMap& props,
                                 std::vector<int>& result) const;
    void tracePropertyDefs();

    AclValidator();
    ~AclValidator();

private:
    void registerProperties(const std::string& source,
                            const std::string& description,
                            Action action,
                            ObjectType object,
                            const std::string& properties = "");
    int propertyIndex;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
};

}} // namespace qpid::acl

#endif // QPID_ACL_ACLVALIDATOR_H
