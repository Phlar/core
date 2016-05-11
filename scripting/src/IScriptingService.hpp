#pragma once

#include "IService.hpp"
#include "IScriptContext.hpp"
#include "IScriptResolver.hpp"

#include "UUID.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scripting {

//! \brief Service that manages all possible scripting resolvers.
//! When executing a script the service takes care about dispatching
//! it to the appropriate resolver for execution.
class IScriptingService : virtual base::IService {

    public:

        //@{
        //! Implementations of IService
        virtual base::UUID GetServiceID() const = 0;
        //@}

        //! \brief Adds a resolver for a certain kind of scripts.
        virtual void AddResolver(IScriptResolverPtr resolver) = 0;

        //! \brief Tries to find a matching resolver.
        virtual IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath) const = 0;
};
typedef boost::intrusive_ptr<IScriptingService> IScriptingServicePtr;

} // namespace scripting
} // namespace core
} // namespace aw