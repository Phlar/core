#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/function.hpp>

#include <list>
#include <vector>

namespace aw {
namespace core {
namespace ai {

class ITask;
typedef boost::intrusive_ptr<ITask> ITaskPtr;

class ITaskContainer;
typedef boost::intrusive_ptr<ITask> ITaskContainerPtr;

class IAction;
typedef boost::intrusive_ptr<IAction> IActionPtr;

class ICondition;
typedef boost::intrusive_ptr<ICondition> IConditionPtr;

class ISequencer;
typedef boost::intrusive_ptr<ISequencer> ISequencerPtr;

class ISelector;
typedef boost::intrusive_ptr<ISelector> ISelectorPtr;

typedef std::list<ITaskPtr> TaskList;
typedef std::vector<ITaskPtr> TaskVector;

} // namespace ai
} // namespace core
} // namespace aw