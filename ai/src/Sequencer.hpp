#pragma once

#include "AIFwdDeclarations.hpp"
#include "ISequencer.hpp"

#include "TaskContainer.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Sequencer : public base::InterfaceImpl<ISequencer>, public impl::TaskContainer {

    public:

        Sequencer();
        virtual ~Sequencer();

    protected:

        ITask::TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePullType* yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw