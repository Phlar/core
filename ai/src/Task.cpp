#pragma once

#include "Task.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

Task::Task()
: m_parents() {
}

Task::~Task() {
}

TaskList Task::GetParents() const {

    return m_parents;
}

} // namespace impl
} // namespace ai
} // namespace core 
} // namespace aw