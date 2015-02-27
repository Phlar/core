#include "TaskContainer.hpp"

namespace aw {
namespace core {
namespace ai {

TaskContainer::TaskContainer() 
: m_children() {
}

TaskContainer::~TaskContainer() {
}

size_t TaskContainer::GetNumTasks() const {

    return m_children.size();
}

void TaskContainer::AddTask(ITaskPtr task, bool append) {

    if(!task) {
        throw std::invalid_argument("Invalid task to add");
    }

    if(append) {
        m_children.push_back(task);
    } else {
        m_children.push_front(task);
    }
}

void TaskContainer::RemoveTasks(ITaskPtr task) {

    if(!task) {
        return; // Todo: Rather throw? 
    }

    m_children.remove(task);
}


} // namespace ai
} // namespace core
} // namespace aw