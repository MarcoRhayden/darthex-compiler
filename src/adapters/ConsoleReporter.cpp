#include "darthex/adapters/ConsoleReporter.hpp"

#include <iomanip>

#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token_kind.hpp"

namespace darthex::adapters {

ConsoleReporter::ConsoleReporter(std::ostream& out, std::ostream& err)
    : out_{out}, err_{err} {}

void ConsoleReporter::print_tokens(const std::vector<domain::Token>& tokens) {
  for (const auto& t : tokens) {
    out_ << std::setw(4) << t.location.line
         << ':'
         << std::left << std::setw(4) << t.location.column
         << std::resetiosflags(std::ios::adjustfield)
         << "  "
         << std::left << std::setw(20) << domain::token_kind_name(t.kind)
         << std::resetiosflags(std::ios::adjustfield)
         << "  \"" << t.lexeme << "\"\n";
  }
}

void ConsoleReporter::print_errors(const std::vector<domain::LexicalError>& errors) {
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

void ConsoleReporter::print_symbols(const std::vector<std::string>& symbols) {
  out_ << "\nSymbol table (" << symbols.size() << " entries):\n";
  for (std::size_t i = 0; i < symbols.size(); ++i) {
    out_ << std::setw(4) << i << "  \"" << symbols[i] << "\"\n";
  }
}

}  // namespace darthex::adapters
