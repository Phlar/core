#pragma once

#include "IScriptContext.hpp"
#include "InterfaceImpl.hpp"

#include <turtle/mock.hpp>


namespace aw {
namespace core {
namespace scriptingservice {
namespace testing {

class TestScriptResolver : public base::InterfaceImpl<IScriptResolver> {};

MOCK_BASE_CLASS(MockScriptResolver, TestScriptResolver) {

    public:

        MOCK_METHOD(IsFileSupported, 1);
        MOCK_METHOD(GetContext, 1);
};
typedef boost::intrusive_ptr<MockScriptResolver> MockScriptResolverPtr;

} // namespace testing	
} // namespace scriptingservice	
} // namespace core	
} // namespace aw