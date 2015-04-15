#pragma once

#include "IDecorator.hpp"

#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

typedef boost::function<bool(void)> RepeatCondition;

//! Decorator task that repeats its underlying task as long as the repeat
//! condition is evaluated to true.
//! The evaluation-function is invoked exactly once before a potential
//! execution of the sub-task.
//! In case of no RepeatCondition is set the sub-task is executed once.
class IRepeater : public virtual IDecorator {

    //! \brief Set the repeat condition to evaluate prior to the sub-task.
    virtual void SetRepeatCondition(const RepeatCondition& repeatCondition) = 0;
};
typedef boost::intrusive_ptr<IRepeater> IRepeaterPtr;

} // namespace ai
} // namespace core
} // namespace aw