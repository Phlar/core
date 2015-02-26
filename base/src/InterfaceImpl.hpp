#pragma once

#include "ReferenceCounted.hpp"

#include <boost/intrusive_ptr.hpp>


namespace boost
{
    void intrusive_ptr_add_ref(aw::core::base::IReferenceCounted* refCounted);
    void intrusive_ptr_release(aw::core::base::IReferenceCounted* refCounted);
};


namespace aw {
namespace core {
namespace base {

template<typename IfT, typename TRef = ReferenceCounted>
class InterfaceImpl : public ReferenceCounted, public IfT {

    protected:

        InterfaceImpl(){
        }

        virtual ~InterfaceImpl() {
        }

        friend void boost::intrusive_ptr_add_ref(IReferenceCounted* refCounted);
        friend void boost::intrusive_ptr_release(IReferenceCounted* refCounted);
};

} // namespace base
} // namespace core
} // namespace aw

namespace boost
{
    inline void intrusive_ptr_add_ref(aw::core::base::IReferenceCounted* refCounted)
    {
        refCounted->AddReference();
    }

    inline void intrusive_ptr_release(aw::core::base::IReferenceCounted* refCounted)
    {
        refCounted->DecreaseReference();
    } 
} // namespace boost