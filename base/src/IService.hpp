#pragma once

#include "UUID.hpp"
#include "IReferenceCounted.hpp"

#include "InterfaceImpl.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace aw {
namespace core {
namespace base {


//! \brief Interface describing a service.
class IService : public virtual base::IReferenceCounted,
                 public boost::noncopyable {

    public:

        //! \brief Returns the unique ID of the service.
        virtual UUID GetServiceID() const = 0;
};
typedef boost::intrusive_ptr<IService> IServicePtr;

} // namespace base
} // namespace core
} // namespace aw