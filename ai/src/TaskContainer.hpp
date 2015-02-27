#pragma once

#include "ITaskContainer.hpp"

#include "InterfaceImpl.hpp"

#include <list>

namespace aw {
namespace core {
namespace ai {

class TaskContainer : public base::InterfaceImpl<ITaskContainer, base::ReferenceCounted> {

    public:
        virtual ~TaskContainer();

        size_t GetNumTasks() const;
        void AddTask(ITaskPtr task, bool append = true);
        void RemoveTasks(ITaskPtr task);

    protected:

        // A Task-container itself cannot be constructed as such
        TaskContainer();

        typedef std::list<ITaskPtr> TaskList;
        TaskList m_children;
};

} // namespace ai
} // namespace core
} // namespace aw