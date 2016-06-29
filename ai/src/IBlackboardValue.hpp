#pragma once

#include "AIFwdDeclarations.hpp"

#include "UUID.hpp"
#include "IReferenceCounted.hpp"

#include <boost/any.hpp>
#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


//! \brief Base class of values stored on a blackboard.
//! Implementations have to provide a unique identifier.
class IBlackboardValue : public virtual base::IReferenceCounted {

    public:

        //! \brief Returns the unique identifier for this class.
        virtual base::UUID GetSemanticTypeID() const = 0;

        //! \brief Returns the unique identifier for this instance.
        virtual base::UUID GetID() const = 0;

        //! \brief Assigns a value to the blackboard-value.
        virtual void AssignValue(const boost::any& val) = 0;

        //! \brief Retrieves the stored value.
        virtual boost::any GetValue() const = 0;
};

typedef boost::intrusive_ptr<IBlackboardValue> IBlackboardValuePtr;

} // namespace ai
} // namespace core
} // namespace aw