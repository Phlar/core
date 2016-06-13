#pragma once

#include "IDecorator.hpp"
#include "IBlackboard.hpp"

#include "TaskContainer.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


class Decorator : public base::InterfaceImpl<IDecorator>, public impl::Task {

    public:

        virtual ~Decorator();

        //@{
        //! Implementation of IDecorator 
        void SetDecoratedTask(ITaskPtr task);
        ITaskPtr GetDecoratedTask() const;
        //@}

    protected:

        //! \brief Helper derived decorators can invoke to execute
        //! decorated tasks down the chain.
        ITask::TaskResult evaluateDecoratedTask(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const;

        //! Disable direct construction of a decorator itself.
        Decorator(ITaskPtr decoratedTask = ITaskPtr());

    private:

        ITaskPtr m_decoratedTask;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw