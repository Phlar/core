#pragma once

#include "AIFwdDeclarations.hpp"

#include "Task.hpp"

#include "IScriptContext.hpp"

#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <memory>


namespace aw {
namespace core {
namespace ai {
namespace impl {


//! \brief Since an Action and Condition share a lot of code this class acts as 
//! a common base class providing means to extend it with user code to be run.
class RunnableScriptTask : public Task, public boost::noncopyable {

    public:

        enum class RunnableScriptTaskType {

            RUNNABLE_SCRIPT_TASK_ACTION = 0,
            RUNNABLE_SCRIPT_TASK_CONDITION
        };

        virtual ~RunnableScriptTask();

    protected:

        // Enable construction only from classes down the inheritance chain.
        RunnableScriptTask(RunnableScriptTaskType runnableScriptTaskType);

        //! \brief Returns result of the provided function to be run.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //!  function provided.
        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;

        //! \brief Specify the file to load and the function to invoke upon execution.
        void setScriptFileSource(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);

        //! \brief Specify the script's source coming from a plain string.
        void setScriptStringSource(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad);

        //@{
        // Holders of specific script-properties. Either a script can be loaded from file or from a plain string source.
        class ScriptProperties : public boost::noncopyable {

            public:
                ScriptProperties(const std::string& functionName, const std::string& scriptSource = "");
                virtual ~ScriptProperties();

                virtual void resolveScript() = 0;
                virtual std::string buildPropertiesMessage() = 0;

                const std::string m_functionName;
                std::string m_scriptSource;
                scripting::IScriptContextPtr m_scriptContext;

            private:
                ScriptProperties();
        };
        typedef std::unique_ptr<ScriptProperties> ScriptPropertiesPtr;

        class ScriptFileProperties : public ScriptProperties {

            public:
                ScriptFileProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);
                virtual ~ScriptFileProperties();

                virtual void resolveScript();
                virtual std::string buildPropertiesMessage();

            protected:
                const boost::filesystem::path m_filePath;

            private:
                ScriptFileProperties();
        };

        class ScriptStringProperties : public ScriptProperties {

            public:
                ScriptStringProperties(const std::string& scriptSource, const base::UUID& resolverID, const std::string& functionName, bool delayLoad);
                virtual ~ScriptStringProperties();

                virtual void resolveScript();
                virtual std::string buildPropertiesMessage();

            protected:
                const base::UUID m_resolverID;

            private:
                ScriptStringProperties();
        };
        //@}

        // Holder of the actual script's properties.
        ScriptPropertiesPtr m_scriptProperties;
        
        // Type of action does this node represents.
        const RunnableScriptTaskType m_runnableScriptTaskType;

    private:

        // Disable default construction.
        RunnableScriptTask();
};

typedef boost::intrusive_ptr<RunnableScriptTask> RunnableScriptTaskPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace 