#pragma once

#include <optional>
#include <string>

#include "darthex/application/ports/ILexer.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/source_location.hpp"
#include "darthex/domain/token.hpp"

namespace darthex::adapters {

class Lexer : public application::ports::ILexer {
 public:
  explicit Lexer(std::string source);
  ~Lexer() override = default;

  application::ports::LexerResult next() override;

 private:
  bool at_end() const;
  char peek() const;
  char peek_next() const;
  char advance();
  bool match(char expected);
  void skip_invalid_utf8();

  std::optional<domain::LexicalError> skip_trivia();

  domain::Token scan_identifier(domain::SourceLocation start);
  application::ports::LexerResult scan_number(domain::SourceLocation start);
  application::ports::LexerResult scan_char_const(domain::SourceLocation start);
  application::ports::LexerResult scan_string(domain::SourceLocation start);

  domain::Token make_token(domain::TokenKind kind, domain::SourceLocation start) const;
  domain::LexicalError make_error(domain::LexicalErrorKind kind, domain::SourceLocation start) const;

  std::string source_;
  domain::SourceLocation pos_;
};

}  // namespace darthex::adapters
