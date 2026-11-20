#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#include "darthex/adapters/Lexer.hpp"
#include "darthex/adapters/Parser.hpp"
#include "darthex/adapters/SymbolTable.hpp"
#include "darthex/application/services/LexService.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/syntax_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace {

using darthex::domain::LexicalError;
using darthex::domain::SyntaxError;
using darthex::domain::TokenKind;

struct FrontendResult {
  std::vector<LexicalError> lexical_errors;
  std::optional<SyntaxError> syntax_error;
};

FrontendResult analyze(const std::string& path) {
  std::ifstream file{path};
  REQUIRE(file.is_open());

  const std::string source{std::istreambuf_iterator<char>{file},
                           std::istreambuf_iterator<char>{}};
  darthex::adapters::Lexer lexer{source};
  darthex::adapters::SymbolTable symbols;
  darthex::application::services::LexService lex_service{lexer, symbols};
  lex_service.run();

  FrontendResult result{lex_service.errors(), std::nullopt};
  if (result.lexical_errors.empty()) {
    darthex::adapters::Parser parser;
    result.syntax_error = parser.parse(lex_service.tokens());
  }
  return result;
}

}  // namespace

TEST_CASE("corrected assignment programs pass syntax analysis") {
  const std::string fixtures[] = {
      "test1.dex", "test2.dex", "test3.dex", "test4.dex", "test5.dex",
      "extra1.dex", "extra2.dex",
  };

  for (const auto& fixture : fixtures) {
    DYNAMIC_SECTION(fixture) {
      const auto result =
          analyze(FIXTURE_DIR "/parser/valid/" + fixture);

      CHECK(result.lexical_errors.empty());
      CHECK_FALSE(result.syntax_error);
    }
  }
}

TEST_CASE("syntax fixtures contain no lexical errors and report the expected error") {
  struct InvalidFixture {
    const char* filename;
    TokenKind unexpected;
    const char* expected;
  };

  const InvalidFixture fixtures[] = {
      {"test1.dex", TokenKind::string_literal, "'('"},
      {"test2.dex", TokenKind::colon, "'='"},
      {"test3.dex", TokenKind::dot, "'end'"},
      {"test4.dex", TokenKind::kw_do, "')'"},
      {"test5.dex", TokenKind::identifier, "'then'"},
  };

  for (const auto& fixture : fixtures) {
    DYNAMIC_SECTION(fixture.filename) {
      const auto result =
          analyze(FIXTURE_DIR "/parser/invalid/" +
                  std::string{fixture.filename});

      CHECK(result.lexical_errors.empty());
      REQUIRE(result.syntax_error);
      CHECK(result.syntax_error->unexpected.kind == fixture.unexpected);
      CHECK(result.syntax_error->expected == fixture.expected);
    }
  }
}
