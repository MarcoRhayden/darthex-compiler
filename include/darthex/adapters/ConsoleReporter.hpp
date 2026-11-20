#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token.hpp"

namespace darthex::adapters {

class ConsoleReporter {
 public:
  ConsoleReporter(std::ostream& out, std::ostream& err);

  void print_tokens(const std::vector<domain::Token>& tokens);
  void print_errors(const std::vector<domain::LexicalError>& errors);
  void print_symbols(const std::vector<std::string>& symbols);

 private:
  std::ostream& out_;
  std::ostream& err_;
};

}  // namespace darthex::adapters
