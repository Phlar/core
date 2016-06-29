#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AISampleApplication"

#include "Utils.hpp"

#include "ISequencer.hpp"
#include "ISelector.hpp"
#include "IAction.hpp"
#include "ICondition.hpp"
#include "IBlackboardValue.hpp"
#include "BlackboardUtils.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <random>
#include <iostream>


#include "AIServiceFixture.hpp"


struct ScopedLog
{
    ScopedLog(const char* name) {
        std::cout << name << std::endl;
    }

    ~ScopedLog() {
        std::cout << "--------------------------------------------------" << std::endl;
    }
};

#define LOG_METHOD ScopedLog slog(__FUNCTION__);


namespace aw {
namespace core {
namespace ai {
namespace testing {

// Sample test inspired by "The Settlers". A villager is supposed to
// chop (n) trees from the forest to finish his task. However not to
// entirely exploit nature at a certain limit (l) he decides to plant a
// new tree before continuing chopping another one.

namespace {

// Use a global random generator.
std::mt19937 randomGenerator;

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
extern const base::UUID BBForestUID =             base::utils::CreateUUIDFromString("{EF80B0F3-8728-48B1-BCE3-3477EB8F28EB}");
extern const base::UUID BBTreesInWarehouseUID =   base::utils::CreateUUIDFromString("{230D961F-642A-4D54-919E-84C0F653158C}");
extern const base::UUID BBTreeToPlantPositionID = base::utils::CreateUUIDFromString("{FDA92EFC-67B9-4CB0-B225-BB31A0D7D1C5}");
extern const base::UUID BBTreeToChopUID =         base::utils::CreateUUIDFromString("{29AC3857-F712-4F77-B364-74A8582578AB}");
extern const base::UUID BBTreePopulationLimitID = base::utils::CreateUUIDFromString("{CD35478D-180D-4CED-AE2B-DBB022F140B7}");
extern const base::UUID BBChoppedTreesLimitID =   base::utils::CreateUUIDFromString("{F295989C-9CBC-4B46-9CE5-EBA1F8892DAF}");

BLACKBOARD_TYPE(Forest,              TreeVector, BBForestUID);
BLACKBOARD_TYPE(TreesInWarehouse,    TreeVector, BBTreesInWarehouseUID);
BLACKBOARD_TYPE(PositionToPlant,     Position,   BBTreeToPlantPositionID);
BLACKBOARD_TYPE(TreeToChop,          uint8_t,    BBTreeToChopUID);
BLACKBOARD_TYPE(TreePopulationLimit, uint32_t,   BBTreePopulationLimitID);
BLACKBOARD_TYPE(ChoppedTreesLimit,   uint32_t,   BBChoppedTreesLimitID);


// Condition function checking the lower population.
ITask::TaskResult enoughTreesAvailable(IBlackboardPtr blackboard) {

    LOG_METHOD

    const uint32_t populationLimit = support::getRawValueFromBlackboard<TreePopulationLimit::type>(blackboard, BBTreePopulationLimitID);
    const uint32_t availableTrees = static_cast<uint32_t>(support::getRawValueFromBlackboard<Forest::type>(blackboard, BBForestUID).size());
    
    return (availableTrees < populationLimit) ? ITask::TaskResult::TASK_RESULT_FAILED: ITask::TaskResult::TASK_RESULT_PASSED;
}

ITask::TaskResult enoughTreesCollected(IBlackboardPtr blackboard) {

    LOG_METHOD

    const uint32_t choppedTreesLimit = support::getRawValueFromBlackboard<ChoppedTreesLimit::type>(blackboard, BBChoppedTreesLimitID);
    const uint32_t choppedTrees = static_cast<uint32_t>(support::getRawValueFromBlackboard<TreesInWarehouse::type>(blackboard, BBTreesInWarehouseUID).size());

    return (choppedTrees <= choppedTreesLimit) ? ITask::TaskResult::TASK_RESULT_FAILED: ITask::TaskResult::TASK_RESULT_PASSED;
}

// Action that selects a tree to chop.
ITask::TaskResult selectTreeToChop(IBlackboardPtr blackboard) {

    LOG_METHOD

    const TreeVector& forest = support::getRawValueFromBlackboard<Forest::type>(blackboard, BBForestUID);
    if(forest.empty()) {
        return ITask::TaskResult::TASK_RESULT_FAILED;
    }

    std::uniform_int_distribution<> distribution(0, static_cast<int>(forest.size()) - 1);
    const size_t treeIndex = distribution(randomGenerator);

    blackboard->SetValue(support::createBlackboardValue<TreeToChop>(static_cast<uint8_t>(treeIndex)));
    return ITask::TaskResult::TASK_RESULT_PASSED;
}

// Action that chops a tree.
ITask::TaskResult chopTree(IBlackboardPtr blackboard) {

    LOG_METHOD

    IBlackboardValuePtr bbForest = support::getValueFromBlackboard(blackboard, BBForestUID);
    TreeVector forest = support::getRawValue<Forest::type>(bbForest);
    if(forest.empty()) {
        return ITask::TaskResult::TASK_RESULT_FAILED;
    }

    const uint8_t treeIndex = support::getRawValueFromBlackboard<TreeToChop::type>(blackboard, BBTreeToChopUID);
    TreePtr treeToChop = forest.at(treeIndex);
    forest.erase(forest.begin() + treeIndex);
    bbForest->AssignValue(forest);


    IBlackboardValuePtr bbWarehouse = support::getValueFromBlackboard(blackboard, BBTreesInWarehouseUID);
    TreeVector warehouseTrees = support::getRawValue<TreesInWarehouse::type>(bbWarehouse);
    warehouseTrees.push_back(treeToChop);
    bbWarehouse->AssignValue(warehouseTrees);

    return ITask::TaskResult::TASK_RESULT_PASSED;
}

// Action that plants a new tree.
ITask::TaskResult plantTree(IBlackboardPtr blackboard) {

    LOG_METHOD

    IBlackboardValuePtr bbForest = support::getValueFromBlackboard(blackboard, BBForestUID);
    TreeVector forest = support::getRawValue<Forest::type>(bbForest);

    std::uniform_real_distribution<> distribution(-10.0, 10.0);
    TreePtr newTree = Tree::create(Position(static_cast<float>(distribution(randomGenerator)),
                                            static_cast<float>(distribution(randomGenerator))));

    forest.push_back(newTree);
    bbForest->AssignValue(forest);

    return ITask::TaskResult::TASK_RESULT_PASSED;
}

} // namespace anonymous

BOOST_FIXTURE_TEST_CASE(SampleApplication, AIServiceFixture) {


    IConditionPtr enoughTreesInForestCondition  = aiService->createCondition(enoughTreesAvailable);
    IConditionPtr enoughTreesChoppedCondition   = aiService->createCondition(enoughTreesCollected);
    IActionPtr    selectTreeToChopAction        = aiService->createAction(selectTreeToChop);
    IActionPtr    chopTreeAction                = aiService->createAction(chopTree);
    IActionPtr    plantTreeAction               = aiService->createAction(plantTree);

    ISequencerPtr sequencer1 = aiService->createSequencer();
    ISequencerPtr sequencer2 = aiService->createSequencer();

    IBlackboardPtr blackboard = aiService->createBlackboard();

    sequencer1->AddTask(plantTreeAction);
    sequencer1->AddTask(sequencer2);
    sequencer2->AddTask(enoughTreesInForestCondition);
    sequencer2->AddTask(selectTreeToChopAction);
    sequencer2->AddTask(chopTreeAction);

    
    blackboard->SetValue(support::createBlackboardValue<TreePopulationLimit>(5));
    blackboard->SetValue(support::createBlackboardValue<ChoppedTreesLimit>(5));
    blackboard->SetValue(support::createBlackboardValue<Forest>());
    blackboard->SetValue(support::createBlackboardValue<TreesInWarehouse>());

    size_t collectedTrees = support::getRawValueFromBlackboard<TreesInWarehouse::type>(blackboard, BBTreesInWarehouseUID).size();
    const uint32_t choppedTreesLimit = support::getRawValueFromBlackboard<ChoppedTreesLimit::type>(blackboard, BBChoppedTreesLimitID);

    // Todo: Refactor out into decorators.
    do {
        sequencer1->Evaluate(blackboard);
        collectedTrees = support::getRawValueFromBlackboard<TreesInWarehouse::type>(blackboard, BBTreesInWarehouseUID).size();

        std::cout << "Forest currently has " << support::getRawValueFromBlackboard<Forest::type>(blackboard, BBForestUID).size() << " trees." << std::endl;
        std::cout << "Currently chopped trees " << collectedTrees << "/" << choppedTreesLimit << std::endl;
    } while(collectedTrees < choppedTreesLimit);
}

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw