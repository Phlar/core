#include "BehaviourTree.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


BehaviourTree::BehaviourTree() {
}

BehaviourTree::~BehaviourTree() {
}

void BehaviourTree::SetRoot(ITaskPtr /*rootTask*/) {
}

BehaviourTreeState BehaviourTree::EvaluateOneTick() {

    return BehaviourTreeState::STATE_RUNNING;
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw