#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIBlackboard"

#include <boost/test/unit_test.hpp>

#include "UUID.hpp"
#include "InterfaceImpl.hpp"
#include "AIFactory.hpp"
#include "IBlackboard.hpp"

#include "BlackboardValue.hpp"

#include <boost/assign.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

const base::UUID UUIDTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
const base::UUID UUIDTypeB = boost::uuids::string_generator()("{22222222-3333-4444-5555-666666666666}");
const base::UUID UUIDTypeC = boost::uuids::string_generator()("{33333333-4444-5555-6666-777777777777}");

const base::UUID UUIDTaskA = boost::uuids::string_generator()("{AAAAAAAA-1111-2222-3333-444444444444}");
const base::UUID UUIDTaskB = boost::uuids::string_generator()("{BBBBBBBB-1111-2222-3333-444444444444}");
const base::UUID UUIDTaskC = boost::uuids::string_generator()("{CCCCCCCC-1111-2222-3333-444444444444}");



class BlackboardTestValue : public support::BlackboardValue<int> {
    
    public:

        BlackboardTestValue(int value, const base::UUID& typeID)
        : support::BlackboardValue<int>(value, typeID) {
        }

        ~BlackboardTestValue() {
        }

        void SetTypeID(const base::UUID& typeID) {

            m_typeID = typeID;
        }

        void SetID(const base::UUID& id) {

            m_id = id;
        }
};
typedef boost::intrusive_ptr<BlackboardTestValue> BlackboardTestValuePtr;


struct AIBlackboardFixture {

    AIBlackboardFixture()
    : valueA_1(new BlackboardTestValue(10, UUIDTypeA))
    , valueB_1(new BlackboardTestValue(20, UUIDTypeB))
    , valueC_1(new BlackboardTestValue(30, UUIDTypeC)) {

        blackboard = aiFactory.createBlackboard();
        BOOST_REQUIRE(blackboard);
    }

    BlackboardTestValuePtr valueA_1;
    BlackboardTestValuePtr valueB_1;
    BlackboardTestValuePtr valueC_1;

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};


BOOST_FIXTURE_TEST_CASE(AddValuesToBlackboard, AIBlackboardFixture) {

    BOOST_REQUIRE_NO_THROW(blackboard->SetValue(valueA_1));
    BOOST_REQUIRE_NO_THROW(blackboard->SetValue(valueB_1));

    // Retrieve all expected entries for certain types.
    {
        IBlackboardValuePtr blackboardEntry = blackboard->GetValue(UUIDTypeA);
        BOOST_CHECK_EQUAL(blackboardEntry, valueA_1);
    }
    {
        IBlackboardValuePtr blackboardEntry = blackboard->GetValue(UUIDTypeB);
        BOOST_CHECK_EQUAL(blackboardEntry, valueB_1);
    }
    {
        // There should not be a value of this ID.
        IBlackboardValuePtr blackboardEntry = blackboard->GetValue(UUIDTypeC);
        BOOST_CHECK_EQUAL(blackboardEntry, IBlackboardValuePtr());
    }
}

BOOST_FIXTURE_TEST_CASE(AddingInvalidValueShouldThrow, AIBlackboardFixture) {

    BOOST_CHECK_THROW(blackboard->SetValue(IBlackboardValuePtr()), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithInvalidTypeIDShouldThrow, AIBlackboardFixture) {

    // Modify a value from the fixture thus its type-ID becomes "empty"
    valueA_1->SetTypeID(base::UUID());
    BOOST_CHECK_THROW(blackboard->SetValue(valueA_1), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithInvalidIDSHouldThrow, AIBlackboardFixture) {

    // Modify a value from the fixture thus its type-ID becomes "empty"
    valueA_1->SetID(boost::uuids::uuid());
    BOOST_CHECK_THROW(blackboard->SetValue(valueA_1), std::invalid_argument);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw