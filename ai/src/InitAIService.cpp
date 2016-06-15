#include "InitAIService.hpp"

#include "AIService.hpp"

#include "ServiceLocator.hpp"

namespace aw {
namespace core {
namespace ai {

IAIServicePtr RegisterService(base::ServiceLocatorPtr serviceLocator) {

    if(!serviceLocator) {
        throw std::invalid_argument("Invalid service-locator to register AI service to.");
    }

    IAIServicePtr aiService = IAIServicePtr(new AIService());
    serviceLocator->RegisterService(IAIServicePtr(new AIService()));
    return aiService;
}

} // namespace ai
} // namespace core
} // namespace aw