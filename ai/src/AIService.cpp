#include "AIService.hpp"

#include "Action.hpp"
#include "BehaviorTree.hpp"
#include "Condition.hpp"
#include "Sequencer.hpp"
#include "Selector.hpp"
#include "Blackboard.hpp"
#include "Inverter.hpp"
#include "Repeater.hpp"

#include <boost/uuid/string_generator.hpp>


namespace aw {
namespace core {
namespace ai {


AIService::AIService() {
}

AIService::~AIService() {
}

base::UUID AIService::GetServiceID() const {

    return ID_AI_SERVICE;
}

IActionPtr AIService::createAction(const ActionFnc& action) const {

    return IActionPtr(new impl::Action(action));
}

IConditionPtr AIService::createCondition(const ConditionFnc& condition) const {

    return IConditionPtr(new impl::Condition(condition));
}

ISequencerPtr AIService::createSequencer() const {

    return ISequencerPtr(new impl::Sequencer());
}

ISelectorPtr AIService::createSelector() const {

    return ISelectorPtr(new impl::Selector());
}

IBlackboardPtr AIService::createBlackboard() const {

    return IBlackboardPtr(new impl::Blackboard());
}

IInverterPtr AIService::createInverter() const {

    return IInverterPtr(new impl::Inverter());
}

IRepeaterPtr AIService::createRepeater() const {

    return IRepeaterPtr(new impl::Repeater());
}

IBehaviorTreePtr AIService::createBehaviorTree() const {

    return IBehaviorTreePtr(new impl::BehaviorTree());
}

} // namespace ai
} // namespace core
} // namespace aw