#pragma once

#include "IAIService.hpp"

#include "IService.hpp"
#include "UUID.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {

class AIService : public base::InterfaceImpl<core::ai::IAIService> {

    public:

        AIService();

        virtual ~AIService();

        //@{
        //! Implementations of IService.
        virtual base::UUID GetServiceID() const;
        //@}

        //@{
        //! Creators for behavior tree elements.
        virtual IActionPtr          createAction() const;
        virtual IActionPtr          createAction(const ActionFnc& action) const;
        virtual IScriptActionPtr    createScriptAction() const;
        virtual IScriptActionPtr    createScriptActionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const;
        virtual IScriptActionPtr    createScriptActionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const;
        virtual IConditionPtr       createCondition() const;
        virtual IConditionPtr       createCondition(const ConditionFnc& condition) const;
        virtual IScriptConditionPtr createScriptCondition() const;
        virtual IScriptConditionPtr createScriptConditionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const;
        virtual IScriptConditionPtr createScriptConditionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const;
        virtual ISequencerPtr       createSequencer() const;
        virtual ISelectorPtr        createSelector() const;
        virtual IBlackboardPtr      createBlackboard() const;
        virtual IInverterPtr        createInverter() const;
        virtual IRepeaterPtr        createRepeater() const;
        virtual IBehaviorTreePtr    createBehaviorTree() const;
        //@}
};

} // namespace ai
} // namespace core
} // namespace aw