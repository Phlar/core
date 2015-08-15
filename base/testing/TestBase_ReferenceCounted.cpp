#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test_ReferenceCounted"

#include <IReferenceCounted.hpp>
#include <ReferenceCounted.hpp>
#include <InterfaceImpl.hpp>

#include <boost/intrusive_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/noncopyable.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <vector>

namespace coro = boost::coroutines;

#pragma warning (disable: 4250)

enum TaskResult {

    TASK_RUNNING = -1,
    TASK_FAILED,
    TASK_SUCCESS
};

enum TreeState {

    TREE_NOT_RUN = -1,
    TREE_RUNNING,
    TREE_FINISHED
};

class Task {

    public:

        virtual TaskResult evaluate(coro::coroutine<void>::push_type& yield) = 0;

};
typedef boost::shared_ptr<Task> TaskPtr;

class TaskContainer : public Task {

    public:

        std::vector<TaskPtr> m_children;
};
typedef boost::shared_ptr<TaskContainer> TaskContainerPtr;

class TaskSelector : public TaskContainer {

    public:

        virtual TaskResult evaluate(coro::coroutine<void>::push_type& yield) {

            std::cout << "[Sel] 'yield' at selector" << std::endl;
            yield();

            for(size_t childIndex = 0; childIndex < m_children.size(); ++childIndex) {

                std::cout << "[Sel] evaluating child at index " << childIndex << std::endl;

                if(m_children[childIndex]->evaluate(yield) == TASK_SUCCESS) {
                    return TASK_SUCCESS;
                }
            }

            return TASK_FAILED;
        }
};
typedef boost::shared_ptr<TaskSelector> TaskSelectorPtr;

class Condition : public Task {

    public:

        typedef boost::function<TaskResult()> ConditionFnc;

        Condition(const ConditionFnc& conditionFnc) 
        : m_condition(conditionFnc) {
        }

        virtual TaskResult evaluate(coro::coroutine<void>::push_type& yield) {

            std::cout << "[Cond] 'yield' at condition" << std::endl;
            yield();
            return m_condition();
        }

    TaskResult m_expectedResult;
    ConditionFnc m_condition;
};
typedef boost::shared_ptr<Condition> ConditionPtr;

class BehaviourTree {

    public:

        BehaviourTree(TaskPtr root)
        : m_root(root)
        , m_treeState(TREE_NOT_RUN)
        {}

        TreeState evaluateTick() {

            std::cout << "[BT] state before tick: " << m_treeState << std::endl;

            if(!m_treeCoroutine) {
                m_treeCoroutine = CoroUPtr(new coro::coroutine<void>::pull_type(boost::bind(&BehaviourTree::evaluate, this, _1)));
            }
            else {
                (*m_treeCoroutine)();
            }

            if(*m_treeCoroutine) {

                m_treeState = TREE_RUNNING;
            } else {

                m_treeCoroutine.reset(nullptr);
                m_treeState = TREE_FINISHED;
            }

            std::cout << "[BT] state after tick: " << m_treeState << std::endl;
            return m_treeState;
        }

        TreeState state() const {

            return m_treeState;
        }

    protected:

        void evaluate(coro::coroutine<void>::push_type& yield) {

            std::cout << "[BT] starting evaluating" << std::endl;
            m_root->evaluate(yield);
            std::cout << "[BT] finished evaluating" << std::endl;
        }

        typedef std::unique_ptr<coro::coroutine<void>::pull_type> CoroUPtr;
        CoroUPtr m_treeCoroutine;
        TreeState m_treeState;
        TaskPtr m_root;

};

TaskResult failingConditionFunc() {

    std::cout << "[F] flag failing" << std::endl;
    return TASK_FAILED;
}

TaskResult succeedingConditionFunc() {

    std::cout << "[F] flag success" << std::endl;
    return TASK_SUCCESS;
}


BOOST_AUTO_TEST_CASE(TestCoroBT) {

    TaskSelectorPtr selectorA(new TaskSelector());
    TaskSelectorPtr selectorB(new TaskSelector());

    ConditionPtr succeedingCondition(new Condition(succeedingConditionFunc));
    ConditionPtr failingCondition(new Condition(failingConditionFunc));

    selectorA->m_children.push_back(failingCondition);
    selectorA->m_children.push_back(selectorB);

    selectorB->m_children.push_back(failingCondition);
    selectorB->m_children.push_back(failingCondition);
    selectorB->m_children.push_back(succeedingCondition);

    BehaviourTree bt(selectorA);

    int numTicks = 1;
    while(bt.state() != TREE_FINISHED) 
    {
        std::cout << std::string(40, '#') << std::endl << "[Root] next tick (" << numTicks++ << ")" << std::endl;
        bt.evaluateTick();
    }

}


// namespace aw {
// namespace core {
// namespace base {
// namespace testing {

void secondSimpleFunction() {

    int result = 0;
    //std::cout << std::string(8, ' ') << __FUNCTION__ << " Entering - value at " << result++ << std::endl;

    for(int i=0; i<10; ++i) {

        result++;

        //std::cout << std::string(8, ' ') << __FUNCTION__ << " Before 'yield' - value at " << result++ << std::endl;
        //std::cout << std::string(8, ' ') << __FUNCTION__ << " After 'yield'" << std::endl;
    }
    //std::cout << std::string(8, ' ') << __FUNCTION__ << " Exiting" << std::endl;
}

void firstSimpleFunction() {

    int result = 0;

    // std::cout << std::string(4, ' ') << __FUNCTION__ << " Entering - value at " << result++ << std::endl;

    for(int i=0; i<1000; ++i) {

        result++;

        //std::cout << std::string(4, ' ') << __FUNCTION__ << " Before calling child coroutine - value at " << result++ << std::endl;
        secondSimpleFunction();

        //std::cout << std::string(4, ' ') << __FUNCTION__ << " Before 'yield'" << std::endl;
        //std::cout << std::string(4, ' ') << __FUNCTION__ << " After 'yield'" << std::endl;
    }

    //std::cout << std::string(4, ' ') << __FUNCTION__ << " Exiting coroutine" << std::endl;
}




int secondCoroutine(coro::coroutine<void>::push_type& yield) {

    int result = 0;

    std::cout << std::string(8, ' ') << __FUNCTION__ << " Entering - value at " << result++ << std::endl;

    for(int i=0; i<2; ++i) {

        std::cout << std::string(8, ' ') << __FUNCTION__ << " Before 'yield' - value at " << result++ << std::endl;
        yield();
        std::cout << std::string(8, ' ') << __FUNCTION__ << " After 'yield'" << std::endl;
    }

   std::cout << std::string(8, ' ') << __FUNCTION__ << " Exiting" << std::endl;

   return result * 1000;
}


void firstCoroutine(coro::coroutine<void>::push_type& yield) {

    int result = 0;

    std::cout << std::string(4, ' ') << __FUNCTION__ << " Entering - value at " << result++ << std::endl;

    for(int i=0; i<3; ++i) {

        //std::cout << "    first: " << i << std::endl;

        result++;
        std::cout << std::string(4, ' ') << __FUNCTION__ << " Before calling child coroutine - value at " << result++ << std::endl;
        int secondResult = secondCoroutine(yield);

        std::cout << __FUNCTION__ << " result from nested call: " << secondResult << std::endl;

        std::cout << std::string(4, ' ') << __FUNCTION__ << " Before 'yield'" << std::endl;
        yield();
        std::cout << std::string(4, ' ') << __FUNCTION__ << " After 'yield'" << std::endl;
    }

    std::cout << std::string(4, ' ') << __FUNCTION__ << " Exiting coroutine" << std::endl;
}


/*
BOOST_AUTO_TEST_CASE(TestCoroutines) {

    coro::coroutine<void>::pull_type topLevelCoroutine(
        [](coro::coroutine<void>::push_type& yield) {
            firstCoroutine(yield);
        });


    boost::posix_time::ptime t1(boost::posix_time::microsec_clock::local_time());

    while(topLevelCoroutine) {

        std::cout << __FUNCTION__ << " Before calling top level coroutine" << std::endl;
        topLevelCoroutine();
        std::cout << __FUNCTION__ << " After calling top level coroutine" << std::endl;
        
    }

    /*
    std::cout << "root: " << result++ << std::endl;

    //firstSimpleFunction();
    
    boost::posix_time::ptime t2(boost::posix_time::microsec_clock::local_time());
    boost::posix_time::time_duration dt = t2 - t1;
    std::cout << "Elapsed time: " << dt.total_milliseconds() << std::endl;
    
    std::cout << "adsg";
}
*/
// 

// } // namespace testing
// } // namespace base
// } // namespace core
// } // namespace aw

#pragma warning (default: 4250)