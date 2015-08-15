#pragma once

#include "AIFwdDeclarations.hpp"
#include "IBehaviourTree.hpp"

#include "Task.hpp"

#include "InterfaceImpl.hpp"

#include <boost/coroutine/asymmetric_coroutine.hpp>

namespace aw {
namespace core {
namespace ai {
namespace impl {


//! \brief Implements "step-wise" execution of the behaviour tree.
//! Similar to generators / coroutines in other languages execution
//! of an Task::evaluate() function will be suspended at user defined
//! places inside the function thus returning the execution context
//! back to the caller. On the next call of "BahviourTree::EvaluateOneTick()"
//! execution will resume at the previously suspended location.
//! \note Execution may not be triggered from threads different from
//! the thread the tree was constructed at!
class BehaviourTree : public base::InterfaceImpl<IBehaviourTree> {

    public:

        BehaviourTree();
        virtual ~BehaviourTree();

        //@{
        //! Implementations of IBehviourTree
        void SetRoot(ITaskPtr rootTask);
        BehaviourTreeState EvaluateOneTick();
        //@}

    protected:

        TaskPtr m_root; // Root task to start execution from.
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw