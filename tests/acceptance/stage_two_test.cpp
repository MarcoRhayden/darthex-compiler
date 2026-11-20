#include <catch2/catch_test_macros.hpp>

#include <cstddef>
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

TEST_CASE("report fixtures reproduce every documented syntax result") {
  struct ReportFixture {
    const char* path;
    TokenKind unexpected;
    const char* expected;
    std::size_t line;
    std::size_t column;
  };

  const ReportFixture errors[] = {
      {"test1/01.dex", TokenKind::identifier, "':'", 5, 10},
      {"test1/02.dex", TokenKind::string_literal, "'('", 13, 7},
      {"test2/01.dex", TokenKind::op_assign, "':'", 4, 8},
      {"test2/02.dex", TokenKind::colon, "'='", 10, 5},
      {"test2/03.dex", TokenKind::dot, "'end'", 10, 9},
      {"test3/01.dex", TokenKind::comma, "'begin'", 2, 2},
      {"test3/02.dex", TokenKind::kw_begin, "';'", 4, 1},
      {"test3/03.dex", TokenKind::kw_write, "'then'", 15, 15},
      {"test3/04.dex", TokenKind::end_of_file, "'end'", 19, 1},
      {"test4/01.dex", TokenKind::kw_do, "')'", 12, 56},
      {"test4/02.dex", TokenKind::kw_write, "';'", 18, 1},
      {"test4/03.dex", TokenKind::end_of_file, "'.'", 24, 1},
      {"test5/01.dex", TokenKind::kw_end, "'then'", 13, 23},
      {"test5/02.dex", TokenKind::op_equal, "')'", 26, 26},
  };

  for (const auto& fixture : errors) {
    DYNAMIC_SECTION(fixture.path) {
      const auto result =
          analyze(FIXTURE_DIR "/parser/report/" +
                  std::string{fixture.path});

      CHECK(result.lexical_errors.empty());
      REQUIRE(result.syntax_error);
      CHECK(result.syntax_error->unexpected.kind == fixture.unexpected);
      CHECK(result.syntax_error->expected == fixture.expected);
      CHECK(result.syntax_error->unexpected.location.line == fixture.line);
      CHECK(result.syntax_error->unexpected.location.column == fixture.column);
    }
  }

  const char* successful[] = {
      "test1/03.dex", "test2/04.dex", "test3/05.dex",
      "test4/04.dex", "test5/03.dex",
  };

  for (const auto* fixture : successful) {
    DYNAMIC_SECTION(fixture) {
      const auto result =
          analyze(FIXTURE_DIR "/parser/report/" + std::string{fixture});

      CHECK(result.lexical_errors.empty());
      CHECK_FALSE(result.syntax_error);
    }
  }
}
