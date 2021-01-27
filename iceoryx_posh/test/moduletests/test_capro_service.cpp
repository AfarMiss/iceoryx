// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test.hpp"

#include "iceoryx_posh/capro/service_description.hpp"
#include "iceoryx_utils/cxx/serialization.hpp"
#include "iceoryx_utils/cxx/string.hpp"

#include <cstdint>

using namespace ::testing;
using ::testing::Return;

using namespace iox::capro;

class ServiceDescription_test : public Test
{
  public:
    uint16_t testAnyServiceID = iox::capro::AnyService;
    uint16_t testAnyEventID = iox::capro::AnyEvent;
    uint16_t testAnyInstanceID = iox::capro::AnyInstance;
    IdString_t service1{"Service"};
    IdString_t instance1{"Instance"};
    IdString_t event1{"Event"};

    ServiceDescription csd1 = ServiceDescription(1, 2, 3);
    ServiceDescription csd1Eq = ServiceDescription(testAnyServiceID, 2, 3);
    ServiceDescription csd1Ne = ServiceDescription(testAnyServiceID, 9, 3);
    ServiceDescription csd2 = ServiceDescription(1, testAnyEventID, 3);
    ServiceDescription csd2Eq = ServiceDescription(1, 2, 3);
    ServiceDescription csd2Ne = ServiceDescription(1, 2, 9);
    ServiceDescription csd3 = ServiceDescription(1, 2, testAnyInstanceID);
    ServiceDescription csd3Eq = ServiceDescription(1, 2, testAnyInstanceID);
    ServiceDescription csd3Ne = ServiceDescription(9, 2, testAnyInstanceID);
    ServiceDescription csd4 = ServiceDescription(1, 2, 3);
    ServiceDescription csd4Eq = ServiceDescription(1, 2, 3);
    ServiceDescription csd4Ne = ServiceDescription(9, 9, 9);
    ServiceDescription csdIdStr = ServiceDescription(service1, instance1, event1);
    void SetUp(){};
    void TearDown(){};
};

TEST_F(ServiceDescription_test, ServiceDescriptionSerializationCtorCreatesServiceDescriptionWithValuesPassedToTheCtor)
{
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};
    IdString_t testService{"Service"};
    IdString_t testInstance{"Instance"};
    IdString_t testEvent{"Event"};
    uint16_t testServiceID = 1U;
    uint16_t testInstanceID = 2U;
    uint16_t testEventID = 3U;
    Scope testScope = Scope::INTERNAL;
    Interfaces testInterfaceSource = Interfaces::INTERNAL;
    auto serialObj = iox::cxx::Serialization::create(testService.c_str(),
                                                     testInstance.c_str(),
                                                     testEvent.c_str(),
                                                     testServiceID,
                                                     testInstanceID,
                                                     testEventID,
                                                     testHash[0],
                                                     testHash[1],
                                                     testHash[2],
                                                     testHash[3],
                                                     true,
                                                     static_cast<uint16_t>(testScope),
                                                     static_cast<uint16_t>(testInterfaceSource));

    ServiceDescription serviceDescription1 = ServiceDescription(serialObj);

    EXPECT_THAT(serviceDescription1.getServiceIDString(), Eq(testService));
    EXPECT_THAT(serviceDescription1.getInstanceIDString(), Eq(testInstance));
    EXPECT_THAT(serviceDescription1.getEventIDString(), Eq(testEvent));
    EXPECT_THAT(serviceDescription1.getServiceID(), Eq(testServiceID));
    EXPECT_THAT(serviceDescription1.getInstanceID(), Eq(testInstanceID));
    EXPECT_THAT(serviceDescription1.getEventID(), Eq(testEventID));
    EXPECT_THAT((serviceDescription1.getClassHash())[0], Eq(testHash[0]));
    EXPECT_THAT((serviceDescription1.getClassHash())[1], Eq(testHash[1]));
    EXPECT_THAT((serviceDescription1.getClassHash())[2], Eq(testHash[2]));
    EXPECT_THAT((serviceDescription1.getClassHash())[3], Eq(testHash[3]));
    EXPECT_THAT(serviceDescription1.hasServiceOnlyDescription(), Eq(true));
    EXPECT_THAT(serviceDescription1.getScope(), Eq(Scope::INTERNAL));
    EXPECT_THAT(serviceDescription1.getSourceInterface(), Eq(Interfaces::INTERNAL));
}

TEST_F(ServiceDescription_test, ServiceDescriptionObjectInitialisationWithOutOfBoundaryScopeSetsTheScopeToInvalid)
{
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};
    IdString_t testService{"Service"};
    IdString_t testInstance{"Instance"};
    IdString_t testEvent{"Event"};
    uint16_t testServiceID = 1U;
    uint16_t testInstanceID = 2U;
    uint16_t testEventID = 3U;
    uint16_t invalidScope = 3U;
    auto serialObj = iox::cxx::Serialization::create(testService.c_str(),
                                                     testInstance.c_str(),
                                                     testEvent.c_str(),
                                                     testServiceID,
                                                     testInstanceID,
                                                     testEventID,
                                                     testHash[0],
                                                     testHash[1],
                                                     testHash[2],
                                                     testHash[3],
                                                     true,
                                                     invalidScope);

    ServiceDescription serviceDescription1 = ServiceDescription(serialObj);

    EXPECT_THAT(serviceDescription1.getScope(), Eq(Scope::INVALID));
}

TEST_F(ServiceDescription_test,
       ServiceDescriptionObjectInitialisationWithOutOfBoundaryInterfaceSourceSetsTheInterfaceSourceToInterfaceEnd)
{
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};
    IdString_t testService{"Service"};
    IdString_t testInstance{"Instance"};
    IdString_t testEvent{"Event"};
    uint16_t testServiceID = 1U;
    uint16_t testInstanceID = 2U;
    uint16_t testEventID = 3U;
    uint16_t testScope = 2U;
    uint16_t invalidInterfaceSource = 10U;
    auto serialObj = iox::cxx::Serialization::create(testService.c_str(),
                                                     testInstance.c_str(),
                                                     testEvent.c_str(),
                                                     testServiceID,
                                                     testInstanceID,
                                                     testEventID,
                                                     testHash[0],
                                                     testHash[1],
                                                     testHash[2],
                                                     testHash[3],
                                                     true,
                                                     static_cast<uint16_t>(testScope),
                                                     invalidInterfaceSource);

    ServiceDescription serviceDescription1 = ServiceDescription(serialObj);

    EXPECT_THAT(serviceDescription1.getSourceInterface(), Eq(Interfaces::INTERFACE_END));
}

TEST_F(ServiceDescription_test,
       ServiceDescriptionCtorWithOnlyIDsCreatesServiceDescriptionWithTheSameServiceEventAndInstanceStringsAsIDs)
{
    uint16_t testServiceID = 1U;
    uint16_t testEventID = 2U;
    uint16_t testInstanceID = 3U;

    ServiceDescription serviceDescription1 = ServiceDescription(testServiceID, testEventID, testInstanceID);

    EXPECT_THAT(std::to_string(serviceDescription1.getServiceID()), serviceDescription1.getServiceIDString());
    EXPECT_THAT(std::to_string(serviceDescription1.getEventID()), serviceDescription1.getEventIDString());
    EXPECT_THAT(std::to_string(serviceDescription1.getInstanceID()), serviceDescription1.getInstanceIDString());
}

TEST_F(ServiceDescription_test, ServiceDescriptionDefaultCtorInitializesTheIDsAndStringsToZero)
{
    ServiceDescription serviceDescription1 = ServiceDescription();

    EXPECT_EQ(serviceDescription1.getServiceID(), 0);
    EXPECT_EQ(serviceDescription1.getEventID(), 0);
    EXPECT_EQ(serviceDescription1.getInstanceID(), 0);
    EXPECT_THAT(serviceDescription1.getServiceIDString(), StrEq("0"));
    EXPECT_THAT(serviceDescription1.getEventIDString(), StrEq("0"));
    EXPECT_THAT(serviceDescription1.getInstanceIDString(), StrEq("0"));
}

TEST_F(ServiceDescription_test, ServiceDescriptionStringCtorCreatesServiceDescriptionWithValuesPassedToTheCtor)
{
    IdString_t testService("1");
    IdString_t testInstance("2");
    IdString_t testEvent("3");
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};

    ServiceDescription serviceDescription1 = ServiceDescription(testService, testInstance, testEvent, testHash);

    EXPECT_THAT(serviceDescription1.getServiceIDString(), StrEq("1"));
    EXPECT_THAT(serviceDescription1.getInstanceIDString(), StrEq("2"));
    EXPECT_THAT(serviceDescription1.getEventIDString(), StrEq("3"));
    EXPECT_EQ(uint16_t(1), serviceDescription1.getServiceID());
    EXPECT_EQ(uint16_t(2), serviceDescription1.getInstanceID());
    EXPECT_EQ(uint16_t(3), serviceDescription1.getEventID());
    EXPECT_EQ(uint32_t(1), serviceDescription1.getClassHash()[0]);
    EXPECT_EQ(uint32_t(2), serviceDescription1.getClassHash()[1]);
    EXPECT_EQ(uint32_t(3), serviceDescription1.getClassHash()[2]);
    EXPECT_EQ(uint32_t(4), serviceDescription1.getClassHash()[3]);
}
TEST_F(ServiceDescription_test, ServiceDescriptionStringCtorWithNonIntegerStringValuesSetTheIDsToInvalid)
{
    IdString_t testService("Service");
    IdString_t testInstance("Instance");
    IdString_t testEvent("Event");
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};

    ServiceDescription serviceDescription1 = ServiceDescription(testService, testInstance, testEvent, testHash);

    EXPECT_EQ(InvalidID, serviceDescription1.getServiceID());
    EXPECT_EQ(InvalidID, serviceDescription1.getInstanceID());
    EXPECT_EQ(InvalidID, serviceDescription1.getEventID());
}

TEST_F(ServiceDescription_test, ServiceDescriptionStringCtorWithOutOfBoundaryIntegerStringValuesSetTheIDsToInvalid)
{
    IdString_t testService("65536");
    IdString_t testInstance("65536");
    IdString_t testEvent("65536");
    ServiceDescription::ClassHash testHash = {1, 2, 3, 4};

    ServiceDescription serviceDescription1 = ServiceDescription(testService, testInstance, testEvent, testHash);

    EXPECT_EQ(InvalidID, serviceDescription1.getServiceID());
    EXPECT_EQ(InvalidID, serviceDescription1.getInstanceID());
    EXPECT_EQ(InvalidID, serviceDescription1.getEventID());
}


TEST_F(ServiceDescription_test, operatorEq)
{
    EXPECT_TRUE(csd1 == csd1Eq);
    EXPECT_TRUE(csd2 == csd2Eq);
    EXPECT_TRUE(csd3 == csd3Eq);
    EXPECT_TRUE(csd4 == csd4Eq);

    EXPECT_FALSE(csd1 == csd1Ne);
    EXPECT_FALSE(csd2 == csd2Ne);
    EXPECT_FALSE(csd3 == csd3Ne);
    EXPECT_FALSE(csd4 == csd4Ne);
}

TEST_F(ServiceDescription_test, operatorNe)
{
    EXPECT_FALSE(csd1 != csd1Eq);
    EXPECT_FALSE(csd2 != csd2Eq);
    EXPECT_FALSE(csd3 != csd3Eq);
    EXPECT_FALSE(csd4 != csd4Eq);

    EXPECT_TRUE(csd1 != csd1Ne);
    EXPECT_TRUE(csd2 != csd2Ne);
    EXPECT_TRUE(csd3 != csd3Ne);
    EXPECT_TRUE(csd4 != csd4Ne);
}

TEST_F(ServiceDescription_test, operatorAssign)
{
    ServiceDescription csdNew = csd1;
    EXPECT_TRUE(csdNew == csd1);
}

TEST_F(ServiceDescription_test, CreateServiceOnlyDescription)
{
    ServiceDescription desc1(1u, 2u);
    ServiceDescription desc2("bla", "fuh");
    ServiceDescription desc3(1u, 2u, 3u);
    ServiceDescription desc4("bla", "fuh", "dumbledoodoo");

    EXPECT_TRUE(desc1.hasServiceOnlyDescription());
    EXPECT_TRUE(desc2.hasServiceOnlyDescription());
    EXPECT_FALSE(desc3.hasServiceOnlyDescription());
    EXPECT_FALSE(desc4.hasServiceOnlyDescription());
}

TEST_F(ServiceDescription_test, defaultClassHash)
{
    EXPECT_EQ(uint32_t(0), csdIdStr.getClassHash()[0]);
    EXPECT_EQ(uint32_t(0), csdIdStr.getClassHash()[1]);
    EXPECT_EQ(uint32_t(0), csdIdStr.getClassHash()[2]);
    EXPECT_EQ(uint32_t(0), csdIdStr.getClassHash()[3]);
}

TEST_F(ServiceDescription_test, CopyAssignmentClassHash)
{
    ServiceDescription desc1("TestService", "TestInstance", "TestEvent", {1, 2, 3, 4});
    ServiceDescription desc2 = desc1;
    EXPECT_EQ(desc2.getClassHash()[0], desc1.getClassHash()[0]);
    EXPECT_EQ(desc2.getClassHash()[1], desc1.getClassHash()[1]);
    EXPECT_EQ(desc2.getClassHash()[2], desc1.getClassHash()[2]);
    EXPECT_EQ(desc2.getClassHash()[3], desc1.getClassHash()[3]);
}
