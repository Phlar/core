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

        //@{
        //! Implementations of ITaskContainer
        size_t GetNumTasks() const override;
        void AddTask(ITaskPtr task, bool append = true) override;
        void RemoveTasks(ITaskPtr task) override;
        //@}

    protected:

        // A Task-container itself cannot be constructed as such
        TaskContainer();

        std::list<ITaskPtr> m_children;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw