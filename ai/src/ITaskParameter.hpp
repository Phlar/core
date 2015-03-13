#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

//! \brief In order to be able to reuse behavior trees by multiple users,
//! there must not be any data kept within a task that is required to
//! control the flow down the tree. For example, when coming back to a selector
//! node from a sub-tree this selector node has to know the next child to
//! to process. As storage in the selector node itself would interfere
//! with the execution of the tree by someone else, we will use this parameter
//! structure for storing transient data 'affecting the control flow'.
//! In case of a node requires such data it asks the used blackboard to
//! do a lookup for a parameter structure of the task's ID.
//! \todo: Check whether we could directly keep a reference to the task
//! itself in here...
class ITaskParameter : public virtual base::IReferenceCounted {

    public:

        //! \brief Returns the unique identifier of the task this 
        //! parameter structure is associated to.
        virtual UUID GetAssociatedTaskID() const = 0;
};

typedef boost::intrusive_ptr<ITaskParameter> ITaskParameterPtr;

} // namespace ai
} // namespace core
} // namespace aw
