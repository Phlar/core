#pragma once

#include "ITask.hpp"

namespace aw {
namespace core {
namespace ai {
    
//! \brief This container as such itself implements a task - however 
//! also serves the purpose of adding / removing and ordering its
//! child tasks.
class ITaskContainer : public virtual ITask {

    public:

        //! \brief Returns the number of child tasks.
        virtual size_t GetNumTasks() const = 0;

        //! \brief Adds a task as a new child entry.
        //! Todo: Check how to react on creating possible cycles.
        virtual void AddTask(ITaskPtr task, bool append = true) = 0;

        //! \brief Removes one (or more) tasks.
        virtual void RemoveTasks(ITaskPtr task) = 0;
};

} // namespace ai
} // namespace ai     
} // namespace aw