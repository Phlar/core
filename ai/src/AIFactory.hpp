#pragma once

#include "AIFwdDeclarations.hpp"

#include "IAction.hpp"
#include "ICondition.hpp"


namespace aw {
namespace core {
namespace ai {


class AIFactory {

    public:
        AIFactory();
        virtual ~AIFactory();

        // Todo: Check how to provide default parameters in case of pass by reference!
        IActionPtr      createAction(const ActionFnc& action) const;
        IConditionPtr   createCondition(const ConditionFnc& condition) const;
        ISequencerPtr   createSequencer() const;
        ISelectorPtr    createSelector() const;
};

} // namespace ai
} // namespace core
} // namespace aw