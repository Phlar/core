#pragma once

#include "IScriptContext.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace scripting {
namespace lua {

class LUAScriptContext : public base::InterfaceImpl<IScriptContext> {

    public:
        LUAScriptContext();
        virtual ~LUAScriptContext();

        //@{
        //! Implementations of IScriptContext.
        virtual void ExecuteScript(const std::string& functionName,
                                   const std::vector<Argument>& params);
        //@}
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw