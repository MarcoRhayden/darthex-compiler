#include "darthex/adapters/Parser.hpp"

#include <string>

namespace darthex::adapters {

using domain::TokenKind;

application::ports::ParseResult Parser::parse(
    const std::vector<domain::Token>& tokens) {
  tokens_ = &tokens;
  current_index_ = 0;
  fallback_eof_ = {};
  error_.reset();

  if (!tokens.empty()) {
    fallback_eof_.location = tokens.back().location;
  }

  parse_program();
  return error_;
}

bool Parser::parse_program() {
  return expect(TokenKind::kw_program, "'program'") &&
         expect(TokenKind::identifier, "identifier") &&
         parse_declarations() &&
         expect(TokenKind::kw_begin, "'begin'") &&
         parse_statement_list() &&
         expect(TokenKind::kw_end, "'end'") &&
         expect(TokenKind::dot, "'.'") &&
         expect(TokenKind::end_of_file, "end of file");
}

bool Parser::parse_declarations() {
  while (check(TokenKind::identifier)) {
    if (!parse_declaration() || !expect(TokenKind::semicolon, "';'")) {
      return false;
    }
  }
  return true;
}

bool Parser::parse_declaration() {
  return parse_identifier_list() && expect(TokenKind::colon, "':'") &&
         parse_type();
}

bool Parser::parse_identifier_list() {
  if (!expect(TokenKind::identifier, "identifier")) {
    return false;
  }

  while (match(TokenKind::comma)) {
    if (!expect(TokenKind::identifier, "identifier")) {
      return false;
    }
  }
  return true;
}

bool Parser::parse_type() {
  if (match(TokenKind::kw_int) || match(TokenKind::kw_float) ||
      match(TokenKind::kw_char)) {
    return true;
  }
  return fail("type");
}

bool Parser::parse_statement_list() {
  if (!is_statement_start()) {
    return fail("statement");
  }

  while (true) {
    if (!parse_statement()) {
      return false;
    }

    if (!match(TokenKind::semicolon)) {
      if (is_statement_start()) {
        return fail("';'");
      }
      return true;
    }

    if (!is_statement_start()) {
      return true;
    }
  }
}

bool Parser::parse_statement() {
  if (check(TokenKind::identifier)) {
    return parse_assignment();
  }
  if (check(TokenKind::kw_if)) {
    return parse_if();
  }
  if (check(TokenKind::kw_while)) {
    return parse_while();
  }
  if (check(TokenKind::kw_repeat)) {
    return parse_repeat();
  }
  if (check(TokenKind::kw_read)) {
    return parse_read();
  }
  if (check(TokenKind::kw_write)) {
    return parse_write();
  }
  return fail("statement");
}

bool Parser::parse_assignment() {
  return expect(TokenKind::identifier, "identifier") &&
         expect(TokenKind::op_assign, "'='") && parse_simple_expression();
}

bool Parser::parse_if() {
  if (!expect(TokenKind::kw_if, "'if'") || !parse_expression() ||
      !expect(TokenKind::kw_then, "'then'") || !parse_statement_list()) {
    return false;
  }

  if (match(TokenKind::kw_else) && !parse_statement_list()) {
    return false;
  }

  return expect(TokenKind::kw_end, "'end'");
}

bool Parser::parse_while() {
  return expect(TokenKind::kw_while, "'while'") && parse_expression() &&
         expect(TokenKind::kw_do, "'do'") && parse_statement_list() &&
         expect(TokenKind::kw_end, "'end'");
}

bool Parser::parse_repeat() {
  return expect(TokenKind::kw_repeat, "'repeat'") && parse_statement_list() &&
         expect(TokenKind::kw_until, "'until'") && parse_expression();
}

bool Parser::parse_read() {
  return expect(TokenKind::kw_read, "'read'") &&
         expect(TokenKind::left_paren, "'('") &&
         expect(TokenKind::identifier, "identifier") &&
         expect(TokenKind::right_paren, "')'");
}

bool Parser::parse_write() {
  if (!expect(TokenKind::kw_write, "'write'") ||
      !expect(TokenKind::left_paren, "'('")) {
    return false;
  }

  if (!match(TokenKind::string_literal) && !parse_simple_expression()) {
    return false;
  }

  return expect(TokenKind::right_paren, "')'");
}

bool Parser::parse_expression() {
  if (!parse_simple_expression()) {
    return false;
  }

  if (is_relational_operator()) {
    advance();
    return parse_simple_expression();
  }
  return true;
}

bool Parser::parse_simple_expression() {
  if (!parse_term()) {
    return false;
  }

  while (is_additive_operator()) {
    advance();
    if (!parse_term()) {
      return false;
    }
  }
  return true;
}

bool Parser::parse_term() {
  if (!parse_factor_a()) {
    return false;
  }

  while (is_multiplicative_operator()) {
    advance();
    if (!parse_factor_a()) {
      return false;
    }
  }
  return true;
}

bool Parser::parse_factor_a() {
  if (check(TokenKind::op_not) || check(TokenKind::op_minus)) {
    advance();
  }
  return parse_factor();
}

bool Parser::parse_factor() {
  if (match(TokenKind::identifier) || match(TokenKind::integer_const) ||
      match(TokenKind::float_const) || match(TokenKind::char_const)) {
    return true;
  }

  if (match(TokenKind::left_paren)) {
    return parse_expression() && expect(TokenKind::right_paren, "')'");
  }

  return fail("expression value");
}

bool Parser::expect(TokenKind kind, std::string_view expected) {
  if (!check(kind)) {
    return fail(expected);
  }
  advance();
  return true;
}

bool Parser::fail(std::string_view expected) {
  if (!error_) {
    error_ = domain::SyntaxError{current(), std::string{expected}};
  }
  return false;
}

bool Parser::check(TokenKind kind) const {
  return current().kind == kind;
}

bool Parser::match(TokenKind kind) {
  if (!check(kind)) {
    return false;
  }
  advance();
  return true;
}

bool Parser::is_statement_start() const {
  return check(TokenKind::identifier) || check(TokenKind::kw_if) ||
         check(TokenKind::kw_while) || check(TokenKind::kw_repeat) ||
         check(TokenKind::kw_read) || check(TokenKind::kw_write);
}

bool Parser::is_relational_operator() const {
  return check(TokenKind::op_equal) || check(TokenKind::op_not_equal) ||
         check(TokenKind::op_less) || check(TokenKind::op_less_equal) ||
         check(TokenKind::op_greater) || check(TokenKind::op_greater_equal);
}

bool Parser::is_additive_operator() const {
  return check(TokenKind::op_plus) || check(TokenKind::op_minus) ||
         check(TokenKind::op_or);
}

bool Parser::is_multiplicative_operator() const {
  return check(TokenKind::op_multiply) || check(TokenKind::op_divide) ||
         check(TokenKind::op_modulo) || check(TokenKind::op_and);
}

const domain::Token& Parser::current() const {
  if (tokens_ != nullptr && current_index_ < tokens_->size()) {
    return (*tokens_)[current_index_];
  }
  return fallback_eof_;
}

void Parser::advance() {
  if (tokens_ != nullptr && current_index_ < tokens_->size()) {
    ++current_index_;
  }
}

}  // namespace darthex::adapters
