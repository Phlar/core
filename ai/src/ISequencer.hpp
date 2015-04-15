#pragma once

#include "ITaskContainer.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

   
//! \brief Basic sequencer-task which executes its child tasks sequentially until one succeeds. 
//! In case of no child being attached, Evaluate() returns TaskResult::TASK_RESULT_PASSED.
class ISequencer : public virtual ITaskContainer {
};
typedef boost::intrusive_ptr<ISequencer> ISequencerPtr;

} // namespace ai
} // namespace core
} // namespace aw