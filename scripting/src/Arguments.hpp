#pragma once

#include "../../ai/src/IBlackboard.hpp"

#include <boost/variant.hpp>

namespace aw {
namespace core {
namespace scriptingservice {

// Variant of arguments that get passsed to script resolvers as function parameters.
typedef boost::variant<int16_t,
                       float,
                       std::string, 
                       aw::core::ai::IBlackboardPtr> Argument;

} // namespace scriptingservice
} // namespace core
} // namespace aw



