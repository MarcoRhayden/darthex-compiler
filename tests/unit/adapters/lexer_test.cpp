#include <catch2/catch_test_macros.hpp>
#include <variant>
#include <vector>

#include "darthex/adapters/Lexer.hpp"
#include "darthex/application/ports/ILexer.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/source_location.hpp"
#include "darthex/domain/token.hpp"
#include "darthex/domain/token_kind.hpp"

using darthex::adapters::Lexer;
using darthex::application::ports::LexerResult;
using darthex::domain::LexicalError;
using darthex::domain::LexicalErrorKind;
using darthex::domain::SourceLocation;
using darthex::domain::Token;
using darthex::domain::TokenKind;

namespace {

Token next_token(Lexer& lexer) {
  const auto result = lexer.next();
  REQUIRE(std::holds_alternative<Token>(result));
  return std::get<Token>(result);
}

LexicalError next_error(Lexer& lexer) {
  const auto result = lexer.next();
  REQUIRE(std::holds_alternative<LexicalError>(result));
  return std::get<LexicalError>(result);
}

std::vector<Token> scan_all(const std::string& source) {
  Lexer lexer{source};
  std::vector<Token> tokens;
  while (true) {
    auto t = next_token(lexer);
    tokens.push_back(t);
    if (t.kind == TokenKind::end_of_file) {
      return tokens;
    }
  }
}

}  // namespace

TEST_CASE("empty source produces end_of_file at 1:1") {
  Lexer lexer{""};
  auto t = next_token(lexer);
  CHECK(t.kind == TokenKind::end_of_file);
  CHECK(t.location == SourceLocation{1, 1, 0});
}

TEST_CASE("lexer skips whitespace and tracks location") {
  Lexer lexer{" \t\r\nwrite"};
  auto t = next_token(lexer);
  CHECK(t.kind == TokenKind::kw_write);
  CHECK(t.location.line == 2);
  CHECK(t.location.column == 1);
}

TEST_CASE("lexer skips block comments") {
  Lexer lexer{"{* first line\nsecond line *}\nread"};
  auto t = next_token(lexer);
  CHECK(t.kind == TokenKind::kw_read);
  CHECK(t.location.line == 3);
}

TEST_CASE("unterminated comment returns error at comment start") {
  Lexer lexer{"{* not closed"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::unterminated_comment);
  CHECK(e.location == SourceLocation{1, 1, 0});
}

TEST_CASE("lexer recognises all 15 keywords case-sensitively") {
  const auto tokens = scan_all(
      "program begin end int float char if then else while do repeat until read write");
  REQUIRE(tokens.size() == 16);
  CHECK(tokens[0].kind  == TokenKind::kw_program);
  CHECK(tokens[1].kind  == TokenKind::kw_begin);
  CHECK(tokens[2].kind  == TokenKind::kw_end);
  CHECK(tokens[3].kind  == TokenKind::kw_int);
  CHECK(tokens[4].kind  == TokenKind::kw_float);
  CHECK(tokens[5].kind  == TokenKind::kw_char);
  CHECK(tokens[6].kind  == TokenKind::kw_if);
  CHECK(tokens[7].kind  == TokenKind::kw_then);
  CHECK(tokens[8].kind  == TokenKind::kw_else);
  CHECK(tokens[9].kind  == TokenKind::kw_while);
  CHECK(tokens[10].kind == TokenKind::kw_do);
  CHECK(tokens[11].kind == TokenKind::kw_repeat);
  CHECK(tokens[12].kind == TokenKind::kw_until);
  CHECK(tokens[13].kind == TokenKind::kw_read);
  CHECK(tokens[14].kind == TokenKind::kw_write);
}

TEST_CASE("keywords are case-sensitive — Write and Program are identifiers") {
  const auto tokens = scan_all("Write Program BEGIN");
  CHECK(tokens[0].kind == TokenKind::identifier);
  CHECK(tokens[1].kind == TokenKind::identifier);
  CHECK(tokens[2].kind == TokenKind::identifier);
}

TEST_CASE("identifiers: letters, digits, underscore") {
  const auto tokens = scan_all("x _var value2 _");
  CHECK(tokens[0].kind == TokenKind::identifier);
  CHECK(tokens[0].lexeme == "x");
  CHECK(tokens[1].lexeme == "_var");
  CHECK(tokens[2].lexeme == "value2");
  CHECK(tokens[3].lexeme == "_");
}

TEST_CASE("integer constants") {
  const auto tokens = scan_all("0 42 100");
  CHECK(tokens[0].kind == TokenKind::integer_const);
  CHECK(tokens[0].lexeme == "0");
  CHECK(tokens[1].lexeme == "42");
  CHECK(tokens[2].lexeme == "100");
}

