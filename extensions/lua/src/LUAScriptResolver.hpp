#pragma once

#include "IScriptResolver.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace scripting {
namespace lua {

class LUAScriptResolver : public base::InterfaceImpl<IScriptResolver> {

    public:
        LUAScriptResolver();
        virtual ~LUAScriptResolver();

        //@{
        //! Implementations of IScriptResolver.
        virtual bool IsFileSupported(const boost::filesystem::path& scriptPath);
        virtual IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath);
        //@}
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw