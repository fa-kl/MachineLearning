#ifndef LINALG_TYPES_H
#define LINALG_TYPES_H

#include <cstdint>
#include <array>

namespace LinAlg {

using size_t = uint32_t;
using index_t = int64_t;
using dimension_t = std::array<size_t, 2>;

};  // namespace LinAlg

#endif  // LINALG_TYPES_H
