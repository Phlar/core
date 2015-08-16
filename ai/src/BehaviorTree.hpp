#pragma once

#include "AIFwdDeclarations.hpp"
#include "IBehaviorTree.hpp"

#include "Task.hpp"

#include "InterfaceImpl.hpp"

#include <boost/coroutine/asymmetric_coroutine.hpp>

#include <memory>


namespace aw {
namespace core {
namespace ai {
namespace impl {


//! \brief Implements "step-wise" execution of the behavior tree.
//! Similar to generators / coroutines in other languages execution
//! of an Task::evaluate() function will be suspended at user defined
//! places inside the function thus returning the execution context
//! back to the caller. On the next call of "BahviourTree::EvaluateOneTick()"
//! execution will resume at the previously suspended location.
//! \note Execution may not be triggered from threads different from
//! the thread the tree was constructed at!
class BehaviorTree : public base::InterfaceImpl<IBehaviorTree> {

    public:

        BehaviorTree();
        virtual ~BehaviorTree();

        //! \todo Test exception thrown during synchronous / async execution inside of condition / action.

        //@{
        //! Implementations of IBehviourTree
        //! \todo Test exception handling in case of no root set.
        void SetRoot(ITaskPtr rootTask);

        //! \todo  - Test stepwise execution.
        //!        - Test resetting execution.
        BehaviorTreeState ExecuteAsync();
        void ResetAsyncExecution();

        //! \todo Test synchronous execution.
        BehaviorTreeState ExecuteSync();

        //! \todo Test various state checks.
        BehaviorTreeState State() const;
        //@}

    protected:

        typedef std::unique_ptr<ITask::TaskCoroutinePushType> TaskCoroutinePushTypePtr;

        //! \brief Worker method to be used as a coroutine.
        void executeAsync(ITask::TaskCoroutinePushType*);
        
        TaskCoroutinePushTypePtr m_coroutineHandler; // Optional coroutine handler.

        TaskPtr m_root;                 // Root task to start execution from.
        IBlackboardPtr m_blackboard;    // Blackboard used throughout execution.
        BehaviorTreeState m_state;     // Execution state of the tree.
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw