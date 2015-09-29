#pragma once

#include "IScriptContext.hpp"
#include "InterfaceImpl.hpp"

#include <turtle/mock.hpp>


namespace aw {
namespace core {
namespace scriptingservice {
namespace testing {

class TestScriptContext : public base::InterfaceImpl<IScriptContext> {};

MOCK_BASE_CLASS(MockScriptContext, TestScriptContext) {

    public:

        MOCK_METHOD(ExecuteScript, 2);
};
typedef boost::intrusive_ptr<MockScriptContext> MockScriptContextPtr;

} // namespace testing	
} // namespace scriptingservice	
} // namespace core	
} // namespace aw