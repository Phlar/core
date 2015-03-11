#include "Action.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

Action::Action(const ActionFnc& action)
: m_action(action) {
}

Action::~Action() {
}

TaskResult Action::Evaluate() {

    if(m_action) {
        return m_action();
    }

    return TaskResult::TASK_RESULT_PASSED;
}

void Action::SetAction(const ActionFnc& action) {

    //! Todo: Log in case of an empty functor passed.
    //        "TASK_RESULT_PASSED" will be always returned in that case.
    m_action = action;
}



} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw