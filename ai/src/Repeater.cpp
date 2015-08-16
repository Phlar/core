#include "Repeater.hpp"

#include "IBlackboard.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

Repeater::Repeater()
: m_repeatCondition() {
}

Repeater::~Repeater() {
}

void Repeater::SetRepeatCondition(RepeatConditionUPtr repeatCondition) {

    //! \todo Uncomment as we have logging at hand.
    /*
    if(!repeatCondition) {
        // Log this.
    }
    */

    m_repeatCondition = std::move(repeatCondition);
}

TaskResult Repeater::evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType* yield) const {

    if(!m_repeatCondition) {
        
        // Log this.
        return evaluateDecoratedTask(blackboard, yield);
    }
    else {

        TaskResult result = TaskResult::TASK_RESULT_FAILED;
        while(m_repeatCondition->Evaluate()) {

            result = evaluateDecoratedTask(blackboard, yield);
        }

        return result;
    }
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw