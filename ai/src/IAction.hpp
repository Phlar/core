#pragma once

#include "ITask.hpp"

#include "IReferenceCounted.hpp"

#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {

typedef boost::function<TaskResult()> ActionFnc;

//! \brief 'Leaf' element that triggers a certain action - if set 
//! evaluate() will return the action's result. Otherwise TASK_RESULT_PASSED
//! will be returned.
class IAction : public virtual ITask {

    public:
        
        //! \brief Defines the action to invoke.
        virtual void SetAction(const ActionFnc& action) = 0;
};
typedef boost::intrusive_ptr<IAction> IActionPtr;

} // namespace ai
} // namespace core
} // namespace aw