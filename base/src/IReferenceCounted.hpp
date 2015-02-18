#pragma once

namespace aw {
namespace core {
namespace base {

class IReferenceCounted {

    public:

        virtual void AddReference() = 0;

        virtual void DecreaseReference() = 0;
};

} // namespace base
} // namespace core
} // namespace