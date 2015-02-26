#pragma once

#include "IAction.hpp"
#include "ICondition.hpp"

namespace aw {
namespace core {
namespace ai {


class AIFactory {

    public:
        AIFactory();
        virtual ~AIFactory();

        IActionPtr createAction(const ActionFnc& action) const;
        IConditionPtr createCondition(const ConditionFnc& condition) const;
};

} // namespace ai
} // namespace core
} // namespace aw