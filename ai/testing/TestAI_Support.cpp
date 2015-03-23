#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "TestA"

#include <boost/test/unit_test.hpp>

#include "InterfaceImpl.hpp"
#include "AIFactory.hpp"
#include "IBlackboard.hpp"
#include "ITaskParameter.hpp"

#include "BlackboardUtils.hpp"

#include <boost/assign.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

namespace {

    struct WindProperties {

        WindProperties(const std::string name, float value)
            : someName(name)
            , someValue(value) {
        }

        std::string someName;
        float someValue;
    };
    typedef boost::shared_ptr<WindProperties> WindPropertiesPtr;

} // anonymous namespace


extern const UUID UUIDSemanticTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
extern const UUID UUIDSemanticTypeB = boost::uuids::string_generator()("{22222222-3333-4444-5555-666666666666}");

typedef support::SemanticTypeTrait<float, UUIDSemanticTypeA>             WindFloatTrait;
typedef support::BlackboardValue<WindFloatTrait::type>                   WindFloatValue;
typedef boost::intrusive_ptr<WindFloatValue>                             WindFloatValuePtr;

typedef support::SemanticTypeTrait<WindPropertiesPtr, UUIDSemanticTypeA> WindStructTrait;
typedef support::BlackboardValue<WindStructTrait::type>                  WindStructValue;
typedef boost::intrusive_ptr<WindStructValue>                             WindStructValuePtr;

typedef support::SemanticTypeTrait<int, UUIDSemanticTypeB>               IntTrait;
typedef support::BlackboardValue<IntTrait::type>                         IntValue;
typedef boost::intrusive_ptr<IntValue>                                   IntValuePtr;


struct AIBlackboardFixture {

    AIBlackboardFixture()
    {
        blackboard = aiFactory.createBlackboard();
    }

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};


BOOST_FIXTURE_TEST_CASE(TestDifferentValueTypesSameSemanticUUID, AIBlackboardFixture)
{
    {
        // Add a single value.
        IntValuePtr intValue = support::createBlackBoardValue<IntTrait>(15);
        BOOST_REQUIRE_NO_THROW(blackboard->AddValue(intValue));

        // Add two values sharing the same semantic ID.
        WindFloatValuePtr windFloatValue = support::createBlackBoardValue<WindFloatTrait>(10.0f);
        BOOST_REQUIRE_NO_THROW(blackboard->AddValue(windFloatValue));

        WindStructValuePtr windObjectValue = support::createBlackBoardValue<WindStructTrait>(WindPropertiesPtr(new WindProperties("ABC", 42.0f)));
        BOOST_REQUIRE_NO_THROW(blackboard->AddValue(windObjectValue));
    }


    BlackboardValueList values = blackboard->GetValuesByType(UUIDSemanticTypeA);
    BOOST_REQUIRE_EQUAL(values.size(), 2);

    WindFloatValuePtr windFloatValueEntry = boost::dynamic_pointer_cast<WindFloatValue>(values.front());
    BOOST_CHECK(windFloatValueEntry);
    BOOST_CHECK_EQUAL(windFloatValueEntry->GetValue(), 10.0f);

    WindStructValuePtr windStructValueEntry = boost::dynamic_pointer_cast<WindStructValue>(values.back());
    BOOST_CHECK(windStructValueEntry);
    WindPropertiesPtr windProperties = boost::dynamic_pointer_cast<WindProperties>(windStructValueEntry->GetValue());
    BOOST_REQUIRE(windProperties);
    BOOST_CHECK_EQUAL(windProperties->someName, "ABC");
    BOOST_CHECK_EQUAL(windProperties->someValue, 42.0f);


    values = blackboard->GetValuesByType(UUIDSemanticTypeB);
    BOOST_REQUIRE_EQUAL(values.size(), 1);
    IntValuePtr intValueEntry = boost::dynamic_pointer_cast<IntValue>(values.front());
    BOOST_CHECK_EQUAL(intValueEntry->GetValue(), 15);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw