#pragma once

#include "AIFwdDeclarations.hpp"
#include "IBehaviorTree.hpp"

#include "InterfaceImpl.hpp"

#pragma warning (disable: 4251)
#include <boost/coroutine/asymmetric_coroutine.hpp>
#pragma warning (default: 4251)

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

        //@{
        //! Implementations of IBehviourTree
        void SetRoot(ITaskPtr rootTask);
        BehaviorTreeState ExecuteAsync();
        void ResetAsyncExecution();
        BehaviorTreeState ExecuteSync();
        BehaviorTreeState State() const;
        //@}

    protected:

        typedef boost::coroutines::coroutine<void>::pull_type   CoroutinePullType;
        typedef boost::coroutines::coroutine<void>::push_type   CoroutinePushType;
        typedef std::unique_ptr<CoroutinePullType>              CoroutinePullTypePtr;
        typedef std::unique_ptr<CoroutinePushType>              CoroutinePushTypePtr;

        //! \brief Worker method to be used as a coroutine.
        void executeAsync(CoroutinePullType& yield);
        
        CoroutinePushTypePtr m_coroutineHandler; // Optional coroutine handler.

        ITaskPtr m_root;               // Root task to start execution from.
        IBlackboardPtr m_blackboard;   // Blackboard used throughout execution.
        BehaviorTreeState m_state;     // Execution state of the tree.
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw