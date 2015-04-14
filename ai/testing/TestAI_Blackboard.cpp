#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIBlackboard"

#include <boost/test/unit_test.hpp>

#include "InterfaceImpl.hpp"
#include "AIFactory.hpp"
#include "IBlackboard.hpp"
#include "ITaskParameter.hpp"

#include "BlackboardValue.hpp"

#include <boost/assign.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

const UUID UUIDTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
const UUID UUIDTypeB = boost::uuids::string_generator()("{22222222-3333-4444-5555-666666666666}");
const UUID UUIDTypeC = boost::uuids::string_generator()("{33333333-4444-5555-6666-777777777777}");

const UUID UUIDTaskA = boost::uuids::string_generator()("{AAAAAAAA-1111-2222-3333-444444444444}");
const UUID UUIDTaskB = boost::uuids::string_generator()("{BBBBBBBB-1111-2222-3333-444444444444}");
const UUID UUIDTaskC = boost::uuids::string_generator()("{CCCCCCCC-1111-2222-3333-444444444444}");



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


class BlackboardTestTaskParameter : public base::InterfaceImpl<ITaskParameter> {

    public:

        BlackboardTestTaskParameter(const UUID& taskID)
        : m_taskID(taskID) {
        }

        UUID GetAssociatedTaskID() const {
            return m_taskID;
        }

    protected:
        UUID m_taskID;
};
typedef boost::intrusive_ptr<BlackboardTestTaskParameter> BlackboardTestTaskParameterPtr;

struct AIBlackboardFixture {

    AIBlackboardFixture()
    : valueA_1(new BlackboardTestValue(10, UUIDTypeA))
    , valueB_1(new BlackboardTestValue(20, UUIDTypeB))
    , valueC_1(new BlackboardTestValue(30, UUIDTypeC))
    , taskParameter_1(new BlackboardTestTaskParameter(UUIDTaskA))
    , taskParameter_2(new BlackboardTestTaskParameter(UUIDTaskB))
    , taskParameter_3(new BlackboardTestTaskParameter(UUIDTaskC)) {

        blackboard = aiFactory.createBlackboard();
        BOOST_REQUIRE(blackboard);
    }

    BlackboardTestValuePtr valueA_1;
    BlackboardTestValuePtr valueB_1;
    BlackboardTestValuePtr valueC_1;

    BlackboardTestTaskParameterPtr taskParameter_1;
    BlackboardTestTaskParameterPtr taskParameter_2;
    BlackboardTestTaskParameterPtr taskParameter_3;

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
    valueA_1->SetTypeID(UUID());
    BOOST_CHECK_THROW(blackboard->SetValue(valueA_1), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddingValueWithInvalidIDSHouldThrow, AIBlackboardFixture) {

    // Modify a value from the fixture thus its type-ID becomes "empty"
    valueA_1->SetID(boost::uuids::uuid());
    BOOST_CHECK_THROW(blackboard->SetValue(valueA_1), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddEmptyTaskParameterShouldThrow, AIBlackboardFixture) {

    BOOST_CHECK_THROW(blackboard->StoreTaskParameter(BlackboardTestTaskParameterPtr()),
        std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddTaskParameterWithInvalidTaskIDSholdThrow, AIBlackboardFixture) {

    BlackboardTestTaskParameterPtr invalidTaskParameter = BlackboardTestTaskParameterPtr(
        new BlackboardTestTaskParameter(UUID()));

    BOOST_CHECK_THROW(blackboard->StoreTaskParameter(invalidTaskParameter), 
        std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddTaskParametersToBlackboard, AIBlackboardFixture) {

    BOOST_CHECK_NO_THROW(blackboard->StoreTaskParameter(taskParameter_1));
    BOOST_CHECK_NO_THROW(blackboard->StoreTaskParameter(taskParameter_2));
    BOOST_CHECK_NO_THROW(blackboard->StoreTaskParameter(taskParameter_3));

    ITaskParameterPtr blackBoardTaskParameter_1 = blackboard->GetTaskParameter(
        taskParameter_1->GetAssociatedTaskID());
    BOOST_REQUIRE(blackBoardTaskParameter_1);
    BOOST_CHECK_EQUAL(taskParameter_1, blackBoardTaskParameter_1);

    ITaskParameterPtr blackBoardTaskParameter_2 = blackboard->GetTaskParameter(
        taskParameter_2->GetAssociatedTaskID());
    BOOST_REQUIRE(blackBoardTaskParameter_2);
    BOOST_CHECK_EQUAL(taskParameter_2, blackBoardTaskParameter_2);

    ITaskParameterPtr blackBoardTaskParameter_3 = blackboard->GetTaskParameter(
        taskParameter_3->GetAssociatedTaskID());
    BOOST_REQUIRE(blackBoardTaskParameter_3);
    BOOST_CHECK_EQUAL(taskParameter_3, blackBoardTaskParameter_3);
}

BOOST_FIXTURE_TEST_CASE(RemoveTaskParameters, AIBlackboardFixture) {

    BOOST_CHECK_NO_THROW(blackboard->StoreTaskParameter(taskParameter_1));
    BOOST_CHECK_NO_THROW(blackboard->StoreTaskParameter(taskParameter_2));

    // Removing the listed ones...
    BOOST_CHECK_EQUAL(blackboard->RemoveTaskParameter(taskParameter_1->GetAssociatedTaskID()), true);
    BOOST_CHECK_EQUAL(blackboard->RemoveTaskParameter(taskParameter_2->GetAssociatedTaskID()), true);

    // ...the should not be there any more
    BOOST_CHECK_EQUAL(blackboard->GetTaskParameter(taskParameter_1->GetAssociatedTaskID()), 
        ITaskParameterPtr());
    BOOST_CHECK_EQUAL(blackboard->GetTaskParameter(taskParameter_2->GetAssociatedTaskID()), 
        ITaskParameterPtr());


    // Removing a not-listed parameter should not remove anything...
    BOOST_CHECK_EQUAL(blackboard->RemoveTaskParameter(taskParameter_3->GetAssociatedTaskID()), false);

    // ... as well as the previously removed ones should not be there as well.
    BOOST_CHECK_EQUAL(blackboard->RemoveTaskParameter(taskParameter_1->GetAssociatedTaskID()), false);
    BOOST_CHECK_EQUAL(blackboard->RemoveTaskParameter(taskParameter_2->GetAssociatedTaskID()), false);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw