#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "TestBlackboardValue"

#include <boost/test/unit_test.hpp>

#include "UUID.hpp"
#include "Utils.hpp"
#include "InterfaceImpl.hpp"
#include "IBlackboard.hpp"

#include "BlackboardUtils.hpp"

#include "AIServiceFixture.hpp"

#include <boost/any.hpp>
#include <boost/assign.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {


extern const base::UUID UUIDSemanticTypeA = base::utils::CreateUUIDFromString("{11111111-2222-3333-4444-555555555555}");
extern const base::UUID UUIDSemanticTypeB = base::utils::CreateUUIDFromString("{22222222-2222-3333-4444-555555555555}");
extern const base::UUID UUIDSemanticTypeC = base::utils::CreateUUIDFromString("{33333333-2222-3333-4444-555555555555}");
extern const base::UUID UUIDNil =           base::utils::CreateUUIDFromString("{00000000-0000-0000-0000-000000000000}");

BLACKBOARD_TYPE(IntVectorValue, std::vector<int>, UUIDSemanticTypeA);
BLACKBOARD_TYPE(StringValue,    std::string,      UUIDSemanticTypeB);


struct AIBlackboardFixture : public AIServiceFixture {

    AIBlackboardFixture()
    {
        blackboard = aiService->createBlackboard();
    }

    IBlackboardPtr blackboard;
};

BOOST_AUTO_TEST_CASE(TestBlackboardValueCreation) {

    IBlackboardValuePtr bbVal;
    BOOST_CHECK_NO_THROW(bbVal = support::createBlackboardValue<std::string>(UUIDSemanticTypeA, "FooBar"));
    BOOST_REQUIRE(bbVal);

    BOOST_CHECK(bbVal->GetID() != UUIDNil);
    BOOST_CHECK_EQUAL(bbVal->GetSemanticTypeID(), UUIDSemanticTypeA);
    boost::any anyVal;
    BOOST_CHECK_NO_THROW(anyVal = bbVal->GetValue());
    std::string* strPtr = boost::any_cast<std::string>(&anyVal);
    BOOST_REQUIRE(strPtr);
    BOOST_CHECK_EQUAL(*strPtr, "FooBar");
}

BOOST_AUTO_TEST_CASE(TestBlackboardValueRetrieval) {

    IBlackboardValuePtr bbVal;
    BOOST_CHECK_NO_THROW(bbVal = support::createBlackboardValue<std::string>(UUIDSemanticTypeA, "FooBar"));
    BOOST_REQUIRE(bbVal);
    
    std::string rawStringVal;
    BOOST_CHECK_NO_THROW(rawStringVal = support::getRawValue<std::string>(bbVal));
    BOOST_CHECK_EQUAL(rawStringVal, "FooBar");

    // Querying for another type should throw...
    int rawIntVal;
    BOOST_CHECK_THROW(rawIntVal = support::getRawValue<int>(bbVal), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(TestBlackboardValueAssignment) {

    IBlackboardValuePtr bbVal;
    BOOST_CHECK_NO_THROW(bbVal = support::createBlackboardValue<std::string>(UUIDSemanticTypeA, "FooBar"));
    BOOST_REQUIRE(bbVal);

    std::string rawVal;
    BOOST_CHECK_NO_THROW(rawVal = support::getRawValue<std::string>(bbVal));
    BOOST_CHECK_EQUAL(rawVal, "FooBar");

    bbVal->AssignValue(std::string("Foo42"));
    BOOST_CHECK_NO_THROW(rawVal = support::getRawValue<std::string>(bbVal));
    BOOST_CHECK_EQUAL(rawVal, "Foo42");
}


BOOST_FIXTURE_TEST_CASE(TestRetrievingValues, AIBlackboardFixture)
{
    // Set up the values to store.
    std::vector<int> initialIntValues(1, 42);
    IBlackboardValuePtr intValues = support::createBlackboardValue<IntVectorValue>(initialIntValues);
    blackboard->SetValue(intValues);

    std::string initialStringValue("foo");
    IBlackboardValuePtr stringValues = support::createBlackboardValue<StringValue>(initialStringValue);
    BOOST_CHECK_EQUAL(support::getRawValue<std::string>(stringValues), "foo");
    blackboard->SetValue(stringValues);

    // 1. Just try getting the "pointer" to the value.
    IBlackboardValuePtr blackBoardValuePointer;
    // 1a) Retrieving a unlisted one should just return an empty pointer.
    BOOST_CHECK_NO_THROW(blackBoardValuePointer = support::getValueFromBlackboard(blackboard, UUIDSemanticTypeC));
    BOOST_CHECK(!blackBoardValuePointer);

    // 1b) Retrieving a listed one should return some valid pointer.
    IBlackboardValuePtr intBlackBoardValuePtr;
    IBlackboardValuePtr stringBlackBoardValuePtr;
    BOOST_CHECK_NO_THROW(blackBoardValuePointer = support::getValueFromBlackboard(blackboard, UUIDSemanticTypeA));
    BOOST_CHECK(blackBoardValuePointer);
    BOOST_CHECK_NO_THROW(stringBlackBoardValuePtr = support::getValueFromBlackboard(blackboard, UUIDSemanticTypeB));
    BOOST_CHECK(stringBlackBoardValuePtr);

    // 2. Try converting values to some types.
    // 2a) Casting to a wrong type underneath should throw.
    std::string castedStringValue;
    std::vector<int> castedIntVectorValue;
    BOOST_CHECK_THROW(castedIntVectorValue = support::getRawValue<std::vector<int>>(stringBlackBoardValuePtr), std::runtime_error);

    // 2b) Casting to the appropriate type should succeed.
    BOOST_CHECK_NO_THROW(castedStringValue = support::getRawValue<std::string>(stringBlackBoardValuePtr));
    BOOST_CHECK_EQUAL(castedStringValue, "foo");

    // 3. Try all in one - get a value from the board and directly cast it to the type.
    // 3a) Not listed value should throw.
    std::string retrievedString;
    BOOST_CHECK_THROW(retrievedString = support::getRawValueFromBlackboard<std::string>(blackboard, UUIDSemanticTypeA), std::runtime_error);

    // 3b) Test successful case.
    BOOST_CHECK_NO_THROW(retrievedString = support::getRawValueFromBlackboard<std::string>(blackboard, UUIDSemanticTypeB));
    BOOST_CHECK_EQUAL(retrievedString, "foo");
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw