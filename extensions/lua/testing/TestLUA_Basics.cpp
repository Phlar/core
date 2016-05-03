#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_LUABasics"

#include "Utils.hpp"
#include "LUAScriptResolver.hpp"
#include "LUATypeHelpers.hpp"

#include <ReferenceCounted.hpp>
#include <InterfaceImpl.hpp>

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


// As it doesn't seem to be possible mocking free functions with
// turtle-mock, an instance of the following class / derived mock-class 
// is kept globally and triggered from the tested free functions.
class FreeFunctionHelperTestClass {

    public:

        virtual void FunctionNoParamA() = 0;
        virtual void FunctionNoParamB() = 0;
};

MOCK_BASE_CLASS(MockFreeFunctionHelperTestClass, FreeFunctionHelperTestClass) {

    public:
        MOCK_METHOD(FunctionNoParamA, 0);
        MOCK_METHOD(FunctionNoParamB, 0);
};
// Functions / mock objects used for LUA tests checking free function invocation.
std::unique_ptr<MockFreeFunctionHelperTestClass> freeFunctionsTestClass;

void FreeFunctionNoParamA() {
    freeFunctionsTestClass->FunctionNoParamA();
}

void FreeFunctionNoParamB() {
    freeFunctionsTestClass->FunctionNoParamB();
}



// Class / mock class to be used for testing passing user defined data
// to LUA. Moreover wrapping that classes into intrusive_ptr should be tested.
class IMemberFunctionHelperTestClass;
typedef boost::intrusive_ptr<IMemberFunctionHelperTestClass> IMemberFunctionHelperTestClassPtr;

class IMemberFunctionHelperTestClass : public virtual base::IReferenceCounted {

    public:

        virtual ~IMemberFunctionHelperTestClass() {}

        virtual void MemberFunctionNoParam() = 0;
        virtual void MemberFunction(IMemberFunctionHelperTestClassPtr) = 0;

        virtual int GetReferenceCount() const = 0;
};

class MemberFunctionHelperTestClass : public base::InterfaceImpl<IMemberFunctionHelperTestClass> {

    public:

        MemberFunctionHelperTestClass() {}
        virtual ~MemberFunctionHelperTestClass() {
        }

        virtual void MemberFunctionNoParam() {};
        virtual void MemberFunction(IMemberFunctionHelperTestClassPtr) {};

        virtual int GetReferenceCount() const {
            return ReferenceCounted::GetRefCount();
        }
};
typedef boost::intrusive_ptr<MemberFunctionHelperTestClass> MemberFunctionHelperTestClassPtr;


MOCK_BASE_CLASS(MockMemberFunctionHelperTestClass, MemberFunctionHelperTestClass) {

    // Expecting a destructor call requires a workaround of using a static member function:
    // http://sourceforge.net/p/turtle/tickets/21/
    MOCK_STATIC_METHOD(destructor, 0, void(), destructor)
        ~MockMemberFunctionHelperTestClass() {
            try {
                destructor();
            } catch(...) {}
        }

    MOCK_METHOD(MemberFunctionNoParam, 0);
    MOCK_METHOD(MemberFunction, 1, void(IMemberFunctionHelperTestClassPtr));
};
typedef boost::intrusive_ptr<MockMemberFunctionHelperTestClass> MockMemberFunctionHelperTestClassPtr;

} // namespace anonymous


struct LUATestFixture {

    LUATestFixture()
    : luaResolver()
    , luaTestFilePath(base::utils::GetProcessDirectory() / "../../testdata/TestFunctions.lua") {

        freeFunctionsTestClass = std::unique_ptr<MockFreeFunctionHelperTestClass>(new MockFreeFunctionHelperTestClass());
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
    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncNoParam", ArgumentVector(), ReturnValuesHolder::Create()));
}

BOOST_FIXTURE_TEST_CASE(TestLUAUnknownFunctionShouldThrow, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    BOOST_CHECK_THROW(ctx->ExecuteScript("GarbageFunction", ArgumentVector(), ReturnValuesHolder::Create()), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(TestAddingTypeRegistrationFunctions, LUATestFixture) {

    BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(
        TypeRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionA, this))));
}

