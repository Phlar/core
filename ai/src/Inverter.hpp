#pragma once

#include "AIFwdDeclarations.hpp"
#include "IInverter.hpp"

#include "Decorator.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Inverter : public base::InterfaceImpl<IInverter>, public impl::Decorator {

    public:
        Inverter();
        virtual ~Inverter();

    protected:

        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw