 #define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_LUABasics"

#include "Utils.hpp"
#include "LUAScriptResolver.hpp"

#include <luabind/luabind.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <turtle/mock.hpp>

#include <iostream>
#include <vector>

namespace aw {
namespace core {
namespace scripting {
namespace lua {
namespace testing {

namespace {

class TestClass {

    public:

        virtual void FunctionNoParamA() = 0;
        virtual void FunctionNoParamB() = 0;
};

MOCK_BASE_CLASS(MockTestClass, TestClass) {

    public:
        MOCK_METHOD(FunctionNoParamA, 0);
        MOCK_METHOD(FunctionNoParamB, 0);
};

} // namespace anonymous


// Functions / mock objects used for LUA tests checking free function invocation.
std::unique_ptr<MockTestClass> freeFunctionsTestClass;

void FreeFunctionNoParamA() {
    freeFunctionsTestClass->FunctionNoParamA();
}

void FreeFunctionNoParamB() {
    freeFunctionsTestClass->FunctionNoParamB();
}


struct LUATestFixture {

    LUATestFixture()
    : luaResolver()
    , luaTestFilePath(base::utils::GetProcessDirectory() / "../../testdata/TestFunctions.lua") {

        std::cout << aw::core::base::utils::GetProcessDirectory() << std::endl;

        freeFunctionsTestClass = std::unique_ptr<MockTestClass>(new MockTestClass());
        // MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamA).once();
        // freeFunctionsTestClass->FunctionNoParamA();
    }

    IScriptContextPtr getCheckedContext(const boost::filesystem::path& scriptPath) {

        IScriptContextPtr ctx;
        BOOST_REQUIRE_NO_THROW(ctx = luaResolver.GetContext(scriptPath));
        BOOST_REQUIRE(ctx);
        return ctx;
    }

    

    luabind::scope registerFreeFunctionA() {
        return luabind::def("FreeFunctionNoParamA", &FreeFunctionNoParamA);
    }

    luabind::scope registerFreeFunctionB() {
        return luabind::def("FreeFunctionNoParamB", &FreeFunctionNoParamB);
    }


    LUAScriptResolver luaResolver;
    boost::filesystem::path luaTestFilePath;

};

BOOST_FIXTURE_TEST_CASE(TestLUASimpleFunctionWithoutParameters, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector noArgs;
    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncNoParam", noArgs));
}

BOOST_FIXTURE_TEST_CASE(TestLUAUnknownFunctionShouldThrow, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector noArgs;
    BOOST_CHECK_THROW(ctx->ExecuteScript("GarbageFunction", noArgs), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(TestAddingRegistrationFunctions, LUATestFixture) {

    BOOST_CHECK_NO_THROW(luaResolver.AddRegistrationFunction(RegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionA, this))));
}

BOOST_FIXTURE_TEST_CASE(TestCallingSingleFreeFunctionFromLUA, LUATestFixture) {

    BOOST_CHECK_NO_THROW(luaResolver.AddRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionA, this)));
    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    
    // Expect one call back to C++ code.
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamA).once();

    ArgumentVector noArgs;
    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallSingleFreeFunction", noArgs));
}


BOOST_FIXTURE_TEST_CASE(TestCallingTwoFreeFunctionsFromLUA, LUATestFixture) {

    // This test ensures that split up registration works, i.e. two free functions get registered
    // in two different steps.
    BOOST_CHECK_NO_THROW(luaResolver.AddRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionA, this)));
    BOOST_CHECK_NO_THROW(luaResolver.AddRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionB, this)));
    
    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);

    // Expect two calls back to C++ code.
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamA).once();
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamB).once();

    ArgumentVector noArgs;
    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallTwoFreeFunctions", noArgs));
}

BOOST_FIXTURE_TEST_CASE(TestCallingFunctionWithSimpleParameters, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector twoSimpleArgs;

    twoSimpleArgs.push_back(Argument(static_cast<int16_t>(2)));
    twoSimpleArgs.push_back(Argument(static_cast<int16_t>(5)));

    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallTwoSimpleParameters", twoSimpleArgs));
}


} // namespace testing
} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw