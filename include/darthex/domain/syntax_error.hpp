#pragma once

#include <string>

#include "darthex/domain/token.hpp"

namespace darthex::domain {

struct SyntaxError {
  Token unexpected;
  std::string expected;
};

inline bool operator==(const SyntaxError& a, const SyntaxError& b) {
  return a.unexpected == b.unexpected && a.expected == b.expected;
}

inline bool operator!=(const SyntaxError& a, const SyntaxError& b) {
  return !(a == b);
}

}  // namespace darthex::domain
