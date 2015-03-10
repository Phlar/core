#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {


enum class TaskResult {

    TASK_RESULT_FAILED = 0,
    TASK_RESULT_PASSED
};

class ITask : public virtual base::IReferenceCounted {

    public:

        //! \brief Returns the task's unique ID
        virtual UUID GetID() const;
        
        //! \brief Calling this invokes the subtree underneath to
        //! get processed.
        virtual TaskResult evaluate() = 0;
};
typedef boost::intrusive_ptr<ITask> ITaskPtr;

} // namespace ai
} // namespace core
} // namespace aw