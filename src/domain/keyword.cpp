#include "darthex/domain/keyword.hpp"

namespace darthex::domain {

std::optional<TokenKind> keyword_kind(std::string_view lexeme) {
  if (lexeme == "program") return TokenKind::kw_program;
  if (lexeme == "begin")   return TokenKind::kw_begin;
  if (lexeme == "end")     return TokenKind::kw_end;
  if (lexeme == "int")     return TokenKind::kw_int;
  if (lexeme == "float")    return TokenKind::kw_float;
  if (lexeme == "char")    return TokenKind::kw_char;
  if (lexeme == "if")      return TokenKind::kw_if;
  if (lexeme == "then")    return TokenKind::kw_then;
  if (lexeme == "else")    return TokenKind::kw_else;
  if (lexeme == "while")   return TokenKind::kw_while;
  if (lexeme == "do")      return TokenKind::kw_do;
  if (lexeme == "repeat")  return TokenKind::kw_repeat;
  if (lexeme == "until")   return TokenKind::kw_until;
  if (lexeme == "read")    return TokenKind::kw_read;
  if (lexeme == "write")   return TokenKind::kw_write;
  return std::nullopt;
}

}  // namespace darthex::domain
