#pragma once

#include <string>

#include "darthex/domain/source_location.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::domain {

struct Token {
  TokenKind kind{TokenKind::end_of_file};
  std::string lexeme;
  SourceLocation location;
};

inline bool operator==(const Token& a, const Token& b) {
  return a.kind == b.kind && a.lexeme == b.lexeme && a.location == b.location;
}

inline bool operator!=(const Token& a, const Token& b) {
  return !(a == b);
}

}  // namespace darthex::domain
