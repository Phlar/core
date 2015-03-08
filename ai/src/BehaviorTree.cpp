#include "BehaviorTree.hpp"

#include "ITask.hpp"


namespace aw {
namespace core {
namespace ai {

BehaviorTree::BehaviorTree(ITaskPtr task)
: m_rootTask(task)
, m_processingType(TreeProcessingType::PROCESS_ONE_CYCLE) {
}

BehaviorTree::~BehaviorTree() {
}

void BehaviorTree::SetRoot(ITaskPtr task) {

    // In case of setting a new root this means we cannot be sure about 
    // the sub-tree anymore, so everything will be reseted.
    // Todo: Think of dealing with the blackboard.
    m_rootTask = task;
}

void BehaviorTree::SetProcessingType(TreeProcessingType processingType) {

    // Todo: Think of moving this property to the blackboard.
    m_processingType = processingType;
}

void BehaviorTree::Process() {

        
}


} // namespace ai
} // namespace core
} // namespace aw