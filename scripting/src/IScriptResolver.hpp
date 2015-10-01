#pragma once

#include "IScriptContext.hpp"

#include "IReferenceCounted.hpp"

#include <boost/filesystem.hpp>
#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scripting {

//! \brief Represents an script resolver of a certain kind.
//! Provides and creates script contexts.
class IScriptResolver : public virtual base::IReferenceCounted {

    public:

        //! \brief Checks whether a script file can be executed
        //! by this resolver / interpreter.
        virtual bool IsFileSupported(const boost::filesystem::path& scriptPath) = 0;

        //! \brief Loads the script and returns a context for later execution.
        virtual IScriptContextPtr GetContext(const boost::filesystem::path& scriptPath) = 0;
};
typedef boost::intrusive_ptr<IScriptResolver> IScriptResolverPtr;

} // namespace scripting
} // namespace core
} // namespace aw



