#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "InterfaceImpl.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask, base::ReferenceCounted> {

    public:
        virtual ~Task() {
        };
        
    protected:

        // A task itself cannot be constructed as such
        Task() {
        };
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw