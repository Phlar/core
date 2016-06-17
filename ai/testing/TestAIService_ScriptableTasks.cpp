#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIScriptingAction"

#include <boost/test/unit_test.hpp>

#include "Utils.hpp"
#include "InterfaceImpl.hpp"

#include "IBehaviorTree.hpp"
#include "IScriptAction.hpp"
#include "InitScriptingService.hpp"

#include "IScriptContext.hpp"
#include "IScriptResolver.hpp"
#include "IScriptingService.hpp"

#include "AIServiceFixture.hpp"

#include <boost/intrusive_ptr.hpp>

#include <turtle/mock.hpp>


#pragma warning(disable: 4512)
#include <boost/scope_exit.hpp>
#pragma warning(default: 4512)

namespace aw {
namespace core {
namespace ai {
namespace testing {

namespace {

// Helper ensuring a file exists - if not it will be created.
void ensureFileExistence(const boost::filesystem::path& filePath) {

    bool fileExists = false;
    BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(filePath));
    if(!fileExists) {

        std::ofstream outStream;
        BOOST_REQUIRE_NO_THROW(outStream.open(filePath.string()));
    }
}

// Helper ensuring a file is removed - if not it will be removed.
void ensureFileRemoval(const boost::filesystem::path& filePath) {

    bool fileExists = false;
    BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(filePath));
    if(fileExists) {
        BOOST_REQUIRE_NO_THROW(boost::filesystem::remove(filePath));
    }
}

MOCK_BASE_CLASS(MockScriptResolver, base::InterfaceImpl<scripting::IScriptResolver>) {

    public:

        MockScriptResolver() {
        }

        MOCK_METHOD(GetResolverID, 0);
        MOCK_METHOD(IsFileSupported, 1, bool(const boost::filesystem::path&));
        MOCK_METHOD(GetContextFromFile, 1, scripting::IScriptContextPtr(const boost::filesystem::path&));
        MOCK_METHOD(GetContextFromString, 1, scripting::IScriptContextPtr(const std::string&));
};
typedef boost::intrusive_ptr<MockScriptResolver> MockScriptResolverPtr;

MOCK_BASE_CLASS(MockScriptContext, base::InterfaceImpl<scripting::IScriptContext>) {

    public:

        MockScriptContext() {
        }

        MOCK_METHOD(ExecuteScript, 3);
};
typedef boost::intrusive_ptr<MockScriptContext> MockScriptContextPtr;

} // namespace anonymous

struct TestFixture : public AIServiceFixture {

    TestFixture() {

        BOOST_REQUIRE_NO_THROW(scripting::RegisterService(serviceLocator));

        // Check if service is available...
        base::IServicePtr service = base::IServicePtr();
        BOOST_REQUIRE_NO_THROW(service = serviceLocator->GetService(scripting::ID_SCRIPTING_SERVICE));
        BOOST_REQUIRE(service);

        // ...and the right one...
        scriptingService = boost::dynamic_pointer_cast<scripting::IScriptingService>(service);
        BOOST_REQUIRE(scriptingService);

        // Define where all script files are found.
        scriptFileDirectory = base::utils::GetProcessDirectory() / "../../testdata/";
        bool directoryExists = false;
        BOOST_REQUIRE_NO_THROW(directoryExists = boost::filesystem::exists(scriptFileDirectory));
        BOOST_REQUIRE(directoryExists);

        // 'Set up' a bogus file path.
        invalidScriptFilePath = scriptFileDirectory / "foo.bar";
        bool fileExists = false;
        BOOST_REQUIRE_NO_THROW(fileExists = boost::filesystem::exists(invalidScriptFilePath));
        BOOST_REQUIRE(!fileExists);

        // Some test cases require a behavior-tree to be run.
        BOOST_REQUIRE_NO_THROW(behaviorTree = aiService->createBehaviorTree());
        BOOST_REQUIRE(behaviorTree);
    }

