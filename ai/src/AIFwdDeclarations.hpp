#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/function.hpp>
#include <boost/uuid/uuid.hpp>

#include <list>
#include <vector>

namespace aw {
namespace core {
namespace ai {

class IBehaviourTree;
typedef boost::intrusive_ptr<IBehaviourTree> IBehaviourTreePtr;

class ITask;
typedef boost::intrusive_ptr<ITask> ITaskPtr;

class ITaskContainer;
typedef boost::intrusive_ptr<ITaskContainer> ITaskContainerPtr;

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

class IDecorator;
typedef boost::intrusive_ptr<IDecorator> IDecoratorPtr;

class IInverter;
typedef boost::intrusive_ptr<IInverter> IInverterPtr;

class IRepeater;
typedef boost::intrusive_ptr<IRepeater> IRepeaterPtr;

typedef boost::uuids::uuid UUID;

} // namespace ai
} // namespace core
} // namespace aw