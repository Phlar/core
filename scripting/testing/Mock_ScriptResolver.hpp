#pragma once

#include "IScriptContext.hpp"
#include "InterfaceImpl.hpp"

#include <turtle/mock.hpp>


namespace aw {
namespace core {
namespace scripting {
namespace testing {

class TestScriptResolver : public base::InterfaceImpl<IScriptResolver> {};

MOCK_BASE_CLASS(MockScriptResolver, TestScriptResolver) {

    public:

        MOCK_METHOD(GetResolverID, 0);
        MOCK_METHOD(IsFileSupported, 1);
        MOCK_METHOD(GetContext, 1);
};
typedef boost::intrusive_ptr<MockScriptResolver> MockScriptResolverPtr;

} // namespace testing	
} // namespace scripting	
} // namespace core	
} // namespace aw