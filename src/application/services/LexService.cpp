#include "darthex/application/services/LexService.hpp"

#include <variant>

#include "darthex/domain/token_kind.hpp"

namespace darthex::application::services {

LexService::LexService(ports::ILexer& lexer, ports::ISymbolTable& symbols)
    : lexer_{lexer}, symbols_{symbols} {}

void LexService::run() {
  while (true) {
    auto result = lexer_.next();

    if (std::holds_alternative<domain::LexicalError>(result)) {
      errors_.push_back(std::get<domain::LexicalError>(result));
      continue;
    }

    auto token = std::get<domain::Token>(result);

    if (token.kind == domain::TokenKind::identifier || domain::is_keyword(token.kind)) {
      symbols_.install(token.lexeme);
    }

    tokens_.push_back(token);

    if (token.kind == domain::TokenKind::end_of_file) {
      break;
    }
  }
}

const std::vector<domain::Token>& LexService::tokens() const {
  return tokens_;
}

const std::vector<domain::LexicalError>& LexService::errors() const {
  return errors_;
}

bool LexService::has_errors() const {
  return !errors_.empty();
}

}  // namespace darthex::application::services
