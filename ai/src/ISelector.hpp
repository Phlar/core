#pragma once

#include "ITaskContainer.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {


//! \brief Basic sequencer-task which executes its child tasks sequentially until one fails. 
//! In case of no child being attached, Evaluate() reuturns TaskResult::TASK_RESULT_PASSED.
class ISelector : public virtual ITaskContainer {
};
typedef boost::intrusive_ptr<ISelector> ISelectorPtr;

} // namespace ai
} // namespace core
} // namespace aw