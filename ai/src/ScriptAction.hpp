#pragma once

#include "AIFwdDeclarations.hpp"
#include "IScriptAction.hpp"

#include "Action.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class ScriptAction : public base::InterfaceImpl<IScriptAction>, public impl::Action {

    public:

        ScriptAction();
        virtual ~ScriptAction();
};
typedef boost::intrusive_ptr<ScriptAction> ScriptActionPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw