#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

namespace aw {
namespace core {
namespace ai {

class IDecorator : public virtual ITask {

    public:

        //! \brief Sets / unsets the task to be decorated.
        virtual void SetDecoratedTask(ITaskPtr task) = 0;

        //! \brief Returns the task this decorator affects.
        virtual ITaskPtr GetDecoratedTask() const = 0;
};

} // namespace ai
} // namespace core
} // namespace ai