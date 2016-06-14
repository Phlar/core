#pragma once

#include "UUID.hpp"

#include <boost/filesystem.hpp>
#include <boost/uuid/string_generator.hpp>

#include <string>


namespace aw {
namespace core {
namespace base {
namespace utils {

//! \brief Look up the directory of the executable.
boost::filesystem::path GetProcessDirectory();

//! \brief Look up the absolute path of the executable.
boost::filesystem::path GetProcessFilePath();

//! \brief Convenience creator for UUIDs from a string.
//! Throws an exception in case of an invalid string prrovided.
base::UUID CreateUUIDFromString(const std::string& uuidString);

} // namespace utils
} // namespace base
} // namespace core
} // namespace aw