#pragma once

#include "IAIService.hpp"
#include "ServiceLocator.hpp"
#include "InitAIService.hpp"

namespace aw {
namespace core {
namespace ai {
namespace testing {

struct AIServiceFixture {

    public:

        AIServiceFixture()
        : serviceLocator(base::ServiceLocatorPtr())
        , aiService(IAIServicePtr()) {

            BOOST_REQUIRE_NO_THROW(serviceLocator = base::ServiceLocator::Instance());
            BOOST_REQUIRE(serviceLocator);

            BOOST_REQUIRE_NO_THROW(RegisterService(serviceLocator));

            // Check if service is available...
            base::IServicePtr service = base::IServicePtr();
            BOOST_REQUIRE_NO_THROW(service = serviceLocator->GetService(ID_AI_SERVICE));
            BOOST_REQUIRE(service);

            // ...and the right one...
            aiService = boost::dynamic_pointer_cast<IAIService>(service);
            BOOST_REQUIRE(aiService);
        }

        virtual ~AIServiceFixture() {
        }

    base::ServiceLocatorPtr serviceLocator;
    IAIServicePtr aiService;
};

} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw