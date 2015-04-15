#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "InterfaceImpl.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask> {

    public:
        virtual ~Task();

        //!@{
        //! Implmentations of ITask interface
        UUID GetID() const override;
        TaskResult Evaluate(IBlackboardPtr blackboard) const override; /*final*/
        ITaskParameterPtr CreateTaskParameter() const override;
        //!@}

    protected:

        // A task itself cannot be constructed as such
        Task();

        //! \brief Functions derived implementations can implement.
        //! They will be invoked before and after the actual Evaluate() call.
        virtual void preEvaluate(IBlackboardPtr blackboard) const;
        virtual void postEvaluate(IBlackboardPtr blackboard) const;

        virtual TaskResult evaluate(IBlackboardPtr blackboard) const = 0;

    private:

        UUID m_id;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw