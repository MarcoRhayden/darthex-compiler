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
         expect(TokenKind::op_assign, "'='") && parse_operand();
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

  if (!match(TokenKind::string_literal) && !parse_operand()) {
    return false;
  }

  return expect(TokenKind::right_paren, "')'");
}

bool Parser::parse_operand() {
  if (match(TokenKind::identifier) || match(TokenKind::integer_const) ||
      match(TokenKind::float_const) || match(TokenKind::char_const)) {
    return true;
  }
  return fail("identifier or constant");
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
  return check(TokenKind::identifier) || check(TokenKind::kw_read) ||
         check(TokenKind::kw_write);
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
