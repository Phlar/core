#pragma once

#include "AIFwdDeclarations.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace impl {

template<typename T>
struct InstanceEntry {

    InstanceEntry(boost::intrusive_ptr<T> taskImpl)
    :

    boost::intrusive_ptr<T> m_taskImpl;
    
};


} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw