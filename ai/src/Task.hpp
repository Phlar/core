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

        UUID GetID() const override;

        TaskResult Evaluate() final;

    protected:

        // A task itself cannot be constructed as such
        Task();

        //! \brief Functions derived implementations can implement.
        //! They will be invoked before and after the actual Evaluate() call.
        virtual void preEvaluate();
        virtual void postEvaluate();

        virtual TaskResult evaluate() = 0;

    private:

        UUID m_id;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw