#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIScriptingAction"

#include <boost/test/unit_test.hpp>

#include "Utils.hpp"

#include "IBehaviorTree.hpp"
#include "IScriptAction.hpp"
#include "InitScriptingService.hpp"

#include "IScriptingService.hpp"

#include "AIServiceFixture.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

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
        BOOST_TEST_MESSAGE("teardown");
    }


    boost::filesystem::path invalidScriptFilePath;
    boost::filesystem::path scriptFileDirectory;
    scripting::IScriptingServicePtr scriptingService;
    IBehaviorTreePtr behaviorTree; 
};

// Todo: Write tests for:
//          Unresolvable file, i.e. no resolver registered to the ScriptingService.
//          Delaved / immediate resolving of file.



BOOST_FIXTURE_TEST_CASE(ScriptTaskWithInvalidFileAtConstructionShouldThrow, TestFixture) {

    // Try to create a runnable script task by specifying the file to load at construction.
    BOOST_CHECK_THROW(aiService->createScriptAction(invalidScriptFilePath, "foo", false), std::exception);

    // Set up a runnable script task by not defining to immediately load a file.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction());
    BOOST_REQUIRE(scriptAction);

    BOOST_CHECK_THROW(scriptAction->SetScriptFile(invalidScriptFilePath, "foo", false), std::exception);
}

BOOST_FIXTURE_TEST_CASE(ScriptTaskWithDelayedInvalidFileShouldThrow, TestFixture) {

    // Try to create a runnable script task by specifying the file to load at construction.
    IScriptActionPtr scriptAction = nullptr;
    BOOST_REQUIRE_NO_THROW(scriptAction = aiService->createScriptAction(invalidScriptFilePath, "foo", true));
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


} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw