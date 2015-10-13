#pragma once

#include "IScriptContext.hpp"
#include "LUARegistrationFunctions.hpp"

#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>

struct lua_State;

namespace aw {
namespace core {
namespace scripting {
namespace lua {

class LUAScriptContext : public base::InterfaceImpl<IScriptContext> {

    public:
        LUAScriptContext(ConverterFunctionsPtr converterFunctions,
                         const boost::filesystem::path& scriptPath);
        virtual ~LUAScriptContext();

        //@{
        //! Implementations of IScriptContext.
        virtual void ExecuteScript(const std::string& functionName,
                                   const ArgumentVector& params);
        //@}

        //! \brief By default LUA runs a mark and sweep garbage collector in the background.
        //! As the garbage collector cleans at non-deterministic points in time, objects passed
        //! to LUA also cannot be guaranteed to be deleted when one expects it. As this can lead
        //! to problems especially when using smart pointers on them the following function
        //! can be used to force a run of the garbage-collector after invoking LUA code.
        void ForceGCAfterExecution(bool forceGC = false);

    protected:

        //! \brief Create a LUA state object and register all C++ types.
        void initializeLUAStateObject();

        //! \brief Helper loading the script and catching possible syntax errors.
        void loadScriptFile();

        //! \brief Helper that checks whether to call a specific function
        //! within the script or to execute the script as is without any arguments.
        void executeScript(const std::string& functionName, const ArgumentVector& params);

        //! \brief Helper the pushes all arguments onto LUA's stack.
        void pushArguments(const ArgumentVector& params);

        lua_State* m_luaState;
        boost::filesystem::path m_scriptPath;
        ConverterFunctionsPtr m_converterFunctions;
        bool m_forceGCAfterScriptExecution;
        static const bool defaultForceGCAfterExecution;
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw