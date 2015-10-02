#pragma once

#include "IScriptContext.hpp"

#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>

class lua_State;

namespace aw {
namespace core {
namespace scripting {
namespace lua {

class LUAScriptContext : public base::InterfaceImpl<IScriptContext> {

    public:
        LUAScriptContext(const boost::filesystem::path& scriptPath);
        virtual ~LUAScriptContext();

        //@{
        //! Implementations of IScriptContext.
        virtual void ExecuteScript(const std::string& functionName,
                                   const ArgumentVector& params);
        //@}

    protected:

        //! \brief Helper loading the script and catching possible syntax errors.
        void loadScriptFile();

        //! \brief Helper that checks whether to call a specific function
        //! within the script or to execute the script as is without any arguments.
        void executeScript(const std::string& functionName, const ArgumentVector& params);

        //! \brief Helper the pushes all arguments onto LUA's stack.
        void pushArguments(const ArgumentVector& params);

        lua_State* m_luaState;
        boost::filesystem::path m_scriptPath;
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw