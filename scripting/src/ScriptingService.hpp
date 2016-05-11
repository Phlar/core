#pragma once

#include "IScriptResolver.hpp"
#include "IScriptingService.hpp"

#include "IService.hpp"
#include "ServiceLocator.hpp"
#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>

#include <set>

namespace aw {
namespace core {
namespace scripting {

class ScriptingService : public base::InterfaceImpl<core::scripting::IScriptingService> {

    public:

        ScriptingService();

        virtual ~ScriptingService();

        //@{
        //! Implementations of IService
        virtual base::UUID GetServiceID() const;
        virtual void CreateService(base::ServiceLocatorPtr serviceLocator);
        //@}

        //! \brief Adds a resolver for a certain kind of scripts.
        virtual void AddResolver(IScriptResolverPtr resolver);

        //! \brief Tries to find a matching resolver.
        virtual IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath) const;

        static const base::UUID serviceID;

    protected:

        // All registered interpreters.
        std::set<IScriptResolverPtr> m_resolvers;
};

} // namespace scripting
} // namespace core
} // namespace aw



