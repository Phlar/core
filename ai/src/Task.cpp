#include "Task.hpp"

#include "IBlackboard.hpp"
#include "ITaskParameter.hpp"

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

TaskResult Task::Evaluate(IBlackboardPtr blackboard) const {

    preEvaluate(blackboard);
    TaskResult result = evaluate(blackboard);
    postEvaluate(blackboard);

    return result;
}

ITaskParameterPtr Task::CreateTaskParameter() const {

    // By default no parameter-structure is returned.
    return ITaskParameterPtr();
}

void Task::preEvaluate(IBlackboardPtr blackboard) const {

}

void Task::postEvaluate(IBlackboardPtr blackboard) const {

}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
