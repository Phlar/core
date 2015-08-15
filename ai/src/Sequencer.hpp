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

        TaskResult evaluate(IBlackboardPtr blackboard, TaskCoroutinePushType& yield) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw