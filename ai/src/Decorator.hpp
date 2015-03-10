#pragma once

#include "IDecorator.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


class Decorator : public base::InterfaceImpl<IDecorator> {

    public:

        virtual ~Decorator();

        void SetTask(ITaskPtr task);
        TaskResult evaluate();

    protected:

        Decorator();

        virtual void preEvaluate();
        virtual void postEvaluate(TaskResult& result);

    private:

       ITaskPtr m_task;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw