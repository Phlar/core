#pragma once

#include "IDecorator.hpp"

#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

#include <memory>

namespace aw {
namespace core {
namespace ai {

//! Base class for conditions to be used by the IRepeater interface containing
//! the actual condition functor as well as means to "reset" after execution.
class RepeatCondition {

    public:

        virtual ~RepeatCondition() {};

        //! \brief Evaluates the condition and may alter the state of the it.
        //! e.g. Incrementing a counter.
        //! \returns true if condition is met - false otherwise.
        virtual bool Evaluate() = 0;

        //! \brief Subclasses may implement this in order to reset the condition's
        //! internal state after execution - i.e. the repeater decorator is left.
        virtual void ResetCondition() {};
};
typedef std::unique_ptr<RepeatCondition> RepeatConditionUPtr;


//! Decorator task that repeats its underlying task as long as the repeat
//! condition is evaluated to true. It works similar to a "while" loop
//! meaning the task below gets might not get evaluated once.
//! The evaluation-function is invoked exectly once ffor each call to the child.
//! In case of no RepeatCondition is set the sub-task is executed once.
//! \Todo: Clarify what the actual semantic meaning of the Evaluate() return
//! value is in case of the repeater. Should it e.g. return the result of the
//! last run sub-tree? Or is it supposed to return "success" in all cases?
class IRepeater : public virtual IDecorator {

    public:

        //! \brief Set the repeat condition to evaluate prior to the sub-task.
        virtual void SetRepeatCondition(RepeatConditionUPtr repeatCondition) = 0;
};
typedef boost::intrusive_ptr<IRepeater> IRepeaterPtr;

} // namespace ai
} // namespace core
} // namespace aw