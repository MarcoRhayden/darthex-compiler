#include "darthex/adapters/Lexer.hpp"

#include <string>
#include <utility>

#include "darthex/domain/identifier.hpp"
#include "darthex/domain/keyword.hpp"
#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::adapters {

namespace {

constexpr bool is_ascii(char c) {
  return static_cast<unsigned char>(c) <= 127;
}

constexpr bool is_utf8_continuation(char c) {
  return (static_cast<unsigned char>(c) & 0xC0U) == 0x80U;
}

constexpr bool is_horizontal_space(char c) {
  return c == ' ' || c == '\t' || c == '\v' || c == '\f';
}

constexpr bool is_line_break(char c) {
  return c == '\n' || c == '\r';
}

}  // namespace

Lexer::Lexer(std::string source) : source_{std::move(source)} {}

application::ports::LexerResult Lexer::next() {
  if (const auto err = skip_trivia()) {
    return *err;
  }

  const auto start = pos_;

  if (at_end()) {
    return make_token(domain::TokenKind::end_of_file, start);
  }

  const char c = peek();

  if (!is_ascii(c)) {
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
      if (match('&')) {
        return make_token(domain::TokenKind::op_and, start);
      }
      return make_error(domain::LexicalErrorKind::invalid_character, start);
    case '|':
      if (match('|')) {
        return make_token(domain::TokenKind::op_or, start);
      }
      return make_error(domain::LexicalErrorKind::invalid_character, start);
    case '(':
      return make_token(domain::TokenKind::left_paren, start);
    case ')':
      return make_token(domain::TokenKind::right_paren, start);
    case ',':
      return make_token(domain::TokenKind::comma, start);
    case ':':
      return make_token(domain::TokenKind::colon, start);
    case ';':
      return make_token(domain::TokenKind::semicolon, start);
    case '.':
      return make_token(domain::TokenKind::dot, start);
    case '\'':
      return scan_char_const(start);
    case '"':
      return scan_string(start);
    default:
      return make_error(domain::LexicalErrorKind::invalid_character, start);
  }
}

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
  while (!at_end() && is_utf8_continuation(peek())) {
    advance();
  }
}

std::optional<domain::LexicalError> Lexer::skip_trivia() {
  while (!at_end()) {
    if (is_horizontal_space(peek()) || is_line_break(peek())) {
      advance();
      continue;
    }

    if (peek() != '{' || peek_next() != '*') {
      return std::nullopt;
    }

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
  }

  return std::nullopt;
}

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

  auto kind = domain::TokenKind::integer_const;
  if (peek() == '.' && domain::is_ascii_digit(peek_next())) {
    kind = domain::TokenKind::float_const;
    advance();  // '.'
    while (domain::is_ascii_digit(peek())) {
      advance();
    }
  }

  if (domain::is_identifier_start(peek())) {
    while (domain::is_identifier_part(peek())) {
      advance();
    }
    return make_error(domain::LexicalErrorKind::identifier_starts_with_digit, start);
  }

  return make_token(kind, start);
}

application::ports::LexerResult Lexer::scan_char_const(domain::SourceLocation start) {
  if (at_end() || is_line_break(peek())) {
    return make_error(domain::LexicalErrorKind::unterminated_char_const, start);
  }

  if (peek() == '\'') {
    advance();
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  if (!is_ascii(peek())) {
    skip_invalid_utf8();
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  advance();  // consume the character

  if (match('\'')) {
    return make_token(domain::TokenKind::char_const, start);
  }

  while (!at_end() && !is_line_break(peek()) && peek() != '\'') {
    advance();
  }

  if (match('\'')) {
    return make_error(domain::LexicalErrorKind::invalid_char_const, start);
  }

  return make_error(domain::LexicalErrorKind::unterminated_char_const, start);
}

application::ports::LexerResult Lexer::scan_string(domain::SourceLocation start) {
  while (!at_end() && !is_line_break(peek()) && peek() != '"') {
    if (!is_ascii(peek())) {
      const auto err_start = pos_;
      skip_invalid_utf8();
      return make_error(domain::LexicalErrorKind::invalid_character, err_start);
    }
    advance();
  }

  if (at_end() || is_line_break(peek())) {
    return make_error(domain::LexicalErrorKind::unterminated_string, start);
  }

  advance();  // closing '"'
  return make_token(domain::TokenKind::string_literal, start);
}

domain::Token Lexer::make_token(domain::TokenKind kind, domain::SourceLocation start) const {
  return domain::Token{kind, source_.substr(start.offset, pos_.offset - start.offset), start};
}

domain::LexicalError Lexer::make_error(domain::LexicalErrorKind kind, domain::SourceLocation start) const {
  return domain::LexicalError{kind, source_.substr(start.offset, pos_.offset - start.offset), start};
}

}  // namespace darthex::adapters
