#pragma once

#include "ITask.hpp"

namespace aw {
namespace core {
namespace ai {

class IDecorator : public virtual ITask {

    public:

        //! \brief Sets the (one and only) sub-task to execute.
        virtual void SetTask(ITaskPtr task) = 0;
};

} // namespace ai
} // namespace core
} // namespace ai