#include "dart/collision/hit/narrowphase/detail/failed_at_this_configuration.h"

#include <sstream>

namespace dart::collision::hit {
namespace detail {

void ThrowFailedAtThisConfiguration(
    const std::string& message, const char* func, const char* file, int line)
{
  std::stringstream ss;
  ss << file << ":(" << line << "): " << func << "(): " << message;
  throw FailedAtThisConfiguration(ss.str());
}

} // namespace detail
} // namespace dart::collision::hit
