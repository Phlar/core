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

IScriptActionPtr AIService::createScriptActionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const {

    IScriptActionPtr scriptAction(new impl::ScriptAction());
    scriptAction->SetScriptFile(filePath, functionName, delayLoad);
    return scriptAction;
}

IScriptActionPtr AIService::createScriptActionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const {

    IScriptActionPtr scriptAction(new impl::ScriptAction());
    scriptAction->SetScriptString(scriptSource, resolverID, functionName, delayLoad);
    return scriptAction;
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

IScriptConditionPtr AIService::createScriptConditionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const {

    IScriptConditionPtr scriptCondition(new impl::ScriptCondition());
    scriptCondition->SetScriptFile(filePath, functionName, delayLoad);
    return scriptCondition;
}

IScriptConditionPtr AIService::createScriptConditionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const {

    IScriptConditionPtr scriptCondition(new impl::ScriptCondition());
    scriptCondition->SetScriptString(scriptSource, resolverID, functionName, delayLoad);
    return scriptCondition;
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