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


extern const UUID UUIDSemanticTypeA = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");
BLACKBOARD_TYPE(IntVectorValue, std::vector<int>, UUIDSemanticTypeA);


struct AIBlackboardFixture {

    AIBlackboardFixture()
    {
        blackboard = aiFactory.createBlackboard();
    }

    AIFactory aiFactory;
    IBlackboardPtr blackboard;
};


BOOST_FIXTURE_TEST_CASE(TestUpdatingValue, AIBlackboardFixture)
{
    std::vector<int> initialValues;
    initialValues.push_back(10);
    initialValues.push_back(20);

    IntVectorValuePtr values = support::createBlackBoardValue<IntVectorValueTrait>(initialValues);
    BOOST_CHECK_EQUAL(values->GetValue().size(), 2);

    // Ad it to the blackboard.
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