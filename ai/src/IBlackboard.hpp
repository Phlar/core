#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>

#include <list>


namespace aw {
namespace core {
namespace ai {

//! \brief Storage container in order to exchange data
//! while processing the tree. Data is stored in a 
//! key / value store where key = UUID returned via
//! IBlackboardValue::GetTypeID() and a collecction
//! of IBlackboardValues. Within a container of a certain type
//! the values must be unique by ID. However across containers
//! IDs of values need not be unique.
//! \todo Check if we shouldn't be more strict on the above statement!
//! Moreover this container also stores all task-relevant data
//! a tasks needs in order to be processed. \see ITaskParameter.
class IBlackboard : public virtual base::IReferenceCounted {

    public:

        //! \brief Adds a value to the board.
        //! Value must not be null nor a duplicate.
        //! In case of a value with the same semantic identifier is already
        //! to the blackboard, it gets updated.
        virtual void SetValue(IBlackboardValuePtr value) = 0;

        //! \brief Returns all values matching by type-ID.
        virtual IBlackboardValuePtr GetValue(const UUID& semanticTypeID) const = 0;

        //! \brief Adds or updated a task-parameter structure to the lookup.
        virtual void StoreTaskParameter(ITaskParameterPtr taskParameter) = 0;

        //! \brief Performs a lookup for a parameter based on a task-ID.
        virtual ITaskParameterPtr GetTaskParameter(const UUID& taskID) const = 0;

        //! \brief Removes a task-parameter structure based on a task-ID.
        virtual bool RemoveTaskParameter(const UUID& taskID) = 0;
};

typedef boost::intrusive_ptr<IBlackboard> IBlackboardPtr;

} // namespace ai
} // namespace core
} // namespace aw