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

        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType& yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw