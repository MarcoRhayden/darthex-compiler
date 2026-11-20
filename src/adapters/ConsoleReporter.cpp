#include "darthex/adapters/ConsoleReporter.hpp"

#include <iomanip>

#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::adapters {

ConsoleReporter::ConsoleReporter(std::ostream& out, std::ostream& err)
    : out_{out}, err_{err} {}

void ConsoleReporter::print_lexical_errors(
    const std::vector<domain::LexicalError>& errors) {
  for (const auto& e : errors) {
    err_ << std::setw(4) << e.location.line
         << ':'
         << std::left << std::setw(4) << e.location.column
         << std::resetiosflags(std::ios::adjustfield)
         << "  error  "
         << std::left << std::setw(30) << domain::lexical_error_kind_name(e.kind)
         << std::resetiosflags(std::ios::adjustfield)
         << "  \"" << e.lexeme << "\"\n";
  }
}

void ConsoleReporter::print_syntax_error(const domain::SyntaxError& error) {
  const auto& token = error.unexpected;

  err_ << token.location.line << ':' << token.location.column
       << "  syntax error: expected " << error.expected << ", found ";

  if (token.kind == domain::TokenKind::end_of_file) {
    err_ << "end of file";
  } else {
    err_ << '"' << token.lexeme << '"';
  }
  err_ << '\n';
}

void ConsoleReporter::print_success() {
  out_ << "Syntax analysis completed successfully.\n";
}

}  // namespace darthex::adapters
