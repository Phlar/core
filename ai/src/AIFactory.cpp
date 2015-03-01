#include "AIFactory.hpp"

#include "Action.hpp"
#include "Condition.hpp"
#include "Sequencer.hpp"
#include "Selector.hpp"

namespace aw {
namespace core {
namespace ai {

AIFactory::AIFactory() {
}

AIFactory::~AIFactory() {
}

IActionPtr AIFactory::createAction(const ActionFnc& action) const {

    return IActionPtr(new impl::Action(action));
}

IConditionPtr AIFactory::createCondition(const ConditionFnc& condition) const {

    return IConditionPtr(new impl::Condition(condition));
}

ISequencerPtr AIFactory::createSequencer() const {

    return ISequencerPtr(new impl::Sequencer());
}

ISelectorPtr AIFactory::createSelector() const {

    return ISelectorPtr(new impl::Selector());
}

} // namespace ai
} // namespace core
} // namespace aw