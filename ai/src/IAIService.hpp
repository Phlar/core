#pragma once

#include "IService.hpp"

#include "UUID.hpp"
#include "Utils.hpp"

#include "AIFwdDeclarations.hpp"
#include "IAction.hpp"
#include "ICondition.hpp"

#include <boost/filesystem.hpp>
#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


static const base::UUID ID_AI_SERVICE = base::utils::CreateUUIDFromString("{F9A33C54-0178-4D37-B12A-FA364C52D5F4}");

//! \brief Service providing all means accessing the AI subsystem.
class IAIService : public virtual base::IService {

    public:

        //@{
        //! Implementations of IService.
        virtual base::UUID GetServiceID() const = 0;
        //@}

        //@{
        //! \brief Creator functions for all behavior-tree related elements.
        virtual IActionPtr          createAction() const = 0;
        virtual IActionPtr          createAction(const ActionFnc& action) const = 0;
        virtual IScriptActionPtr    createScriptAction() const = 0;
        virtual IScriptActionPtr    createScriptActionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const = 0;
        virtual IScriptActionPtr    createScriptActionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const = 0;
        virtual IConditionPtr       createCondition() const = 0;
        virtual IConditionPtr       createCondition(const ConditionFnc& condition) const = 0;
        virtual IScriptConditionPtr createScriptCondition() const = 0;
        virtual IScriptConditionPtr createScriptConditionFromFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) const = 0;
        virtual IScriptConditionPtr createScriptConditionFromString(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad) const = 0;
        virtual ISequencerPtr       createSequencer() const = 0;
        virtual ISelectorPtr        createSelector() const = 0;
        virtual IBlackboardPtr      createBlackboard() const = 0;
        virtual IInverterPtr        createInverter() const = 0;
        virtual IRepeaterPtr        createRepeater() const = 0;
        virtual IBehaviorTreePtr    createBehaviorTree() const = 0;
        //@}
};
typedef boost::intrusive_ptr<IAIService> IAIServicePtr;

} // namespace ai
} // namespace core
} // namespace aw