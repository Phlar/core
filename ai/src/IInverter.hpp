#pragma once

#include "IDecorator.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {

//! \brief Decorator that just inverts the returned state of the affected task.
//! \note TaskResult::TASK_RESULT_FAILED and TaskResult::TASK_RESULT_PASSE
//! are inverted to each other. Any other state is returned as is.
class IInverter : public virtual IDecorator {
};

typedef boost::intrusive_ptr<IInverter> IInverterPtr;

} // namespace ai
} // namespace core
} // namespace aw