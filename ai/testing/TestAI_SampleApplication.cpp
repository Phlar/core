#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AISampleApplication"

#include "AIFactory.hpp"
#include "ISequencer.hpp"
#include "IAction.hpp"
#include "ICondition.hpp"
#include "IBlackboardValue.hpp"
#include "BlackboardUtils.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/noncopyable.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/shared_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

// Sample test inspired by "The Settlers". A villager is supposed to
// chop (n) trees from the forest to finish his task. However not to
// entirely exploit nature at a certain limit (l) he decides to plant a
// new tree before continuing chopping another one.

namespace {

// Application specific data.

typedef std::pair<float, float> Position;

class Tree : boost::noncopyable
{
    public:
        
        static boost::shared_ptr<Tree> create(const Position& pos) {
            
            return boost::shared_ptr<Tree>(new Tree(pos));
        }

    const uint8_t id;
    const Position position;

    protected:

        static uint8_t nextID;

        Tree(const Position& pos)
        : id(nextID++)
        , position(pos)
        {}
};
uint8_t Tree::nextID = 1;

typedef boost::shared_ptr<Tree> TreePtr;
typedef std::vector<TreePtr> TreeVector;


// Define entities to be stored in the blackboard.
extern const UUID ForestID = boost::uuids::string_generator()("{EF80B0F3-8728-48B1-BCE3-3477EB8F28EB}");
typedef ai::support::SemanticTypeTrait<TreeVector, ForestID> ForestTrait;
typedef boost::intrusive_ptr<ForestTrait::type> BBForestEntry;

extern const UUID TreeStorageID = boost::uuids::string_generator()("{230D961F-642A-4D54-919E-84C0F653158C}");
typedef ai::support::SemanticTypeTrait<TreeVector, TreeStorageID> TreeStorageTrait;
typedef boost::intrusive_ptr<TreeStorageTrait::type> BBChoppedTrees;

extern const UUID PlantPositionID = boost::uuids::string_generator()("{FDA92EFC-67B9-4CB0-B225-BB31A0D7D1C5}");
typedef ai::support::SemanticTypeTrait<Position, PlantPositionID> PlantPositionTrait;
typedef boost::intrusive_ptr<PlantPositionTrait::type> BBCPlantPositionEntry;

extern const UUID TreeToChopID = boost::uuids::string_generator()("{29AC3857-F712-4F77-B364-74A8582578AB}");
typedef ai::support::SemanticTypeTrait<uint8_t, TreeToChopID> TreeToChopTrait;
typedef boost::intrusive_ptr<TreeToChopTrait::type> BBCTreeToChopEntry;

extern const UUID LowerPopulationLimitID = boost::uuids::string_generator()("{CD35478D-180D-4CED-AE2B-DBB022F140B7}");
typedef ai::support::SemanticTypeTrait<uint32_t, LowerPopulationLimitID> LowerPopulationLimitTrait;
typedef boost::intrusive_ptr<LowerPopulationLimitTrait::type> BBLowerPopulationLimitEntry;

// Condition function checking the lower population^.
TaskResult enoughTreesAvailable(IBlackboardPtr blackboard) {

    const uint32_t populationLimit = support::requireSingleBlackBoardValue<uint32_t>(blackboard, LowerPopulationLimitID);
    const uint32_t availableTrees = static_cast<uint32_t>(support::requireSingleBlackBoardValue<TreeVector>(blackboard, ForestID).size());
    
    return (availableTrees >= populationLimit) ? TaskResult::TASK_RESULT_PASSED : TaskResult::TASK_RESULT_FAILED;
}

// Action that selects a tree to chop.
TaskResult selectTreeToChop(IBlackboardPtr blackboard) {

    
}

// Action that chops a tree.
TaskResult chopTree(IBlackboardPtr blackboard) {



}

} // anonymous namespace

BOOST_AUTO_TEST_CASE(SampleApplication) {
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw