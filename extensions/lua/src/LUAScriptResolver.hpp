#pragma once

#include "IScriptResolver.hpp"
#include "LUATypedefs.hpp"

#include "InterfaceImpl.hpp"

#include "luabind/scope.hpp"

#include <boost/shared_ptr.hpp>


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

        //! \brief Add a LUA registration function.
        void AddRegistrationFunction(const RegistrationFunction& registrationFunction);

    protected:

        static const char* m_supportedFileExtension;

        // When creating a context and the internal LUA state object the listed
        // functions below get executed.
        RegistrationFunctionsPtr m_registrationFunctions;
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw