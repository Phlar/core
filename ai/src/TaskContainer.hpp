#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITaskContainer.hpp"
#include "Task.hpp"

#include "InterfaceImpl.hpp"

#include <list>

namespace aw {
namespace core {
namespace ai {
namespace impl {

class TaskContainer : public base::InterfaceImpl<ITaskContainer>, public impl::Task {

    public:
        virtual ~TaskContainer();

        size_t GetNumTasks() const;
        void AddTask(ITaskPtr task, bool append = true);
        void RemoveTasks(ITaskPtr task);

    protected:

        // A Task-container itself cannot be constructed as such
        TaskContainer();

        TaskList m_children;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw