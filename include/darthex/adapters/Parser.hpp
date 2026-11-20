#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "darthex/application/ports/IParser.hpp"
#include "darthex/domain/token.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::adapters {

class Parser : public application::ports::IParser {
 public:
  application::ports::ParseResult parse(
      const std::vector<domain::Token>& tokens) override;

 private:
  bool parse_program();
  bool parse_declarations();
  bool parse_declaration();
  bool parse_identifier_list();
  bool parse_type();
  bool parse_statement_list();
  bool parse_statement();
  bool parse_assignment();
  bool parse_read();
  bool parse_write();
  bool parse_expression();
  bool parse_simple_expression();
  bool parse_term();
  bool parse_factor_a();
  bool parse_factor();

  bool expect(domain::TokenKind kind, std::string_view expected);
  bool fail(std::string_view expected);
  bool check(domain::TokenKind kind) const;
  bool match(domain::TokenKind kind);
  bool is_statement_start() const;
  bool is_relational_operator() const;
  bool is_additive_operator() const;
  bool is_multiplicative_operator() const;
  const domain::Token& current() const;
  void advance();

  const std::vector<domain::Token>* tokens_{nullptr};
  std::size_t current_index_{0};
  domain::Token fallback_eof_;
  application::ports::ParseResult error_;
};

}  // namespace darthex::adapters
