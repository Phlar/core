#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "TestA"

#include <boost/test/unit_test.hpp>

#include "InterfaceImpl.hpp"
#include "AIFactory.hpp"
#include "IBlackboard.hpp"

#include "BlackboardUtils.hpp"

#include <boost/assign.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {


extern const UUID UUIDSemanticTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
extern const UUID UUIDSemanticTypeB = boost::uuids::string_generator()("{22222222-2222-3333-4444-555555555555}");
extern const UUID UUIDSemanticTypeC = boost::uuids::string_generator()("{33333333-2222-3333-4444-555555555555}");

BLACKBOARD_TYPE(IntVectorValue, std::vector<int>, UUIDSemanticTypeA);
BLACKBOARD_TYPE(StringValue,    std::string,      UUIDSemanticTypeB);


struct AIBlackboardFixture {

    AIBlackboardFixture()
    {
        blackboard = aiFactory.createBlackboard();
    }

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};

BOOST_FIXTURE_TEST_CASE(TestRetrievingValues, AIBlackboardFixture)
{
    // Set up the values to store.
    std::vector<int> initialIntValues(1, 42);
    IntVectorValuePtr intValues = support::createBlackBoardValue<IntVectorValue>(initialIntValues);
    BOOST_CHECK_EQUAL_COLLECTIONS(intValues->GetValue().begin(), intValues->GetValue().end(),
        initialIntValues.begin(), initialIntValues.end());
    blackboard->SetValue(intValues);

    std::string initialStringValue("foo");
    StringValuePtr stringValues = support::createBlackBoardValue<StringValue>(initialStringValue);
    BOOST_CHECK_EQUAL(stringValues->GetValue(), "foo");
    blackboard->SetValue(stringValues);

    // 1. Just try getting the "pointer" to the value.
    IBlackboardValuePtr blackBoardValuePointer;
    // 1a) Retrieving a unlisted one should just return an empty pointer.
    BOOST_CHECK_NO_THROW(blackBoardValuePointer = support::getValue(blackboard, UUIDSemanticTypeC));
    BOOST_CHECK(!blackBoardValuePointer);

    // 1b) Retrieving a listed one should return some valid pointer.
    IBlackboardValuePtr intBlackBoardValuePtr;
    IBlackboardValuePtr stringBlackBoardValuePtr;
    BOOST_CHECK_NO_THROW(blackBoardValuePointer = support::getValue(blackboard, UUIDSemanticTypeA));
    BOOST_CHECK(blackBoardValuePointer);
    BOOST_CHECK_NO_THROW(stringBlackBoardValuePtr = support::getValue(blackboard, UUIDSemanticTypeB));
    BOOST_CHECK(stringBlackBoardValuePtr);

    // 2. Try converting values to some types.
    // 2a) Casting to a wrong type underneath should throw.
    std::string castedStringValue;
    std::vector<int> castedIntVectorValue;
    BOOST_CHECK_THROW(castedIntVectorValue = support::castBlackboardValueToType<std::vector<int>>(stringBlackBoardValuePtr), std::logic_error);

    // 2b) Casting to the appropriate type should succeed.
    BOOST_CHECK_NO_THROW(castedStringValue = support::castBlackboardValueToType<std::string>(stringBlackBoardValuePtr));
    BOOST_CHECK_EQUAL(castedStringValue, "foo");

    // 3. Try all in one - get a value from the board and directly cast it to the type.
    // 3a) Not listed value should throw.
    std::string retrievedString;
    BOOST_CHECK_THROW(retrievedString = support::getTypedValue<std::string>(blackboard, UUIDSemanticTypeA), std::logic_error);

    // 3b) Test successful case.
    BOOST_CHECK_NO_THROW(retrievedString = support::getTypedValue<std::string>(blackboard, UUIDSemanticTypeB));
    BOOST_CHECK_EQUAL(retrievedString, "foo");
}

BOOST_FIXTURE_TEST_CASE(TestUpdatingValue, AIBlackboardFixture)
{
    std::vector<int> initialValues;
    initialValues.push_back(10);
    initialValues.push_back(20);

    IntVectorValuePtr values = support::createBlackBoardValue<IntVectorValue>(initialValues);
    BOOST_CHECK_EQUAL(values->GetValue().size(), 2);

    // Add it to the blackboard.
    blackboard->SetValue(values);

    // Get the container as reference.
    std::vector<int>& refValues = support::getTypedValue<std::vector<int>>(blackboard, UUIDSemanticTypeA);
    BOOST_CHECK_EQUAL_COLLECTIONS(refValues.begin(), refValues.end(), initialValues.begin(), initialValues.end());

    // Modify it without touching the blackboard itself.
    refValues.push_back(30);

    // Get the container as value.
    std::vector<int> copyValues = support::getTypedValue<std::vector<int>>(blackboard, UUIDSemanticTypeA);
    BOOST_CHECK_EQUAL_COLLECTIONS(refValues.begin(), refValues.end(), copyValues.begin(), copyValues.end());
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw