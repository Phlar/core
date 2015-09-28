#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask> {

    public:
        virtual ~Task();

        //!@{
        //! Implementations of ITask interface
        base::UUID GetID() const override;
        TaskResult Evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
        //!@}

    protected:

        // A task itself cannot be constructed as such
        Task();

        //! \brief Functions derived implementations can implement.
        //! They will be invoked before and after the actual Evaluate() call.
        virtual void preEvaluate(IBlackboardPtr blackboard) const;
        virtual void postEvaluate(IBlackboardPtr blackboard) const;

        virtual TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const = 0;

    private:

        base::UUID m_id;
};
typedef boost::intrusive_ptr<Task> TaskPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw