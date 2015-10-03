#pragma once

#include <boost/filesystem.hpp>

#include <windows.h>

namespace aw {
namespace core {
namespace base {
namespace utils {

//! \brief Look up the directory of the executable.
boost::filesystem::path GetProcessDirectory();

//! \brief Look up the absolute path of the executable.
boost::filesystem::path GetProcessFilePath();

} // namespace utils
} // namespace base
} // namespace core
} // namespace aw