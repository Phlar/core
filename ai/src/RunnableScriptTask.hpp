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
        RunnableScriptTask(RunnableScriptTaskType runnableScriptTaskType, const boost::filesystem::path& filePath,
                           const std::string& functionName, bool delayLoad);

        //! \brief Returns result of the provided function to be run.
        //! \return Evaluation result. TASK_RESULT_PASSED in case of no 
        //!  function provided.
        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;

        //! \brief Specify the file to load and the function to invoke upon execution.
        void setScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);


        // Merely some storage of script specific properties.
        struct ScriptProperties : public boost::noncopyable {

            public:

                ScriptProperties(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad);

                // Actually load the script - i.e. validate its location and retrieve the appropriate context.
                void resolveScript();

                // Helper building a error message.
                std::stringstream buildErrorMessage(const std::string& reason) const;

                const boost::filesystem::path m_filePath;
                const std::string m_functionName;
                scripting::IScriptContextPtr m_scriptContext;

            protected:
                ScriptProperties();
        };

        // Holder of the actual script's properties.
        std::unique_ptr<ScriptProperties> m_scriptProperties;
        
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
} // namespace aw