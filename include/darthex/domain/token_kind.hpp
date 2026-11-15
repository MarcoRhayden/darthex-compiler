#pragma once

#include <string_view>

namespace darthex::domain {

enum class TokenKind {
  end_of_file,

  identifier,
  integer_const,
  float_const,
  char_const,
  string_literal,

  kw_program,
  kw_begin,
  kw_end,
  kw_int,
  kw_float,
  kw_char,
  kw_if,
  kw_then,
  kw_else,
  kw_while,
  kw_do,
  kw_repeat,
  kw_until,
  kw_read,
  kw_write,

  op_assign,
  op_equal,
  op_not_equal,
  op_less,
  op_less_equal,
  op_greater,
  op_greater_equal,
  op_plus,
  op_minus,
  op_multiply,
  op_divide,
  op_modulo,
  op_and,
  op_or,
  op_not,

  left_paren,
  right_paren,
  comma,
  colon,
  semicolon,
  dot,
};

constexpr bool is_keyword(TokenKind k) {
  return k >= TokenKind::kw_program && k <= TokenKind::kw_write;
}

constexpr std::string_view token_kind_name(TokenKind k) {
  switch (k) {
    case TokenKind::end_of_file:       return "end_of_file";
    case TokenKind::identifier:        return "identifier";
    case TokenKind::integer_const:    return "integer_const";
    case TokenKind::float_const:       return "float_const";
    case TokenKind::char_const:       return "char_const";
    case TokenKind::string_literal:   return "string_literal";
    case TokenKind::kw_program:       return "kw_program";
    case TokenKind::kw_begin:         return "kw_begin";
    case TokenKind::kw_end:           return "kw_end";
    case TokenKind::kw_int:           return "kw_int";
    case TokenKind::kw_float:          return "kw_float";
    case TokenKind::kw_char:          return "kw_char";
    case TokenKind::kw_if:            return "kw_if";
    case TokenKind::kw_then:          return "kw_then";
    case TokenKind::kw_else:          return "kw_else";
    case TokenKind::kw_while:         return "kw_while";
    case TokenKind::kw_do:            return "kw_do";
    case TokenKind::kw_repeat:        return "kw_repeat";
    case TokenKind::kw_until:         return "kw_until";
    case TokenKind::kw_read:          return "kw_read";
    case TokenKind::kw_write:         return "kw_write";
    case TokenKind::op_assign:        return "op_assign";
    case TokenKind::op_equal:         return "op_equal";
    case TokenKind::op_not_equal:     return "op_not_equal";
    case TokenKind::op_less:          return "op_less";
    case TokenKind::op_less_equal:    return "op_less_equal";
    case TokenKind::op_greater:       return "op_greater";
    case TokenKind::op_greater_equal: return "op_greater_equal";
    case TokenKind::op_plus:          return "op_plus";
    case TokenKind::op_minus:         return "op_minus";
    case TokenKind::op_multiply:      return "op_multiply";
    case TokenKind::op_divide:        return "op_divide";
    case TokenKind::op_modulo:        return "op_modulo";
    case TokenKind::op_and:           return "op_and";
    case TokenKind::op_or:            return "op_or";
    case TokenKind::op_not:           return "op_not";
    case TokenKind::left_paren:       return "left_paren";
    case TokenKind::right_paren:      return "right_paren";
    case TokenKind::comma:            return "comma";
    case TokenKind::colon:            return "colon";
    case TokenKind::semicolon:        return "semicolon";
    case TokenKind::dot:              return "dot";
  }
  return "unknown";
}

}  // namespace darthex::domain
