#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


//! \brief Base class of values stored on a blackboard.
//! Implementations have to provide a unique identifier.
class IBlackboardValue : public virtual base::IReferenceCounted {

    public:

        //! \brief Returns the unique identifier for this class.
        virtual UUID GetTypeID() const = 0;

        //! \brief Returns the unique identifier for this instance.
        virtual UUID GetID() const = 0;

};

typedef boost::intrusive_ptr<IBlackboardValue> IBlackboardValuePtr;

} // namespace ai
} // namespace core
} // namespace aw