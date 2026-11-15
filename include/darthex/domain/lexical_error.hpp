#pragma once

#include <string>
#include <string_view>

#include "darthex/domain/source_location.hpp"

namespace darthex::domain {

enum class LexicalErrorKind {
  invalid_character,
  identifier_starts_with_digit,
  unterminated_comment,
  unterminated_char_const,
  invalid_char_const,
  unterminated_string,
};

constexpr std::string_view lexical_error_kind_name(LexicalErrorKind k) {
  switch (k) {
    case LexicalErrorKind::invalid_character:           return "invalid_character";
    case LexicalErrorKind::identifier_starts_with_digit: return "identifier_starts_with_digit";
    case LexicalErrorKind::unterminated_comment:        return "unterminated_comment";
    case LexicalErrorKind::unterminated_char_const:     return "unterminated_char_const";
    case LexicalErrorKind::invalid_char_const:          return "invalid_char_const";
    case LexicalErrorKind::unterminated_string:         return "unterminated_string";
  }
  return "unknown";
}

struct LexicalError {
  LexicalErrorKind kind{LexicalErrorKind::invalid_character};
  std::string lexeme;
  SourceLocation location;
};

inline bool operator==(const LexicalError& a, const LexicalError& b) {
  return a.kind == b.kind && a.lexeme == b.lexeme && a.location == b.location;
}

inline bool operator!=(const LexicalError& a, const LexicalError& b) {
  return !(a == b);
}

}  // namespace darthex::domain
