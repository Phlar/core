#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/coroutine/asymmetric_coroutine.hpp>

namespace aw {
namespace core {
namespace ai {


enum class TaskResult {

    TASK_RESULT_FAILED = 0,
    TASK_RESULT_PASSED,
    TASK_RESULT_RUNNING
};

class ITask : public virtual base::IReferenceCounted {

    public:

        typedef boost::coroutines::coroutine<void>::push_type& TaskCoroutinePushType;

        //! \brief Returns the task's unique ID.
        virtual UUID GetID() const = 0;

        //! \brief Triggers the next execution step.
        virtual TaskResult Evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType& yield) const = 0;
};
typedef boost::intrusive_ptr<ITask> ITaskPtr;

} // namespace ai
} // namespace core

} // namespace aw