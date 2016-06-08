#pragma once

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {

class IBehaviorTree;
typedef boost::intrusive_ptr<IBehaviorTree> IBehaviorTreePtr;

class ITask;
typedef boost::intrusive_ptr<ITask> ITaskPtr;

class ITaskContainer;
typedef boost::intrusive_ptr<ITaskContainer> ITaskContainerPtr;

class IAction;
typedef boost::intrusive_ptr<IAction> IActionPtr;

class IScriptAction;
typedef boost::intrusive_ptr<IScriptAction> IScriptActionPtr;

class ICondition;
typedef boost::intrusive_ptr<ICondition> IConditionPtr;

class IScriptCondition;
typedef boost::intrusive_ptr<IScriptCondition> IScriptConditionPtr;

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

} // namespace ai
} // namespace core
} // namespace aw