#include "darthex/adapters/Lexer.hpp"

#include <string>
#include <utility>

#include "darthex/domain/char_class.hpp"
#include "darthex/domain/identifier.hpp"
#include "darthex/domain/keyword.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::adapters {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

Lexer::Lexer(std::string source) : source_{std::move(source)} {}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

application::ports::LexerResult Lexer::next() {
  if (const auto err = skip_trivia()) {
    return *err;
  }

  const auto start = pos_;

  if (at_end()) {
    return make_token(domain::TokenKind::end_of_file, start);
  }

  const char c = peek();

  if (!domain::is_ascii(c)) {
    skip_invalid_utf8();
    return make_error(domain::LexicalErrorKind::invalid_character, start);
  }
  if (domain::is_identifier_start(c)) {
    return scan_identifier(start);
  }
  if (domain::is_ascii_digit(c)) {
    return scan_number(start);
  }

  advance();
  return scan_symbol(c, start);
}

// ---------------------------------------------------------------------------
// Cursor primitives
// ---------------------------------------------------------------------------

bool Lexer::at_end() const {
  return pos_.offset >= source_.size();
}

char Lexer::peek() const {
  return at_end() ? '\0' : source_[pos_.offset];
}

char Lexer::peek_next() const {
  const auto next = pos_.offset + 1;
  return next >= source_.size() ? '\0' : source_[next];
}

char Lexer::advance() {
  const char c = source_[pos_.offset];
  ++pos_.offset;

  if (c == '\r') {
    if (!at_end() && peek() == '\n') {
      ++pos_.offset;
    }
    ++pos_.line;
    pos_.column = 1;
  } else if (c == '\n') {
    ++pos_.line;
    pos_.column = 1;
  } else {
    ++pos_.column;
  }

  return c;
}

bool Lexer::match(char expected) {
  if (at_end() || peek() != expected) {
    return false;
  }
  advance();
  return true;
}

void Lexer::skip_invalid_utf8() {
  advance();
  while (!at_end() && domain::is_utf8_continuation(peek())) {
    advance();
  }
}

// ---------------------------------------------------------------------------
// Trivia
// ---------------------------------------------------------------------------

std::optional<domain::LexicalError> Lexer::skip_trivia() {
  while (!at_end()) {
    if (domain::is_horizontal_space(peek()) || domain::is_line_break(peek())) {
      skip_whitespace();
    } else if (peek() == '{' && peek_next() == '*') {
      if (auto err = skip_comment()) {
        return err;
      }
    } else {
      break;
    }
  }
  return std::nullopt;
}

void Lexer::skip_whitespace() {
  while (!at_end() && (domain::is_horizontal_space(peek()) || domain::is_line_break(peek()))) {
    advance();
  }
}

std::optional<domain::LexicalError> Lexer::skip_comment() {
  const auto start = pos_;
  advance();  // '{'
  advance();  // '*'

  while (!at_end() && !(peek() == '*' && peek_next() == '}')) {
    advance();
  }

  if (at_end()) {
    return make_error(domain::LexicalErrorKind::unterminated_comment, start);
  }

  advance();  // '*'
  advance();  // '}'
  return std::nullopt;
}

// ---------------------------------------------------------------------------
// Scanners
// ---------------------------------------------------------------------------

domain::Token Lexer::scan_identifier(domain::SourceLocation start) {
  while (domain::is_identifier_part(peek())) {
    advance();
  }
  const auto lexeme = source_.substr(start.offset, pos_.offset - start.offset);
  const auto kind = domain::keyword_kind(lexeme).value_or(domain::TokenKind::identifier);
  return domain::Token{kind, lexeme, start};
}

application::ports::LexerResult Lexer::scan_number(domain::SourceLocation start) {
  while (domain::is_ascii_digit(peek())) {
    advance();
  }

  if (peek() == '.' && domain::is_ascii_digit(peek_next())) {
    return scan_float_suffix(start);
  }

  if (domain::is_identifier_start(peek())) {
    while (domain::is_identifier_part(peek())) {
      advance();
    }
    return make_error(domain::LexicalErrorKind::identifier_starts_with_digit, start);
  }

  return make_token(domain::TokenKind::integer_const, start);
}

