#pragma once

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

class IAction;
typedef boost::intrusive_ptr<IAction> IActionPtr;

class ICondition;
typedef boost::intrusive_ptr<ICondition> IConditionPtr;


} // namespace ai
} // namespace core
} // namespace aw