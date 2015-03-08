#pragma once

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

        TaskResult evaluate();
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw