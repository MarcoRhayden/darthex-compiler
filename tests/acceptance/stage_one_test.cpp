#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "darthex/adapters/Lexer.hpp"
#include "darthex/adapters/SymbolTable.hpp"
#include "darthex/application/services/LexService.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace {

struct LexResult {
  std::vector<darthex::domain::Token> tokens;
  std::vector<darthex::domain::LexicalError> errors;
  bool has_errors;
};

LexResult run_fixture(const char* path) {
  std::ifstream f{path};
  REQUIRE(f.is_open());
  const std::string source{std::istreambuf_iterator<char>{f},
                            std::istreambuf_iterator<char>{}};
  darthex::adapters::Lexer lexer{source};
  darthex::adapters::SymbolTable symbols;
  darthex::application::services::LexService service{lexer, symbols};
  service.run();
  return {service.tokens(), service.errors(), service.has_errors()};
}

}  // namespace

using darthex::domain::LexicalErrorKind;
using darthex::domain::TokenKind;

// ---------------------------------------------------------------------------
// Test 1: unterminated block comment consumes rest of file
// {*Calculo de perimetro e area} has no closing *}
// ---------------------------------------------------------------------------
TEST_CASE("fixture test1: unterminated comment swallows file from line 2") {
  const auto r = run_fixture(FIXTURE_DIR "/lexer/test1.dex");

  REQUIRE(r.errors.size() == 1);
  CHECK(r.errors[0].kind == LexicalErrorKind::unterminated_comment);
  CHECK(r.errors[0].location.line == 2);
  CHECK(r.has_errors);

  // tokens produced before the error
  REQUIRE(r.tokens.size() >= 2);
  CHECK(r.tokens[0].kind == TokenKind::identifier);
  CHECK(r.tokens[0].lexeme == "programa");
  CHECK(r.tokens[1].kind == TokenKind::identifier);
  CHECK(r.tokens[1].lexeme == "teste1");

  // last token is always end_of_file
  CHECK(r.tokens.back().kind == TokenKind::end_of_file);
}

// ---------------------------------------------------------------------------
// Test 2: identifier starting with a digit
// 1c:int -- the token 1c triggers identifier_starts_with_digit
// ---------------------------------------------------------------------------
TEST_CASE("fixture test2: identifier starting with digit on line 2") {
  const auto r = run_fixture(FIXTURE_DIR "/lexer/test2.dex");

  REQUIRE(r.errors.size() == 1);
  CHECK(r.errors[0].kind == LexicalErrorKind::identifier_starts_with_digit);
  CHECK(r.errors[0].lexeme == "1c");
  CHECK(r.errors[0].location.line == 2);
  CHECK(r.has_errors);

  // program keyword and name are present
  CHECK(r.tokens[0].kind == TokenKind::kw_program);
  CHECK(r.tokens[1].kind == TokenKind::identifier);
  CHECK(r.tokens[1].lexeme == "teste2");

  // lexer continues past the error: float literal 2.2 is scanned
  bool found_float = false;
  for (const auto& t : r.tokens) {
    if (t.kind == TokenKind::float_const && t.lexeme == "2.2") {
      found_float = true;
    }
  }
  CHECK(found_float);

  CHECK(r.tokens.back().kind == TokenKind::end_of_file);
}

// ---------------------------------------------------------------------------
// Test 3: unterminated string literal
// write ("Par);  -- the string never closes on the same line
// ---------------------------------------------------------------------------
TEST_CASE("fixture test3: unterminated string literal on line 15") {
  const auto r = run_fixture(FIXTURE_DIR "/lexer/test3.dex");

  REQUIRE(r.errors.size() == 1);
  CHECK(r.errors[0].kind == LexicalErrorKind::unterminated_string);
  CHECK(r.errors[0].location.line == 15);
  CHECK(r.has_errors);

  // lexer recovers: "Impar" on the next line is a valid string
  bool found_impar = false;
  for (const auto& t : r.tokens) {
    if (t.kind == TokenKind::string_literal && t.lexeme == "\"Impar\"") {
      found_impar = true;
    }
  }
  CHECK(found_impar);

  CHECK(r.tokens.back().kind == TokenKind::end_of_file);
}

// ---------------------------------------------------------------------------
// Test 4: unterminated block comment (comment never closed)
// {* Teste4 do meu compilador  -- no *} anywhere in the file
// ---------------------------------------------------------------------------
TEST_CASE("fixture test4: unterminated comment swallows file from line 2") {
  const auto r = run_fixture(FIXTURE_DIR "/lexer/test4.dex");

  REQUIRE(r.errors.size() == 1);
  CHECK(r.errors[0].kind == LexicalErrorKind::unterminated_comment);
  CHECK(r.errors[0].location.line == 2);
  CHECK(r.has_errors);

  REQUIRE(r.tokens.size() >= 2);
  CHECK(r.tokens[0].kind == TokenKind::identifier);
  CHECK(r.tokens[0].lexeme == "programa");
  CHECK(r.tokens[1].kind == TokenKind::identifier);
  CHECK(r.tokens[1].lexeme == "teste4");

  CHECK(r.tokens.back().kind == TokenKind::end_of_file);
}

// ---------------------------------------------------------------------------
// Test 5: unterminated string on line 22 + unterminated char const on line 26
//
// Line 22: write("Maior valor: "");
//   The second " closes string 1; the third " opens string 2 whose content
//   "); hits the newline before a closing " is found.
//
// Line 26: until (opc == 'N' || opc == 'n)
//   'N' is a valid char_const; 'n) has no closing ' before end of line.
// ---------------------------------------------------------------------------
TEST_CASE("fixture test5: unterminated string line 22 and unterminated char const line 26") {
  const auto r = run_fixture(FIXTURE_DIR "/lexer/test5.dex");

  REQUIRE(r.errors.size() == 2);

  CHECK(r.errors[0].kind == LexicalErrorKind::unterminated_string);
  CHECK(r.errors[0].location.line == 22);

  CHECK(r.errors[1].kind == LexicalErrorKind::unterminated_char_const);
  CHECK(r.errors[1].location.line == 26);

  CHECK(r.has_errors);

  // opening comment {* ... *} is properly closed -- first token is kw_program
  CHECK(r.tokens[0].kind == TokenKind::kw_program);
  CHECK(r.tokens[1].kind == TokenKind::identifier);
  CHECK(r.tokens[1].lexeme == "teste5");

  // 'N' (uppercase) before the error is a valid char_const
  bool found_N = false;
  for (const auto& t : r.tokens) {
    if (t.kind == TokenKind::char_const && t.lexeme == "'N'") {
      found_N = true;
    }
  }
  CHECK(found_N);

  // lexer recovers after both errors: end. on line 27 is fully tokenised
  CHECK(r.tokens.back().kind == TokenKind::end_of_file);
  bool found_end_kw = false;
  bool found_dot = false;
  for (const auto& t : r.tokens) {
    if (t.kind == TokenKind::kw_end)  found_end_kw = true;
    if (t.kind == TokenKind::dot)     found_dot    = true;
  }
  CHECK(found_end_kw);
  CHECK(found_dot);
}
