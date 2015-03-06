#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <list>


namespace aw {
namespace core {
namespace ai {

//! \brief Storage container in order to exchange data
//! while processing the tree. Data is stored in a 
//! key / value store where key = UUID returned via
//! IBlackboardValue::GetTypeID() and a collecction
//! of IBlackboardValues.
class IBlackboard : public virtual base::IReferenceCounted {

    public:

        //! \brief Adds a value to the board.
        //! Value must not be null nor a duplicate.
        //! New values are appended to the collection.
        virtual void AddValue(IBlackboardValuePtr value) = 0;

        //! \brief Returns all values matching by type-ID.
        virtual BlackboardValueList GetValuesByType(const UUID& typeID) const = 0;

};

} // namespace ai
} // namespace core
} // namespace aw