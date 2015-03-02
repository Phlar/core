#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "InterfaceImpl.hpp"

#include <list>

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask, base::ReferenceCounted> {

    public:
        virtual ~Task();

        TaskList GetParents() const;
        
    protected:

        // A task itself cannot be constructed as such
        Task();


        TaskList m_parents;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw