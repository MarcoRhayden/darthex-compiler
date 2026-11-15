#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/token_kind.hpp"

using darthex::domain::TokenKind;
using darthex::domain::is_keyword;
using darthex::domain::token_kind_name;

TEST_CASE("is_keyword returns true for all reserved words") {
  CHECK(is_keyword(TokenKind::kw_program));
  CHECK(is_keyword(TokenKind::kw_begin));
  CHECK(is_keyword(TokenKind::kw_end));
  CHECK(is_keyword(TokenKind::kw_int));
  CHECK(is_keyword(TokenKind::kw_float));
  CHECK(is_keyword(TokenKind::kw_char));
  CHECK(is_keyword(TokenKind::kw_if));
  CHECK(is_keyword(TokenKind::kw_then));
  CHECK(is_keyword(TokenKind::kw_else));
  CHECK(is_keyword(TokenKind::kw_while));
  CHECK(is_keyword(TokenKind::kw_do));
  CHECK(is_keyword(TokenKind::kw_repeat));
  CHECK(is_keyword(TokenKind::kw_until));
  CHECK(is_keyword(TokenKind::kw_read));
  CHECK(is_keyword(TokenKind::kw_write));
}

TEST_CASE("is_keyword returns false for non-keywords") {
  CHECK_FALSE(is_keyword(TokenKind::identifier));
  CHECK_FALSE(is_keyword(TokenKind::integer_const));
  CHECK_FALSE(is_keyword(TokenKind::op_assign));
  CHECK_FALSE(is_keyword(TokenKind::end_of_file));
}

TEST_CASE("token_kind_name returns a non-empty string for every variant") {
  const TokenKind kinds[] = {
    TokenKind::end_of_file,    TokenKind::identifier,
    TokenKind::integer_const, TokenKind::float_const,
    TokenKind::char_const,    TokenKind::string_literal,
    TokenKind::kw_program,    TokenKind::kw_begin,
    TokenKind::kw_end,        TokenKind::kw_int,
    TokenKind::kw_float,       TokenKind::kw_char,
    TokenKind::kw_if,         TokenKind::kw_then,
    TokenKind::kw_else,       TokenKind::kw_while,
    TokenKind::kw_do,         TokenKind::kw_repeat,
    TokenKind::kw_until,      TokenKind::kw_read,
    TokenKind::kw_write,      TokenKind::op_assign,
    TokenKind::op_equal,      TokenKind::op_not_equal,
    TokenKind::op_less,       TokenKind::op_less_equal,
    TokenKind::op_greater,    TokenKind::op_greater_equal,
    TokenKind::op_plus,       TokenKind::op_minus,
    TokenKind::op_multiply,   TokenKind::op_divide,
    TokenKind::op_modulo,     TokenKind::op_and,
    TokenKind::op_or,         TokenKind::op_not,
    TokenKind::left_paren,    TokenKind::right_paren,
    TokenKind::comma,         TokenKind::colon,
    TokenKind::semicolon,     TokenKind::dot,
  };

  for (auto k : kinds) {
    CHECK_FALSE(token_kind_name(k).empty());
  }
}