TEST_CASE("float constants require digits on both sides of dot") {
  const auto tokens = scan_all("3.14 0.5");
  CHECK(tokens[0].kind == TokenKind::float_const);
  CHECK(tokens[0].lexeme == "3.14");
  CHECK(tokens[1].kind == TokenKind::float_const);
}

TEST_CASE("34. is integer followed by dot, not a float") {
  const auto tokens = scan_all("34.");
  REQUIRE(tokens.size() == 3);
  CHECK(tokens[0].kind == TokenKind::integer_const);
  CHECK(tokens[0].lexeme == "34");
  CHECK(tokens[1].kind == TokenKind::dot);
}

TEST_CASE("identifier starting with digit produces error") {
  Lexer lexer{"1abc"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::identifier_starts_with_digit);
  CHECK(e.lexeme == "1abc");
  CHECK(e.location.line == 1);
  CHECK(e.location.column == 1);
}

TEST_CASE("valid char constant") {
  Lexer lexer{"'a'"};
  auto t = next_token(lexer);
  CHECK(t.kind == TokenKind::char_const);
  CHECK(t.lexeme == "'a'");
}

TEST_CASE("empty char constant is invalid") {
  Lexer lexer{"''"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::invalid_char_const);
}

TEST_CASE("multi-char constant is invalid") {
  Lexer lexer{"'ab'"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::invalid_char_const);
}

TEST_CASE("unterminated char constant") {
  Lexer lexer{"'a"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::unterminated_char_const);
}

TEST_CASE("valid string literal") {
  Lexer lexer{"\"hello world\""};
  auto t = next_token(lexer);
  CHECK(t.kind == TokenKind::string_literal);
  CHECK(t.lexeme == "\"hello world\"");
}

TEST_CASE("unterminated string literal") {
  Lexer lexer{"\"not closed"};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::unterminated_string);
  CHECK(e.location.column == 1);
}

TEST_CASE("string literal does not span newlines") {
  Lexer lexer{"\"line one\nline two\""};
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::unterminated_string);
}

TEST_CASE("operators and punctuation") {
  const auto tokens = scan_all("= == != < <= > >= + - * / % && || ! ( ) , : ; .");
  REQUIRE(tokens.size() == 22);
  CHECK(tokens[0].kind  == TokenKind::op_assign);
  CHECK(tokens[1].kind  == TokenKind::op_equal);
  CHECK(tokens[2].kind  == TokenKind::op_not_equal);
  CHECK(tokens[3].kind  == TokenKind::op_less);
  CHECK(tokens[4].kind  == TokenKind::op_less_equal);
  CHECK(tokens[5].kind  == TokenKind::op_greater);
  CHECK(tokens[6].kind  == TokenKind::op_greater_equal);
  CHECK(tokens[7].kind  == TokenKind::op_plus);
  CHECK(tokens[8].kind  == TokenKind::op_minus);
  CHECK(tokens[9].kind  == TokenKind::op_multiply);
  CHECK(tokens[10].kind == TokenKind::op_divide);
  CHECK(tokens[11].kind == TokenKind::op_modulo);
  CHECK(tokens[12].kind == TokenKind::op_and);
  CHECK(tokens[13].kind == TokenKind::op_or);
  CHECK(tokens[14].kind == TokenKind::op_not);
  CHECK(tokens[15].kind == TokenKind::left_paren);
  CHECK(tokens[16].kind == TokenKind::right_paren);
  CHECK(tokens[17].kind == TokenKind::comma);
  CHECK(tokens[18].kind == TokenKind::colon);
  CHECK(tokens[19].kind == TokenKind::semicolon);
  CHECK(tokens[20].kind == TokenKind::dot);
}

TEST_CASE("single & and | are invalid characters") {
  {
    Lexer lexer{"&"};
    CHECK(next_error(lexer).kind == LexicalErrorKind::invalid_character);
  }
  {
    Lexer lexer{"|"};
    CHECK(next_error(lexer).kind == LexicalErrorKind::invalid_character);
  }
}

TEST_CASE("non-ASCII character produces invalid_character error") {
  Lexer lexer{"\xc3\xa9"};  // UTF-8 'é'
  auto e = next_error(lexer);
  CHECK(e.kind == LexicalErrorKind::invalid_character);
  CHECK(e.location == SourceLocation{1, 1, 0});
}

TEST_CASE("lexeme and location are correct after multi-token source") {
  Lexer lexer{"x = 42"};
  auto t0 = next_token(lexer);
  auto t1 = next_token(lexer);
  auto t2 = next_token(lexer);

  CHECK(t0.lexeme == "x");
  CHECK(t0.location.column == 1);
  CHECK(t1.kind == TokenKind::op_assign);
  CHECK(t1.location.column == 3);
  CHECK(t2.kind == TokenKind::integer_const);
  CHECK(t2.lexeme == "42");
  CHECK(t2.location.column == 5);
}
