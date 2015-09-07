#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test_ServiceLocator"

#include "UUID.hpp"
#include "IService.hpp"
#include "ServiceLocator.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/uuid/string_generator.hpp>


namespace aw {
namespace core {
namespace base {
namespace testing {

class DummyService : public base::InterfaceImpl<base::IService> {

    public:

        DummyService() {
        }

        base::UUID GetServiceID() const {
            return serviceID;
        }

        static const base::UUID serviceID;
};

const base::UUID DummyService::serviceID = boost::uuids::string_generator()("{11111111-2222-3333-4444-555555555555}");


BOOST_AUTO_TEST_CASE(TestServiceLocatorLifetime) {

    // Though the service-locator is a lazy initialized singleton,
    // its lifetime is determined by the last kept pointer to it.
    boost::weak_ptr<ServiceLocator> weakCheck;
    {
        ServiceLocatorPtr serviceLocator;
        weakCheck = serviceLocator;
        BOOST_CHECK(!weakCheck.lock());
        
        serviceLocator = ServiceLocator::Instance();
        weakCheck = serviceLocator;
        BOOST_CHECK(weakCheck.lock());
    }

    // Locator should be dead now.
    BOOST_CHECK(!weakCheck.lock());
}

BOOST_AUTO_TEST_CASE(RegisterServices) {

    ServiceLocatorPtr serviceLocator = ServiceLocator::Instance();

    IServicePtr validService(new DummyService());
    BOOST_CHECK_NO_THROW(serviceLocator->RegisterService(validService));
    
    // Registering twice should throw.
    BOOST_CHECK_THROW(serviceLocator->RegisterService(validService), std::exception);


    // Trying to register an "invalid" service should throw.
    IServicePtr invalidService(nullptr);
    BOOST_CHECK_THROW(serviceLocator->RegisterService(invalidService), std::exception);
}

BOOST_AUTO_TEST_CASE(GetServices) {

    ServiceLocatorPtr serviceLocator = ServiceLocator::Instance();

    IServicePtr validService(new DummyService());
    BOOST_CHECK_NO_THROW(serviceLocator->RegisterService(validService));

    IServicePtr fetchedService;
    BOOST_CHECK_NO_THROW(fetchedService = serviceLocator->GetService(DummyService::serviceID));
    BOOST_CHECK_EQUAL(validService, fetchedService);

    // Trying to retrieve an an unregistered service should throw.
    BOOST_CHECK_THROW(fetchedService = serviceLocator->GetService(
        boost::uuids::string_generator()("{00000000-0000-0000-0000-000000000000}")),
        std::exception);
}

} // namespace testing
} // namespace base
} // namespace core
} // namespace aw