BOOST_FIXTURE_TEST_CASE(TestCallingSingleFreeFunctionFromLUA, LUATestFixture) {

    BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(
        boost::bind(&LUATestFixture::registerFreeFunctionA, this)));
    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    
    // Expect one call back to C++ code.
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamA).once();

    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallSingleFreeFunction", ArgumentVector(), ReturnValuesHolder::Create()));
}


BOOST_FIXTURE_TEST_CASE(TestCallingTwoFreeFunctionsFromLUA, LUATestFixture) {

    // This test ensures that split up registration works, i.e. two free functions get registered
    // in two different steps.
    BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionA, this)));
    BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(boost::bind(&LUATestFixture::registerFreeFunctionB, this)));
    
    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);

    // Expect two calls back to C++ code.
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamA).once();
    MOCK_EXPECT(freeFunctionsTestClass->FunctionNoParamB).once();

    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallTwoFreeFunctions", ArgumentVector(), ReturnValuesHolder::Create()));
}

BOOST_FIXTURE_TEST_CASE(TestCallingFreeFunctionWithUnresolverParametersShouldThrow, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    args.push_back(Argument(static_cast<int16_t>(2)));
    args.push_back(Argument(static_cast<int16_t>(5)));
    
    BOOST_CHECK_THROW(ctx->ExecuteScript("FuncCallTwoSimpleParameters", args, ReturnValuesHolder::Create()), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(TestCallingFreeFunctionWithSimpleParameters, LUATestFixture) {

    luaResolver.RegisterPushTypeToLUAFunction(typeid(int16_t), pushToLUAStack<int16_t>);

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    args.push_back(Argument(static_cast<int16_t>(2)));
    args.push_back(Argument(static_cast<int16_t>(5)));

    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallTwoSimpleParameters", args, ReturnValuesHolder::Create()));
}

BOOST_FIXTURE_TEST_CASE(TestCallingFreeFunctionPushTypeRegistrationAfterContextRetrieval, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    // Register the 'push'-function after creating the context - this should not harm at all.
    luaResolver.RegisterPushTypeToLUAFunction(typeid(int16_t), pushToLUAStack<int16_t>);

    args.push_back(Argument(static_cast<int16_t>(2)));
    args.push_back(Argument(static_cast<int16_t>(5)));

    BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallTwoSimpleParameters", args, ReturnValuesHolder::Create()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: This test is broken as it does not prove destruction of the class instance properly. The instance
// should be desctroyed without having to release the entire context!!!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE(TestCallingBackMemberFunctionNoParams, LUATestFixture) {

    // Test calling back a C++ member function from LUA.
    // Verify no data is kept by LUA after execution.

    auto registerClass = []() -> luabind::scope {
        return luabind::class_<IMemberFunctionHelperTestClass, IMemberFunctionHelperTestClassPtr>("IMemberFunctionHelperTestClass")
            .def("MemberFunctionNoParam", &IMemberFunctionHelperTestClass::MemberFunctionNoParam);
    };

    {
        BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(registerClass));
        IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
        ArgumentVector args;

        // Register the intrusive-pointer wrapped interface.
        luaResolver.RegisterPushTypeToLUAFunction(typeid(IMemberFunctionHelperTestClassPtr), pushToLUAStack<IMemberFunctionHelperTestClassPtr>);

        // Create an instance to be passed back to LUA.
        MockMemberFunctionHelperTestClassPtr mockClass(new MockMemberFunctionHelperTestClass());
        args.push_back(Argument(IMemberFunctionHelperTestClassPtr(mockClass)));

        // Expectation is one call back to C++ as well as the destruction of the object
        // i.e. nothing is kept by LUA any longer.
        MOCK_EXPECT(mockClass->MemberFunctionNoParam).once();
        MOCK_EXPECT(MockMemberFunctionHelperTestClass::destructor).once();

        BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallMethodOfCustomClassNoParam", args, ReturnValuesHolder::Create()));
    }

    // Verify all expectations towards Turtle.
    mock::verify();
}

BOOST_FIXTURE_TEST_CASE(TestCallingBackMemberFunction, LUATestFixture) {

    auto registerClass = []() -> luabind::scope {
        return luabind::class_<IMemberFunctionHelperTestClass, IMemberFunctionHelperTestClassPtr>("IMemberFunctionHelperTestClass")
            .def("MemberFunction", &IMemberFunctionHelperTestClass::MemberFunction);
    };

    {
        BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(registerClass));
        IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
        ArgumentVector args;

        luaResolver.RegisterPushTypeToLUAFunction(typeid(IMemberFunctionHelperTestClassPtr), pushToLUAStack<IMemberFunctionHelperTestClassPtr>);

        MockMemberFunctionHelperTestClassPtr mockClass(new MockMemberFunctionHelperTestClass());
        args.push_back(Argument(IMemberFunctionHelperTestClassPtr(mockClass)));

        MOCK_EXPECT(mockClass->MemberFunction).once().calls(
            [&mockClass](IMemberFunctionHelperTestClassPtr fromLUA) {

                // Check whether the LUA passes back the correct 'instance'.
                BOOST_CHECK_EQUAL(fromLUA, mockClass);
            });
        MOCK_EXPECT(MockMemberFunctionHelperTestClass::destructor).once();

        BOOST_CHECK_NO_THROW(ctx->ExecuteScript("FuncCallMethodOfCustomClass", args, ReturnValuesHolder::Create()));
    }

    // Verify all expectations towards Turtle.
    mock::verify();
}

BOOST_FIXTURE_TEST_CASE(TestReturnBuiltinValuesFromLUA, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    // Register converter functions.
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(bool), popFromLUAStack<bool>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(std::string), popFromLUAStack<std::string>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(int16_t), popFromLUAStack<int16_t>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(float), popFromLUAStack<float>));


    // Check a single boolean.
    ReturnValuesHolder returnedBool = ReturnValuesHolder::Create<bool>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnBool", args, returnedBool));
    bool resultBool = false;
    BOOST_REQUIRE_NO_THROW(resultBool = returnedBool.GetTypedValue<bool>(0));
    BOOST_CHECK_EQUAL(resultBool, true);

    // Check a sample string.
    ReturnValuesHolder returnedString = ReturnValuesHolder::Create<std::string>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnString", args, returnedString));
    std::string resultString = "";
    BOOST_REQUIRE_NO_THROW(resultString = returnedString.GetTypedValue<std::string>(0));
    BOOST_CHECK_EQUAL(resultString, "foo");

    // Check a number.
    ReturnValuesHolder returnedInt = ReturnValuesHolder::Create<int16_t>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnInt", args, returnedInt));
    int16_t resultInt = 0;
    BOOST_REQUIRE_NO_THROW(resultInt = returnedInt.GetTypedValue<int16_t>(0));
    BOOST_CHECK_EQUAL(resultInt, 42);

    // Check a floating point number.
    ReturnValuesHolder returnedFloat = ReturnValuesHolder::Create<float>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnFloat", args, returnedFloat));
    float resultFloat = 0.0f;
    BOOST_REQUIRE_NO_THROW(resultFloat = returnedFloat.GetTypedValue<float>(0));
    BOOST_CHECK_CLOSE(resultFloat, 123.456, 0.001);
}

BOOST_FIXTURE_TEST_CASE(TestReturnMultipleValuesFromLUA, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    // Register converter functions.
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(bool), popFromLUAStack<bool>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(std::string), popFromLUAStack<std::string>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(int16_t), popFromLUAStack<int16_t>));

    ReturnValuesHolder returnValues = ReturnValuesHolder::Create<bool, std::string, int16_t>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnBoolStringInt", args, returnValues));

    bool resultBool = false;
    std::string resultString = "";
    float resultInt = 0;

    BOOST_REQUIRE_NO_THROW(resultBool = returnValues.GetTypedValue<bool>(0));
    BOOST_CHECK_EQUAL(resultBool, true);

    BOOST_REQUIRE_NO_THROW(resultString = returnValues.GetTypedValue<std::string>(1));
    BOOST_CHECK_EQUAL(resultString, "foobar");

    BOOST_REQUIRE_NO_THROW(resultInt = returnValues.GetTypedValue<int16_t>(2));
    BOOST_CHECK_EQUAL(resultInt, 42);
}

BOOST_FIXTURE_TEST_CASE(TestReturnCountMismatchShouldThrow, LUATestFixture) {

    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(bool), popFromLUAStack<bool>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(std::string), popFromLUAStack<std::string>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(int16_t), popFromLUAStack<int16_t>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(float), popFromLUAStack<float>));

    ReturnValuesHolder tooFewValuesHolder = ReturnValuesHolder::Create<bool>();
    BOOST_CHECK_THROW(ctx->ExecuteScript("FuncReturnBoolStringInt", args, tooFewValuesHolder), std::runtime_error);

    ReturnValuesHolder tooManyValuesHolder = ReturnValuesHolder::Create<bool, std::string, int16_t, float>();
    BOOST_CHECK_THROW(ctx->ExecuteScript("FuncReturnBoolStringInt", args, tooManyValuesHolder), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(TestReturnCustomClassInstance, LUATestFixture) {

    auto registerClass = []() -> luabind::scope {
        return luabind::class_<IMemberFunctionHelperTestClass, IMemberFunctionHelperTestClassPtr>("IMemberFunctionHelperTestClass")
            .def("MemberFunction", &IMemberFunctionHelperTestClass::MemberFunction)
            .def("GetReferenceCount", &IMemberFunctionHelperTestClass::GetReferenceCount);
    };

    BOOST_CHECK_NO_THROW(luaResolver.AddTypeRegistrationFunction(registerClass));
    
    IScriptContextPtr ctx = getCheckedContext(luaTestFilePath);
    ArgumentVector args;

    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterPushTypeToLUAFunction(typeid(IMemberFunctionHelperTestClassPtr), pushToLUAStack<IMemberFunctionHelperTestClassPtr>));
    BOOST_REQUIRE_NO_THROW(luaResolver.RegisterFetchTypeFromLUAFunction(typeid(IMemberFunctionHelperTestClassPtr), popFromLUAStack<IMemberFunctionHelperTestClassPtr>));

    // The instance the test is all about...
    IMemberFunctionHelperTestClassPtr classInstanceToLUA(new MemberFunctionHelperTestClass());
    args.push_back(classInstanceToLUA);

    ReturnValuesHolder returnValues = ReturnValuesHolder::Create<IMemberFunctionHelperTestClassPtr>();
    BOOST_REQUIRE_NO_THROW(ctx->ExecuteScript("FuncReturnInAsOut", args, returnValues));

    // ...get it back from LUA
    IMemberFunctionHelperTestClassPtr classInstanceFromLUA;
    BOOST_REQUIRE_NO_THROW(classInstanceFromLUA = returnValues.GetTypedValue<IMemberFunctionHelperTestClassPtr>(0));

    BOOST_CHECK_EQUAL(classInstanceFromLUA, classInstanceToLUA);
}


// Todo:
//        - Negative test case for missing converters back from LUA.
//        - Exception handling in callback (C++) from within LUA. (C++ -> LUA -> C++ throwing exception)


} // namespace testing
} // namespace lua
} // namespace scripting
} // namespace core
} // namespace aw