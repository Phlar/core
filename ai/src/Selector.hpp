#pragma once

#include "AIFwdDeclarations.hpp"
#include "ISelector.hpp"

#include "TaskContainer.hpp"
#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {

class Selector : public base::InterfaceImpl<ISelector>, public impl::TaskContainer {

    public:

        Selector();
        virtual ~Selector();

    protected:

        TaskResult evaluate(IBlackboardPtr blackboard) const override;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw