#pragma once

#include "IScriptResolver.hpp"
#include "LUARegistrationFunctions.hpp"

#include "InterfaceImpl.hpp"

#include "luabind/scope.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/type_index.hpp>


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

        //! \brief Add function to register C++ types and functions 
        //! towards LUA. Registration-functions must return the 
        //! "exposed data / functions" utilizing the luabind::scope type.
        void AddTypeRegistrationFunction(const TypeRegistrationFunction& registrationFunction);


        //@{
        //! \brief Register converters that convert a value to / from LUA.
        void RegisterPushTypeToLUAFunction(const boost::typeindex::type_info& regType,
                                           const PushToLUAFunction& fnc);

        void RegisterFetchTypeFromLUAFunction(const boost::typeindex::type_info& regType,
                                              const FetchFromLUAFunction& fnc);
        //@}

    protected:

        static const char* m_supportedFileExtension;

        // When creating a context and the internal LUA state object the listed
        // functions (type / function registration) below get executed.
        ConverterFunctionsPtr m_converterFunctions;
};

} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw