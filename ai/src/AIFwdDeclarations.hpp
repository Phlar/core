#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/function.hpp>
#include <boost/uuid/uuid.hpp>

#include <list>
#include <vector>

namespace aw {
namespace core {
namespace ai {

class ITask;
typedef boost::intrusive_ptr<ITask> ITaskPtr;

class ITaskContainer;
typedef boost::intrusive_ptr<ITask> ITaskContainerPtr;

class ITaskParameter;
typedef boost::intrusive_ptr<ITaskParameter> ITaskParameterPtr;

class IAction;
typedef boost::intrusive_ptr<IAction> IActionPtr;

class ICondition;
typedef boost::intrusive_ptr<ICondition> IConditionPtr;

class ISequencer;
typedef boost::intrusive_ptr<ISequencer> ISequencerPtr;

class ISelector;
typedef boost::intrusive_ptr<ISelector> ISelectorPtr;

class IBlackboard;
typedef boost::intrusive_ptr<IBlackboard> IBlackboardPtr;

class IBlackboardValue;
typedef boost::intrusive_ptr<IBlackboardValue> IBlackboardValuePtr;


typedef boost::uuids::uuid UUID;

typedef std::list<ITaskPtr> TaskList;
typedef std::vector<ITaskPtr> TaskVector;

typedef std::list<IBlackboardValuePtr> BlackboardValueList;
typedef std::vector<IBlackboardValuePtr> BlackboardValueVector;

} // namespace ai
} // namespace core
} // namespace aw