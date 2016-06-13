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

        void SetRepeatCondition(RepeatConditionUPtr repeatCondition) override;

    protected:

        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;

        RepeatConditionUPtr m_repeatCondition;
};

typedef boost::intrusive_ptr<Repeater> RepeaterPtr;

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw