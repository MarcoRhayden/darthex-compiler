#pragma once

#include <ostream>
#include <vector>

#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/syntax_error.hpp"

namespace darthex::adapters {

class ConsoleReporter {
 public:
  ConsoleReporter(std::ostream& out, std::ostream& err);

  void print_lexical_errors(const std::vector<domain::LexicalError>& errors);
  void print_syntax_error(const domain::SyntaxError& error);
  void print_success();

 private:
  std::ostream& out_;
  std::ostream& err_;
};

}  // namespace darthex::adapters
