#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIBlackboard"

#include <boost/test/unit_test.hpp>

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

const UUID UUIDTypeA = boost::uuids::string_generator()("{40FBA946-4666-4686-936E-3D1BB09AFA8F}");
const UUID UUIDTypeB = boost::uuids::string_generator()("{6DEB7499-6546-4EC6-8381-B91F3EF857E9}");
const UUID UUIDTypeC = boost::uuids::string_generator()("{F5E4DF4C-B13B-4D03-885A-2A3FE5AA06E6}");

class BlackboardTestValue : public support::BlackboardValue<int> {
    
    public:

        BlackboardTestValue(int value, const UUID& typeID)
        : support::BlackboardValue<int>(value, typeID) {
        }

        ~BlackboardTestValue() {
        }

        void SetTypeID(const UUID& typeID) {

            m_typeID = typeID;
        }

        void SetID(const UUID& id) {

            m_id = id;
        }
};
typedef boost::intrusive_ptr<BlackboardTestValue> BlackboardTestValuePtr;

struct AIBlackboardFixture {

    AIBlackboardFixture()
    : valueA_1(new BlackboardTestValue(10, UUIDTypeA))
    , valueA_2(new BlackboardTestValue(11, UUIDTypeA))
    , valueB_1(new BlackboardTestValue(20, UUIDTypeB))
    , valueB_2(new BlackboardTestValue(21, UUIDTypeB))
    , valueC_1(new BlackboardTestValue(30, UUIDTypeC))
    {

        blackboard = aiFactory.createBlackboard();
        BOOST_REQUIRE(blackboard);
    }

    BlackboardTestValuePtr valueA_1;
    BlackboardTestValuePtr valueA_2;
    BlackboardTestValuePtr valueB_1;
    BlackboardTestValuePtr valueB_2;
    BlackboardTestValuePtr valueC_1;

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};


BOOST_FIXTURE_TEST_CASE(AddValuesToBlackboard, AIBlackboardFixture) {

    BOOST_REQUIRE_NO_THROW(blackboard->AddValue(valueA_1));
    BOOST_REQUIRE_NO_THROW(blackboard->AddValue(valueA_2));
    BOOST_REQUIRE_NO_THROW(blackboard->AddValue(valueB_1));
    BOOST_REQUIRE_NO_THROW(blackboard->AddValue(valueB_2));
    BOOST_REQUIRE_NO_THROW(blackboard->AddValue(valueC_1));

    // Retrieve all expected entries for certain types.
    {
        std::list<IBlackboardValuePtr> referenceEntries = boost::assign::list_of
            (valueA_1)(valueA_2);
        std::list<IBlackboardValuePtr> blackboardEntries = blackboard->GetValuesByType(UUIDTypeA);
        BOOST_CHECK_EQUAL_COLLECTIONS(blackboardEntries.begin(), blackboardEntries.end(),
            referenceEntries.begin(), referenceEntries.end());
    }
    {
        std::list<IBlackboardValuePtr> referenceEntries = boost::assign::list_of
            (valueB_1)(valueB_2);
        std::list<IBlackboardValuePtr> blackboardEntries = blackboard->GetValuesByType(UUIDTypeB);
        BOOST_CHECK_EQUAL_COLLECTIONS(blackboardEntries.begin(), blackboardEntries.end(),
            referenceEntries.begin(), referenceEntries.end());
    }
    {
        std::list<IBlackboardValuePtr> referenceEntries = boost::assign::list_of(valueC_1);
        std::list<IBlackboardValuePtr> blackboardEntries = blackboard->GetValuesByType(UUIDTypeC);
        BOOST_CHECK_EQUAL_COLLECTIONS(blackboardEntries.begin(), blackboardEntries.end(),
            referenceEntries.begin(), referenceEntries.end());
    }
}

BOOST_FIXTURE_TEST_CASE(AddingInvalidValueShouldThrow, AIBlackboardFixture) {

    BOOST_CHECK_THROW(blackboard->AddValue(IBlackboardValuePtr()), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithInvalidTypeIDSHouldThrow, AIBlackboardFixture) {

    // Modify a value from the fixture thus its type-ID becomes "empty"
    valueA_1->SetTypeID(boost::uuids::uuid());
    BOOST_CHECK_THROW(blackboard->AddValue(valueA_1), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithInvalidIDSHouldThrow, AIBlackboardFixture) {

    // Modify a value from the fixture thus its type-ID becomes "empty"
    valueA_1->SetID(boost::uuids::uuid());
    BOOST_CHECK_THROW(blackboard->AddValue(valueA_1), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithSameIDTwiceShouldThrow, AIBlackboardFixture) {

    blackboard->AddValue(valueA_1);
    valueA_2->SetID(valueA_1->GetID());
    BOOST_CHECK_THROW(blackboard->AddValue(valueA_2), std::logic_error);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw