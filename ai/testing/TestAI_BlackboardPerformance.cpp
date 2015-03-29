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

    WindProperties(const std::string& name, float value)
        : someName(name)
        , someValue(value) {
    }

    std::string someName;
    float someValue;
};
typedef boost::shared_ptr<WindProperties> WindPropertiesPtr;

struct CarProperties {

    CarProperties(const std::string& _manufacturer, long _price, float _age) 
    : manufacturer(_manufacturer)
    , price(_price)
    , age(_age) {}

    std::string manufacturer;
    long price;
    float age;
};
typedef boost::shared_ptr<CarProperties> CarPropertiesPtr;

} // anonymous namespace


extern const UUID UUIDSemanticTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
extern const UUID UUIDSemanticTypeB = boost::uuids::string_generator()("{22222222-3333-4444-5555-666666666666}");
extern const UUID UUIDSemanticTypeC = boost::uuids::string_generator()("{33333333-4444-5555-6666-777777777777}");

typedef support::SemanticTypeTrait<float, UUIDSemanticTypeA>             WindFloatTrait;
typedef support::BlackboardValue<WindFloatTrait::type>                   WindFloatValue;
typedef boost::intrusive_ptr<WindFloatValue>                             WindFloatValuePtr;

typedef support::SemanticTypeTrait<WindPropertiesPtr, UUIDSemanticTypeA> WindStructTrait;
typedef support::BlackboardValue<WindStructTrait::type>                  WindStructValue;
typedef boost::intrusive_ptr<WindStructValue>                            WindStructValuePtr;

typedef support::SemanticTypeTrait<int, UUIDSemanticTypeB>               IntTrait;
typedef support::BlackboardValue<IntTrait::type>                         IntValue;
typedef boost::intrusive_ptr<IntValue>                                   IntValuePtr;

typedef support::SemanticTypeTrait<CarPropertiesPtr, UUIDSemanticTypeC>  CarPropertiesTrait;
typedef support::BlackboardValue<CarPropertiesTrait::type>               CarPropertiesValue;
typedef boost::intrusive_ptr<CarPropertiesValue>                         CarPropertiesValuePtr;

struct AIBlackboardFixture {

    AIBlackboardFixture()
    {
        blackboard = aiFactory.createBlackboard();
    }

    void addValues(int times) {

        for(int index = 0; index < times; ++index) {

            WindFloatValuePtr windFloatValue = support::createBlackBoardValue<WindFloatTrait>(123.0f * index);
            BOOST_REQUIRE_NO_THROW(blackboard->AddValue(windFloatValue));

            WindStructValuePtr windStructValue = support::createBlackBoardValue<WindStructTrait>(
                WindPropertiesPtr(new WindProperties("ABC", 234.0f * index)));
            BOOST_REQUIRE_NO_THROW(blackboard->AddValue(windStructValue));

            IntValuePtr intValue = support::createBlackBoardValue<IntTrait>(index);
            BOOST_REQUIRE_NO_THROW(blackboard->AddValue(intValue));

            CarPropertiesValuePtr carValue = support::createBlackBoardValue<CarPropertiesTrait>(
                CarPropertiesPtr(new CarProperties("VW", 100 * index, index * 0.5f)));
            BOOST_REQUIRE_NO_THROW(blackboard->AddValue(carValue));
        }
    }

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};


BOOST_FIXTURE_TEST_CASE(AddMultipleValues, AIBlackboardFixture)
{
    const int timesAdded = 10;
    addValues(timesAdded);

    BOOST_CHECK_EQUAL(blackboard->GetValuesByType(UUIDSemanticTypeA).size(), timesAdded * 2);
    BOOST_CHECK_EQUAL(blackboard->GetValuesByType(UUIDSemanticTypeB).size(), timesAdded);
    BOOST_CHECK_EQUAL(blackboard->GetValuesByType(UUIDSemanticTypeC).size(), timesAdded);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw