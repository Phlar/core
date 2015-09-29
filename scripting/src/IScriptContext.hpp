#pragma once

#include "Arguments.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scriptingservice {

//! \brief Script context denotes a single script instance.
class IScriptContext : public virtual base::IReferenceCounted {

    public:

        //! \brief Executes a function in the loaded script.
        //! \param functionName name of the function to invoke.
        //! \param params container of parameters passed.
        virtual void ExecuteScript(const std::string& functionName,
                                   const std::vector<Argument>& params) = 0;
};
typedef boost::intrusive_ptr<IScriptContext> IScriptContextPtr;

} // namespace scriptingservice
} // namespace core
} // namespace aw
