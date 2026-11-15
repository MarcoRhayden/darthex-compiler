#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/keyword.hpp"
#include "darthex/domain/token_kind.hpp"

using darthex::domain::TokenKind;
using darthex::domain::keyword_kind;

TEST_CASE("keyword_kind recognises all 15 reserved words") {
  CHECK(keyword_kind("program") == TokenKind::kw_program);
  CHECK(keyword_kind("begin")   == TokenKind::kw_begin);
  CHECK(keyword_kind("end")     == TokenKind::kw_end);
  CHECK(keyword_kind("int")     == TokenKind::kw_int);
  CHECK(keyword_kind("float")    == TokenKind::kw_float);
  CHECK(keyword_kind("char")    == TokenKind::kw_char);
  CHECK(keyword_kind("if")      == TokenKind::kw_if);
  CHECK(keyword_kind("then")    == TokenKind::kw_then);
  CHECK(keyword_kind("else")    == TokenKind::kw_else);
  CHECK(keyword_kind("while")   == TokenKind::kw_while);
  CHECK(keyword_kind("do")      == TokenKind::kw_do);
  CHECK(keyword_kind("repeat")  == TokenKind::kw_repeat);
  CHECK(keyword_kind("until")   == TokenKind::kw_until);
  CHECK(keyword_kind("read")    == TokenKind::kw_read);
  CHECK(keyword_kind("write")   == TokenKind::kw_write);
}

TEST_CASE("keyword_kind is case-sensitive") {
  CHECK_FALSE(keyword_kind("Program").has_value());
  CHECK_FALSE(keyword_kind("BEGIN").has_value());
  CHECK_FALSE(keyword_kind("Write").has_value());
  CHECK_FALSE(keyword_kind("IF").has_value());
}

TEST_CASE("keyword_kind returns nullopt for identifiers") {
  CHECK_FALSE(keyword_kind("x").has_value());
  CHECK_FALSE(keyword_kind("_var").has_value());
  CHECK_FALSE(keyword_kind("program2").has_value());
  CHECK_FALSE(keyword_kind("").has_value());
}