application::ports::LexerResult Lexer::scan_float_suffix(domain::SourceLocation start) {
  advance();  // '.'
  while (domain::is_ascii_digit(peek())) {
    advance();
  }

  if (domain::is_identifier_start(peek())) {
    while (domain::is_identifier_part(peek())) {
      advance();
    }
    return make_error(domain::LexicalErrorKind::identifier_starts_with_digit, start);
  }

  return make_token(domain::TokenKind::float_const, start);
}

application::ports::LexerResult Lexer::scan_char_const(domain::SourceLocation start) {
  if (at_end() || domain::is_line_break(peek())) {
    return make_error(domain::LexicalErrorKind::unterminated_char_const, start);
  }

  if (peek() == '\'') {
    advance();
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  if (!domain::is_ascii(peek())) {
    skip_invalid_utf8();
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  advance();  // the single character

  if (match('\'')) {
    return make_token(domain::TokenKind::char_const, start);
  }

  while (!at_end() && !domain::is_line_break(peek()) && peek() != '\'') {
    advance();
  }

  if (match('\'')) {
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  return make_error(domain::LexicalErrorKind::unterminated_char_const, start);
}

application::ports::LexerResult Lexer::scan_string(domain::SourceLocation start) {
  while (!at_end() && !domain::is_line_break(peek()) && peek() != '"') {
    if (!domain::is_ascii(peek())) {
      const auto err_start = pos_;
      skip_invalid_utf8();
      return make_error(domain::LexicalErrorKind::invalid_character, err_start);
    }
    advance();
  }

  if (at_end() || domain::is_line_break(peek())) {
    return make_error(domain::LexicalErrorKind::unterminated_string, start);
  }

  advance();  // closing '"'
  return make_token(domain::TokenKind::string_literal, start);
}

application::ports::LexerResult Lexer::scan_symbol(char c, domain::SourceLocation start) {
  switch (c) {
    case '=':
      return make_token(match('=') ? domain::TokenKind::op_equal : domain::TokenKind::op_assign, start);
    case '!':
      return make_token(match('=') ? domain::TokenKind::op_not_equal : domain::TokenKind::op_not, start);
    case '<':
      return make_token(match('=') ? domain::TokenKind::op_less_equal : domain::TokenKind::op_less, start);
    case '>':
      return make_token(match('=') ? domain::TokenKind::op_greater_equal : domain::TokenKind::op_greater, start);
    case '+':
      return make_token(domain::TokenKind::op_plus, start);
    case '-':
      return make_token(domain::TokenKind::op_minus, start);
    case '*':
      return make_token(domain::TokenKind::op_multiply, start);
    case '/':
      return make_token(domain::TokenKind::op_divide, start);
    case '%':
      return make_token(domain::TokenKind::op_modulo, start);
    case '&':
      if (match('&')) return make_token(domain::TokenKind::op_and, start);
      return make_error(domain::LexicalErrorKind::invalid_character, start);
    case '|':
      if (match('|')) return make_token(domain::TokenKind::op_or, start);
      return make_error(domain::LexicalErrorKind::invalid_character, start);
    case '(':  return make_token(domain::TokenKind::left_paren,   start);
    case ')':  return make_token(domain::TokenKind::right_paren,  start);
    case ',':  return make_token(domain::TokenKind::comma,        start);
    case ':':  return make_token(domain::TokenKind::colon,        start);
    case ';':  return make_token(domain::TokenKind::semicolon,    start);
    case '.':  return make_token(domain::TokenKind::dot,          start);
    case '\'': return scan_char_const(start);
    case '"':  return scan_string(start);
    default:
      return make_error(domain::LexicalErrorKind::invalid_character, start);
  }
}

// ---------------------------------------------------------------------------
// Factories
// ---------------------------------------------------------------------------

domain::Token Lexer::make_token(domain::TokenKind kind, domain::SourceLocation start) const {
  return domain::Token{kind, source_.substr(start.offset, pos_.offset - start.offset), start};
}

domain::LexicalError Lexer::make_error(domain::LexicalErrorKind kind, domain::SourceLocation start) const {
  return domain::LexicalError{kind, source_.substr(start.offset, pos_.offset - start.offset), start};
}

}  // namespace darthex::adapters