    virtual ~TestFixture() {
    }


    boost::filesystem::path invalidScriptFilePath;
    boost::filesystem::path scriptFileDirectory;
    scripting::IScriptingServicePtr scriptingService;
    IBehaviorTreePtr behaviorTree; 
};


BOOST_FIXTURE_TEST_CASE(ScriptTaskWithInvalidFileAtConstructionShouldThrow, TestFixture) {

    // Try to create a runnable script task by specifying the file to load at construction.
    BOOST_CHECK_THROW(aiService->createScriptActionFromFile(invalidScriptFilePath, "foo", false), std::exception);

    // Set up a runnable script task by not defining to immediately load a file.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction());
    BOOST_REQUIRE(scriptAction);

    BOOST_CHECK_THROW(scriptAction->SetScriptFile(invalidScriptFilePath, "foo", false), std::exception);
}

BOOST_FIXTURE_TEST_CASE(ScriptTaskWithDelayedInvalidFileShouldThrow, TestFixture) {

    // Try to create a runnable script task by specifying the file to load at construction.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptActionFromFile(invalidScriptFilePath, "foo", true));
    BOOST_REQUIRE(scriptAction);
    BOOST_REQUIRE_NO_THROW(behaviorTree->SetRoot(scriptAction));

    BehaviorTreeState executionState = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(executionState = behaviorTree->ExecuteSync());
    BOOST_CHECK(executionState == BehaviorTreeState::STATE_FAILED);
}

BOOST_FIXTURE_TEST_CASE(SetScriptTaskWithDelayedInvalidFileShouldThrow, TestFixture) {

    // Create a script-task where the script file is not specified at construction.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction());
    BOOST_REQUIRE(scriptAction);
    BOOST_REQUIRE_NO_THROW(behaviorTree->SetRoot(scriptAction));

    // Now set the script to invoke.
    BOOST_CHECK_NO_THROW(scriptAction->SetScriptFile(invalidScriptFilePath, "foo", true));

    BehaviorTreeState executionState = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(executionState = behaviorTree->ExecuteSync());
    BOOST_CHECK(executionState == BehaviorTreeState::STATE_FAILED);
}

BOOST_FIXTURE_TEST_CASE(MissingScriptResolverShouldThrow, TestFixture) {

    // Set up a file no resolver is registered for.
    const boost::filesystem::path scriptFilePath = base::utils::GetProcessDirectory()  / "../../testdata/SomeScriptFile.foo";
    ensureFileExistence(scriptFilePath);

    // Ensure all files are removed in case of an error.
#pragma warning(disable: 4003 4512)
    BOOST_SCOPE_EXIT(&scriptFilePath) {

        ensureFileRemoval(scriptFilePath);

    } BOOST_SCOPE_EXIT_END
#pragma warning(default: 4003 4512)

    // Create a scripting-task.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_CHECK_THROW(scriptAction = aiService->createScriptActionFromFile(scriptFilePath, "foo", false), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestReturnOfValidScriptContext, TestFixture) {

    const boost::filesystem::path scriptFilePath = base::utils::GetProcessDirectory()  / "../../testdata/SomeScriptFile.foo";
    const std::string functionName = "FooBar";

    ensureFileExistence(scriptFilePath);

    // Ensure all files are removed in case of an error.
#pragma warning(disable: 4003 4512)
    BOOST_SCOPE_EXIT(&scriptFilePath) {

        ensureFileRemoval(scriptFilePath);

    } BOOST_SCOPE_EXIT_END
#pragma warning(default: 4003 4512)

    // Register a mocked script resolver returning a mocked script context.
    MockScriptResolverPtr mockResolver = MockScriptResolverPtr(new MockScriptResolver());
    MOCK_EXPECT(mockResolver->GetResolverID).returns(base::utils::CreateUUIDFromString("{11111111-AAAA-BBBB-CCCC-DDDDDDDDDDDD}"));

    MockScriptContextPtr mockContext = MockScriptContextPtr(new MockScriptContext());
    BOOST_REQUIRE_NO_THROW(scriptingService->AddResolver(mockResolver));

    MOCK_EXPECT(mockResolver->IsFileSupported).returns(true);
    MOCK_EXPECT(mockResolver->GetContextFromFile).calls([&scriptFilePath, &mockContext](const boost::filesystem::path& passedScriptFilePath) {

        BOOST_CHECK_EQUAL(passedScriptFilePath, scriptFilePath);
        return mockContext;
    });

    MOCK_EXPECT(mockContext->ExecuteScript).once().calls(
        [&functionName](const std::string& passedFunctionName, const scripting::ArgumentVector&, const scripting::ReturnValuesHolder& returnValues) {

            BOOST_CHECK_EQUAL(passedFunctionName, functionName);
            returnValues.SetValue(0, ITask::TaskResult::TASK_RESULT_PASSED);
    });

    // Set up the action and invoke it. Delayed load or immediate processing should not have any difference here.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptActionFromFile(scriptFilePath, functionName, true));
    BOOST_REQUIRE(scriptAction);

    BOOST_CHECK_NO_THROW(behaviorTree->SetRoot(scriptAction));
    BehaviorTreeState result = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(result = behaviorTree->ExecuteSync());
    BOOST_CHECK(result == BehaviorTreeState::STATE_FINISHED);
}

BOOST_FIXTURE_TEST_CASE(TestEmptyStringScriptShouldThrow, TestFixture) {

    const std::string scriptSource = "";
    const std::string functionName = "FooBar";
    const base::UUID  resolverID = base::utils::CreateUUIDFromString("{11111111-AAAA-BBBB-CCCC-DDDDDDDDDDDD}");

    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction());
    BOOST_REQUIRE(scriptAction);

    BOOST_CHECK_THROW(scriptAction->SetScriptString(scriptSource, resolverID, functionName, false), std::exception);
}

BOOST_FIXTURE_TEST_CASE(TestReturnOfValidScriptFromStringContext, TestFixture) {

    const std::string scriptSource = "Some script content.";
    const std::string functionName = "FooBar";
    const base::UUID resolverID = base::utils::CreateUUIDFromString("{11111111-AAAA-BBBB-CCCC-DDDDDDDDDDDD}");

    // Register a mocked script resolver returning a mocked script context.
    MockScriptResolverPtr mockResolver = MockScriptResolverPtr(new MockScriptResolver());
    MOCK_EXPECT(mockResolver->GetResolverID).returns(resolverID);

    MockScriptContextPtr mockContext = MockScriptContextPtr(new MockScriptContext());
    BOOST_REQUIRE_NO_THROW(scriptingService->AddResolver(mockResolver));

    MOCK_EXPECT(mockResolver->GetContextFromString).calls([&scriptSource, &mockContext](const std::string& passedScriptSource) {

        BOOST_CHECK_EQUAL(passedScriptSource, scriptSource);
        return mockContext;
    });

    MOCK_EXPECT(mockContext->ExecuteScript).once().calls(
        [&functionName](const std::string& passedFunctionName, const scripting::ArgumentVector&, const scripting::ReturnValuesHolder& returnValues) {

            BOOST_CHECK_EQUAL(passedFunctionName, functionName);
            returnValues.SetValue(0, ITask::TaskResult::TASK_RESULT_PASSED);
    });

    // Set up the action and invoke it.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptActionFromString(scriptSource, resolverID, functionName, true));
    BOOST_REQUIRE(scriptAction);

    BOOST_CHECK_NO_THROW(behaviorTree->SetRoot(scriptAction));
    BehaviorTreeState result = BehaviorTreeState::STATE_NOT_RUN;
    BOOST_CHECK_NO_THROW(result = behaviorTree->ExecuteSync());
    BOOST_CHECK(result == BehaviorTreeState::STATE_FINISHED);
}


} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw