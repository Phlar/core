#pragma once

#include "AIFwdDeclarations.hpp"
#include "IRepeater.hpp"

#include "Decorator.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Repeater : public base::InterfaceImpl<IRepeater>, public impl::Decorator {

    public:

        Repeater();
        virtual ~Repeater();

        void SetRepeatCondition(const RepeatCondition& repeatCondition) override;

    protected:

        TaskResult evaluate(IBlackboardPtr blackboard) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw