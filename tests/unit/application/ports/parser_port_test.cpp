#include <cstddef>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "darthex/application/ports/IParser.hpp"

using darthex::application::ports::IParser;
using darthex::application::ports::ParseResult;
using darthex::domain::SyntaxError;
using darthex::domain::Token;
using darthex::domain::TokenKind;

namespace {

class StubParser final : public IParser {
 public:
  ParseResult parse(const std::vector<Token>& tokens) override {
    received_tokens = tokens.size();
    return result;
  }

  std::size_t received_tokens{0};
  ParseResult result;
};

}  // namespace

TEST_CASE("IParser accepts tokens and reports success without an error") {
  StubParser parser;
  const std::vector<Token> tokens{{TokenKind::end_of_file, {}, {}}};

  const auto result = parser.parse(tokens);

  CHECK_FALSE(result.has_value());
  CHECK(parser.received_tokens == 1);
}

TEST_CASE("IParser returns the first syntax error") {
  StubParser parser;
  parser.result = SyntaxError{{TokenKind::identifier, "value", {}}, "'program'"};

  const auto result = parser.parse({{TokenKind::identifier, "value", {}}});

  REQUIRE(result.has_value());
  CHECK(result->unexpected.lexeme == "value");
  CHECK(result->expected == "'program'");
}
