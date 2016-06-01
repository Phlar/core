#include "ScriptAction.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

ScriptAction::ScriptAction()
: Action(ActionFnc()){
}

ScriptAction::~ScriptAction() {
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw