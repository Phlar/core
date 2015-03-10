#include "Task.hpp"

#include <boost/uuid/random_generator.hpp>

namespace aw {
namespace core {
namespace ai {
namespace impl {

Task::Task()
: m_id(boost::uuids::random_generator()()) {
}

Task::~Task() {
}

UUID Task::GetID() const {

    return m_id;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
