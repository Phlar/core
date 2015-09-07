#pragma once

#include "UUID.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

namespace aw {
namespace core {
namespace base {

class IService;
typedef boost::intrusive_ptr<IService> IServicePtr;

class ServiceLocator;
typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;


//! \brief Thread-safe registry that keeps registered services available.
//! Implemented as a singleton that will be destroyed when released.
class ServiceLocator : public boost::enable_shared_from_this<ServiceLocator> {

    public:

        //! \brief Get the one and only instance.
        static ServiceLocatorPtr Instance();

        //! \brief Adds a new service to the registry.
        //! Throws an exception in case of already registered.
        void RegisterService(IServicePtr service);

        //!  \brief Returns the registered service or throws an exception
        //! in case of not listed.
        IServicePtr GetService(const base::UUID& serviceID) const;

        virtual ~ServiceLocator();

    protected:

        ServiceLocator();

        // Use a weak pointer here in order not to increase the reference count.
        static boost::weak_ptr<ServiceLocator> m_weakInstance;

        // Enable concurrent access.
        static boost::mutex m_mutex;

        // The registry.
        boost::unordered_map<base::UUID, IServicePtr> m_services;
};

} // namespace base
} // namespace core
} // namespace aw