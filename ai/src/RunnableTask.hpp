#pragma once

#include "AIFwdDeclarations.hpp"

#include "Task.hpp"

#include <boost/function.hpp>


namespace aw {
namespace core {
namespace ai {
namespace impl {


//! \brief Since an Action and Condition share a lot of code this class acts as 
//! a common base class providing means to extend it with user code to be run.
class RunnableTask : public Task, public boost::noncopyable {

    public:

        enum class RunnableTaskType {

            RUNNABLE_TASK_ACTION = 0,
            RUNNABLE_TASK_CONDITION
        };

        typedef boost::function<TaskResult(IBlackboardPtr)> RunnableTaskFnc;

        virtual ~RunnableTask();

    protected:

        // Enable construction only from classes down the inheritance chain.
        RunnableTask(const RunnableTaskFnc& runnableFunction, RunnableTaskType runnableTaskType);

        //! \brief Returns result of the provided function to be run.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //!  function provided.
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;

        //! \brief Define the function to executte.
        void setRunnableTaskFunction(const RunnableTaskFnc& runnableFunction);

        RunnableTaskFnc m_runnableFunction;
        const RunnableTaskType m_runnableTaskType;

    private:

        // Disable default construction.
        RunnableTask();
};

typedef boost::intrusive_ptr<RunnableTask> RunnableTaskPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw