#pragma once

#include <vector>

#include "darthex/application/ports/ILexer.hpp"
#include "darthex/application/ports/ISymbolTable.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token.hpp"

namespace darthex::application::services {

class LexService {
 public:
  LexService(ports::ILexer& lexer, ports::ISymbolTable& symbols);

  void run();

  const std::vector<domain::Token>& tokens() const;
  const std::vector<domain::LexicalError>& errors() const;
  bool has_errors() const;

 private:
  ports::ILexer& lexer_;
  ports::ISymbolTable& symbols_;
  std::vector<domain::Token> tokens_;
  std::vector<domain::LexicalError> errors_;
};

}  // namespace darthex::application::services
