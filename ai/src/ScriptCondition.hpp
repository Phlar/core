#pragma once

#include "AIFwdDeclarations.hpp"
#include "IScriptCondition.hpp"

#include "RunnableScriptTask.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class ScriptCondition : public base::InterfaceImpl<IScriptCondition>, public impl::RunnableScriptTask {

    public:

        ScriptCondition();
        ScriptCondition(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);

        virtual ~ScriptCondition();

        //@{
        //! Implementations of IScriptAction.
        virtual void SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);
        //@}

    protected:

        // Override the execute function from the Action class.
        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};
typedef boost::intrusive_ptr<ScriptCondition> ScriptConditionPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw