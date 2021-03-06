#include "Task.hpp"

#include "IBlackboard.hpp"

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

base::UUID Task::GetID() const {

    return m_id;
}

ITask::TaskResult Task::Evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const {

    // Due to the used template-pattern, each executed task-derived instance will pass this
    // point, so this will be the place where to break.
    if(yield) {
        (*yield)();
    }

    preEvaluate(blackboard);
    ITask::TaskResult result = evaluate(blackboard, yield);
    postEvaluate(blackboard);

    return result;
}

void Task::preEvaluate(IBlackboardPtr /*blackboard*/) const {
}

void Task::postEvaluate(IBlackboardPtr /*blackboard*/) const {
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
