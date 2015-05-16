#include "BehaviourTree.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


BehaviourTree::BehaviourTree() {
}

BehaviourTree::~BehaviourTree() {
}

void BehaviourTree::SetTreeRoot(ITaskPtr /*rootTask*/) {
}

TaskResult BehaviourTree::EvaluateOnce() {

    return TaskResult::TASK_RESULT_FAILED;
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw