#pragma once

#include "AIFwdDeclarations.hpp"
#include "ITask.hpp"

#include "InterfaceImpl.hpp"

namespace aw {
namespace core {
namespace ai {
namespace impl {

class Task : public base::InterfaceImpl<ITask> {

    public:
        virtual ~Task();

        UUID GetID() const;

    protected:

        // A task itself cannot be constructed as such
        Task();

    private:

        UUID m_id;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw