#pragma once

#include "ITask.hpp"

#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {


typedef boost::function<TaskResult()> ConditionFnc;

//! \brief 'Leaf' in a behavior tree where a condition is checked.
//! On calling Evaluate() the condition returns its result - if set.
//! In case of no condition is provided Evaluate() will return TASK_RESULT_PASSED.
class ICondition : public virtual ITask {

    public:

        //! \brief Defines the condition to check.
        virtual void SetCondition(const ConditionFnc& condition) = 0;        
};
typedef boost::intrusive_ptr<ICondition> IConditionPtr;

} // namespace ai
} // namespace core
} // namespace aw