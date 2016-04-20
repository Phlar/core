#pragma once

#include "Arguments.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace scripting {

//! \brief Script context denotes a single script instance.
class IScriptContext : public virtual base::IReferenceCounted {

    public:

        //! \brief Executes a function in the loaded script.
        //! \param functionName name of the function to invoke.
        //! \param params container of parameters passed.
        //! \param results container containing the expected
        //!  number of parameters and types.
        virtual void ExecuteScript(const std::string& functionName,
                                   const ArgumentVector& params,
                                   const ReturnValuesHolder& results) = 0;
};
typedef boost::intrusive_ptr<IScriptContext> IScriptContextPtr;

} // namespace scripting
} // namespace core
} // namespace aw
