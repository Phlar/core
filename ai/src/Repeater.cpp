#include "Repeater.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Repeater::Repeater() {
}

Repeater::~Repeater() {
}

void Repeater::SetRepeatCondition(const RepeatCondition& /*repeatCondition*/) {


}

TaskResult Repeater::evaluate(IBlackboardPtr blackboard) const {

    return TaskResult::TASK_RESULT_PASSED;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw