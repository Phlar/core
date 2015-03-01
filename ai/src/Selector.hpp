#pragma once

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

    TaskResult evaluate();
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw