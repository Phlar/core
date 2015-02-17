#pragma once

#include "ITask.hpp"

#include <boost/function.hpp>

namespace aw {
namespace core {
namespace ai {


typedef boost::function<TaskResult()> ConditionFnc;

class ICondition : public ITask {

    public:

        virtual void SetCondition(const ConditionFnc& condition) = 0;        
};

} // namespace ai
} // namespace core
} // namespace aw