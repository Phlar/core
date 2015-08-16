#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "InterfaceImpl.hpp"

#include <boost/coroutine/asymmetric_coroutine.hpp>

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask> {

    public:
        virtual ~Task();

        //!@{
        //! Implementations of ITask interface
        UUID GetID() const override;
        TaskResult Evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType* yield) const override;
        //!@}

    protected:

        // A task itself cannot be constructed as such
        Task();

        //! \brief Functions derived implementations can implement.
        //! They will be invoked before and after the actual Evaluate() call.
        virtual void preEvaluate(IBlackboardPtr blackboard) const;
        virtual void postEvaluate(IBlackboardPtr blackboard) const;

        virtual TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType* yield) const = 0;

    private:

        UUID m_id;
};
typedef boost::intrusive_ptr<Task> TaskPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw