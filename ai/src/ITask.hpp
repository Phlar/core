#pragma once

namespace aw {
namespace core {
namespace ai {


enum class TaskResult{

    TASK_RESULT_FAILED = 0,
    TASK_RESULT_PASSED
};

class ITask {

    public:
        
        virtual TaskResult evaluate() = 0;
};

} // namespace ai
} // namespace core
} // namespace aw