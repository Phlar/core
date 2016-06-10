#pragma once

#include "IScriptingService.hpp"
#include "IScriptResolver.hpp"

#include "IService.hpp"
#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>


namespace aw {
namespace core {
namespace scripting {

class ScriptingService : public base::InterfaceImpl<core::scripting::IScriptingService> {

    public:

        ScriptingService();

        virtual ~ScriptingService();

        //@{
        //! Implementations of IService.
        virtual base::UUID GetServiceID() const;
        virtual void AddResolver(IScriptResolverPtr resolver);
        virtual IScriptResolverPtr GetResolver(const base::UUID& resolverID) const;
        virtual IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath) const;
        //@}

    protected:

        // All registered interpreters.
        boost::unordered_map<base::UUID, scripting::IScriptResolverPtr> m_resolvers;
};

} // namespace scripting
} // namespace core
} // namespace aw



