#pragma once

#include "AIFwdDeclarations.hpp"
#include "IAction.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

//! \brief Specialized action that executes a script-snippet.
class IScriptAction : public virtual IAction {
};
typedef boost::intrusive_ptr<IScriptAction> IScriptActionPtr;

} // namespace ai
} // namespace core
} // namespace aw