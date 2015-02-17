#pragma once

#include "ITask.hpp"

#include <boost/function.hpp>

namespace aw {
namespace core {
namespace ai {

typedef boost::function<TaskResult()> ActionFnc;

class IAction : public ITask {

    public:

        virtual void SetAction(const ActionFnc& action);
};

} // namespace ai
} // namespace core
} // namespace aw