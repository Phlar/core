#pragma once

#include "IService.hpp"
#include "IScriptContext.hpp"
#include "IScriptResolver.hpp"

#include "UUID.hpp"
#include "Utils.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scripting {


static const base::UUID ID_SCRIPTING_SERVICE = base::utils::CreateUUIDFromString("{237C7009-A671-4509-99E9-C9AC6C0BB78F}");

//! \brief Service that manages all possible scripting resolvers.
//! When executing a script the service takes care about dispatching
//! it to the appropriate resolver for execution.
class IScriptingService : public virtual base::IService {

    public:

        //@{
        //! Implementations of IService.
        virtual base::UUID GetServiceID() const = 0;
        //@}

        //! \brief Adds a resolver for a certain kind of scripts.
        virtual void AddResolver(IScriptResolverPtr resolver) = 0;

        //! \brief Returns a registered resolver matching by ID.
        virtual IScriptResolverPtr GetResolver(const base::UUID& resolverID) const = 0;

        //! \brief Tries to find a matching resolver.
        virtual IScriptContextPtr GetContextFromFile(const boost::filesystem::path& scriptPath) const = 0;
};
typedef boost::intrusive_ptr<IScriptingService> IScriptingServicePtr;

} // namespace scripting
} // namespace core
} // namespace aw