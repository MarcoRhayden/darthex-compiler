#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "darthex/adapters/ConsoleReporter.hpp"
#include "darthex/adapters/Lexer.hpp"
#include "darthex/adapters/SymbolTable.hpp"
#include "darthex/application/services/LexService.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: darthexc <source-file>\n";
    return 1;
  }

  std::ifstream file{argv[1]};
  if (!file) {
    std::cerr << "error: cannot open '" << argv[1] << "'\n";
    return 1;
  }

  const std::string source{std::istreambuf_iterator<char>{file},
                            std::istreambuf_iterator<char>{}};

  darthex::adapters::Lexer lexer{source};
  darthex::adapters::SymbolTable symbols;
  darthex::application::services::LexService service{lexer, symbols};
  service.run();

  darthex::adapters::ConsoleReporter reporter{std::cout, std::cerr};
  reporter.print_tokens(service.tokens());
  reporter.print_errors(service.errors());

  return service.has_errors() ? 1 : 0;
}
