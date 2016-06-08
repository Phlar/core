#include "AIService.hpp"

#include "Action.hpp"
#include "BehaviorTree.hpp"
#include "Condition.hpp"
#include "ScriptAction.hpp"
#include "ScriptCondition.hpp"
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

IActionPtr AIService::createAction() const {

    return IActionPtr(new impl::Action(ActionFnc()));
}

IActionPtr AIService::createAction(const ActionFnc& action) const {

    return IActionPtr(new impl::Action(action));
}

IScriptActionPtr AIService::createScriptAction() const {

    return IScriptActionPtr(new impl::ScriptAction());
}

IScriptActionPtr AIService::createScriptAction(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const {

    return IScriptActionPtr(new impl::ScriptAction(filePath, functionName, delayLoad));
}

IConditionPtr AIService::createCondition() const {

    return IConditionPtr(new impl::Condition(ConditionFnc()));
}

IConditionPtr AIService::createCondition(const ConditionFnc& condition) const {

    return IConditionPtr(new impl::Condition(condition));
}

IScriptConditionPtr AIService::createScriptCondition() const {

    return IScriptConditionPtr(new impl::ScriptCondition());
}

IScriptConditionPtr AIService::createScriptCondition(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const {

    return IScriptConditionPtr(new impl::ScriptCondition(filePath, functionName, delayLoad));
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