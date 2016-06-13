#pragma once

#include "AIFwdDeclarations.hpp"

#include "UUID.hpp"
#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

#pragma warning (disable: 4251)
#include <boost/coroutine/asymmetric_coroutine.hpp>
#pragma warning (default: 4251)

namespace aw {
namespace core {
namespace ai {



class ITask : public virtual base::IReferenceCounted {

    public:

        enum class TaskResult {

            TASK_RESULT_FAILED = 0,
            TASK_RESULT_PASSED,
            TASK_RESULT_RUNNING
        };

        typedef boost::coroutines::coroutine<void>::pull_type TaskCoroutinePullType;

        //! \brief Returns the task's unique ID.
        virtual base::UUID GetID() const = 0;

        //! \brief Triggers the next execution step.
        //! \param blackboard   The tree's blackboard to read / write data.
        //! \param yield        Coroutine handler - in case of nullptr the entire
        //!                     subtree will be executed at once.
        //! \todo Coroutine handler rather is an implementation detail and should
        //!       not be exposed through the API.
        virtual ITask::TaskResult Evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield = nullptr) const = 0;
};
typedef boost::intrusive_ptr<ITask> ITaskPtr;

} // namespace ai
} // namespace core
} // namespace aw
