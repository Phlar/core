#pragma once

#include "AIFwdDeclarations.hpp"

#include "UUID.hpp"
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
class IBlackboard : public virtual base::IReferenceCounted {

    public:

        //! \brief Adds a value to the board.
        //! Value must not be null nor a duplicate.
        //! In case of a value with the same semantic identifier is already
        //! to the blackboard, it gets updated.
        virtual void SetValue(IBlackboardValuePtr value) = 0;

        //! \brief Returns all values matching by type-ID.
        virtual IBlackboardValuePtr GetValue(const base::UUID& semanticTypeID) const = 0;
};

typedef boost::intrusive_ptr<IBlackboard> IBlackboardPtr;

} // namespace ai
} // namespace core
} // namespace aw