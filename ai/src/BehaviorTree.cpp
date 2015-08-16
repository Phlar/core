#include "BehaviorTree.hpp"

#include "Blackboard.hpp"
#include "Task.hpp"

#include <boost/bind.hpp>


namespace aw {
namespace core {
namespace ai {
namespace impl {

namespace {

void validateRoot(ITaskPtr root) {

    if(!root) {

        throw std::invalid_argument("Invalid root to start behavior tree from.");
    }
}

} // anonymous namespace


BehaviorTree::BehaviorTree()
: m_coroutineHandler(nullptr)
, m_root(nullptr)
, m_blackboard(IBlackboardPtr(new Blackboard()))
, m_state(BehaviorTreeState::STATE_NOT_RUN) {
}

BehaviorTree::~BehaviorTree() {
}

void BehaviorTree::SetRoot(ITaskPtr rootTask) {

    m_root = rootTask;
}

BehaviorTreeState BehaviorTree::ExecuteAsync() {

    validateRoot(m_root);

    // Reset all if necessary.
    //! \todo: Check whether this should be more explicit - i.e.
    //! throwing in case of starting a finished coroutine, having
    //! the user reset the state manually.
    if(m_state == BehaviorTreeState::STATE_FINISHED ||
       m_state == BehaviorTreeState::STATE_FAILED || 
       !m_coroutineHandler) {
        ResetAsyncExecution();
    }

    assert(m_coroutineHandler);

    try {

        (*m_coroutineHandler)();
        m_state = (*m_coroutineHandler) ? BehaviorTreeState::STATE_RUNNING : BehaviorTreeState::STATE_FINISHED;

    } catch(const std::exception& /*e*/) {

        //! \todo Log me!
        m_state = BehaviorTreeState::STATE_FAILED;
    } catch(...) {

        //! \todo Same here!
        m_state = BehaviorTreeState::STATE_FAILED;
    }

    return m_state;
}

void BehaviorTree::ResetAsyncExecution() {

    //! \todo Check resetting all tasks' properties in the tree!
    m_coroutineHandler.reset(new CoroutinePushType(
        boost::bind(&BehaviorTree::executeAsync, this, _1)));
    m_state = BehaviorTreeState::STATE_NOT_RUN;
}

BehaviorTreeState BehaviorTree::ExecuteSync() {

    validateRoot(m_root);

    // No need to use coroutines in case of the execution
    // was not started yet asynchronously.
    if(!m_coroutineHandler) {

        try {

            const TaskResult result = m_root->Evaluate(m_blackboard, nullptr);

            switch(result) {

                case TaskResult::TASK_RESULT_PASSED: {
                   m_state = BehaviorTreeState::STATE_FINISHED;
                }
                case TaskResult::TASK_RESULT_FAILED: {
                    m_state = BehaviorTreeState::STATE_FAILED;
                    break;
                }
                default: {

                    // (result != TaskResult::TASK_RESULT_RUNNING) Must not happen during synchronous execution!
                    // todo: However check what happens in case of asynchronous actions / conditions.
                    assert(false);
                }
            }
        } catch(const std::exception& /*e*/) {

            //! \todo Log me...
            m_state = BehaviorTreeState::STATE_FAILED;
        } catch( ... ) {

            // ...and me as well!
            m_state = BehaviorTreeState::STATE_FAILED;
        }
    } else {

        while(m_state == BehaviorTreeState::STATE_NOT_RUN ||
              m_state == BehaviorTreeState::STATE_RUNNING) {
            ExecuteAsync();
        }
    }

    return m_state;
}

BehaviorTreeState BehaviorTree::State() const {

    return m_state;
}

void BehaviorTree::executeAsync(CoroutinePullType& yield) {
    
    m_root->Evaluate(m_blackboard, &yield);
}


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw