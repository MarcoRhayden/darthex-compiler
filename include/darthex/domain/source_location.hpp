#pragma once

#include <cstddef>

namespace darthex::domain {

struct SourceLocation {
  std::size_t line{1};
  std::size_t column{1};
  std::size_t offset{0};
};

constexpr bool operator==(const SourceLocation& a, const SourceLocation& b) {
  return a.line == b.line && a.column == b.column && a.offset == b.offset;
}

constexpr bool operator!=(const SourceLocation& a, const SourceLocation& b) {
  return !(a == b);
}

}  // namespace darthex::domain
