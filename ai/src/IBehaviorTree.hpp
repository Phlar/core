#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"
#include "ITask.hpp"

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


enum class BehaviorTreeState {

    STATE_NOT_RUN = 0,
    STATE_RUNNING,
    STATE_FINISHED,
    STATE_FAILED
};

class IBehaviorTree : public virtual base::IReferenceCounted {

    public:

        //! \brief Set the topmost root the tree is composed of.
        virtual void SetRoot(ITaskPtr rootTask) = 0;

        //! \brief Perform a stepwise execution of the tree (asynchronous).
        virtual BehaviorTreeState ExecuteAsync() = 0;

        //! \brief In case of an asynchronous execution, the execution will be canceled.
        virtual void ResetAsyncExecution() = 0;

        //! \brief Synchronous execution of the (sub)tree.
        //! In case of the execution was started asynchronously and is currently suspended,
        //! the entire left tree will be executed at once.
        virtual BehaviorTreeState ExecuteSync() = 0;

        //! \brief Returns current state.
        virtual BehaviorTreeState State() const = 0;
};

typedef boost::intrusive_ptr<IBehaviorTree> IBehaviorTreePtr;

} // namespace ai
} // namespace core
} // namespace aw