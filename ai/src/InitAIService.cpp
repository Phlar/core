#include "InitAIService.hpp"

#include "AIService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace ai {

void RegisterService(base::ServiceLocatorPtr serviceLocator) {

	if(!serviceLocator) {
		throw std::invalid_argument("Invalid service-locator to register AI service to.");
	}
	
    serviceLocator->RegisterService(IAIServicePtr(new AIService()));
}

} // namespace ai
} // namespace core
} // namespace aw