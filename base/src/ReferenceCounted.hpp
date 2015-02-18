#pragma once

#include "IReferenceCounted.hpp"

namespace aw {
namespace core {
namespace base {


class ReferenceCounted : IReferenceCounted {

    public:

        ReferenceCounted()
        : m_refCount(1) {
        }

        //! Todo: Atomic!
        void AddReference() {

            ++m_refCount;            
        }

        //! Todo: Atomic!
        void DecreaseReference() {

            --m_refCount;
            if(m_refCount == 0) {

                delete this;
            }
        }

    protected:

        int m_refCount;
};

} // namespace base
} // namespace core
} // namespace aw