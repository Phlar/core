#pragma once

#include "AIFwdDeclarations.hpp"
#include "IScriptAction.hpp"

#include "Action.hpp"
#include "InterfaceImpl.hpp"

#include "IScriptContext.hpp"

#include <memory>


namespace aw {
namespace core {
namespace ai {
namespace impl {

class ScriptAction : public base::InterfaceImpl<IScriptAction>, public impl::Action {

    public:

        ScriptAction();
        virtual ~ScriptAction();

        //@{
        //! Implementations of IScriptAction.
        virtual void SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);
        //@}

    protected:

        // Override the execute function from the Action class.
        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;


        // Merely some storage of script specific properties.
        struct ScriptProperties {

            public:

                ScriptProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);

                // Actually load the script - i.e. validate its location and retrieve the appropriate context.
                void resolveScript();

                boost::filesystem::path m_filePath;
                std::string m_functionName;
                scripting::IScriptContextPtr m_scriptContext;

            protected:
                ScriptProperties();
        };
        
        std::unique_ptr<ScriptProperties> m_scriptProperties;

};
typedef boost::intrusive_ptr<ScriptAction> ScriptActionPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw