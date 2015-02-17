#pragma once

#include "IAction.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Action : public IAction {

    public:
        Action(const ActionFnc& action);
        virtual ~Action();

        //! \brief Returns result of the provided action function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! action function provided.
        TaskResult evaluate();

        void SetAction(const ActionFnc& action);

    protected:

        ActionFnc m_action;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw