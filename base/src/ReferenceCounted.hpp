#pragma once

#include "IReferenceCounted.hpp"

namespace aw {
namespace core {
namespace base {


class ReferenceCounted : public virtual IReferenceCounted {

    public:

        ReferenceCounted()
        : m_refCount(0) {
        }

        virtual ~ReferenceCounted() {
        }

        //! Todo: Atomic!
        virtual void AddReference() {

            ++m_refCount;
        }

        //! Todo: Atomic!
        virtual void DecreaseReference() {

            --m_refCount;
            if(m_refCount == 0) {

                delete this;
            }
        }

        int GetRefCount() const {

            return m_refCount;
        }

    protected:

        int m_refCount;
};

} // namespace base
} // namespace core
} // namespace aw