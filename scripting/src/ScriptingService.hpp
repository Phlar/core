#pragma once

#include "IService.hpp"
#include "IScriptResolver.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>

#include <set>

namespace aw {
namespace core {
namespace scriptingservice {

//! \brief Service that manages all possible scripting resolvers.
//! When executing a script the service takes care about dispatching
//! it to the appropriate resolver for execution.
class ScriptingService : public base::InterfaceImpl<base::IService> {

    public:

        ScriptingService();

        virtual ~ScriptingService();

        //@{
        //! Implementations of IService
        virtual base::UUID GetServiceID() const;
        //@}

        //! \brief Adds a resolver for a certain kind of scripts.
        void AddResolver(IScriptResolverPtr resolver);

        //! \brief Tries to find a matching resolver.
        IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath) const;

        static const base::UUID serviceID;

    protected:

        // All registered interpreters.
        std::set<IScriptResolverPtr> m_resolvers;
};

} // namespace scriptingservice
} // namespace core
} // namespace aw



