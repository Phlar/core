#pragma once

namespace aw {
namespace core {
namespace ai {

class ITask {

    enum class TaskResult{

        TASK_RESULT_FAILED = 0,
        TASK_RESULT_PASSED
    };

    public:
        
        virtual TaskResult evaluate() = 0;
};

} // namespace ai
} // namespace core
} // namespace aw