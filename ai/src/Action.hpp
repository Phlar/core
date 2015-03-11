#pragma once

#include "IAction.hpp"

#include "Task.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Action : public base::InterfaceImpl<IAction>, public Task {

    public:

        Action(const ActionFnc& action);
        virtual ~Action();

        //! \brief Returns result of the provided action function.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //! action function provided.
        TaskResult Evaluate() override;

        void SetAction(const ActionFnc& action) override;

    protected:

        ActionFnc m_action;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw