#pragma once

#include "AIFwdDeclarations.hpp"
#include "IScriptAction.hpp"

#include "RunnableScriptTask.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class ScriptAction : public base::InterfaceImpl<IScriptAction>, public impl::RunnableScriptTask {

    public:

        ScriptAction();
        ScriptAction(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);
        virtual ~ScriptAction();

        //@{
        //! Implementations of IScriptAction.
        virtual void SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);
        //@}

    protected:

        // Override the execute function from the Action class.
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};
typedef boost::intrusive_ptr<ScriptAction> ScriptActionPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw