#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "darthex/application/ports/ISymbolTable.hpp"

namespace darthex::adapters {

class SymbolTable : public application::ports::ISymbolTable {
 public:
  SymbolTable() = default;
  ~SymbolTable() override = default;

  application::ports::SymbolId install(std::string lexeme) override;
  std::optional<application::ports::SymbolId> find(std::string_view lexeme) const override;

  std::size_t size() const;
  const std::vector<std::string>& symbols() const;

 private:
  std::unordered_map<std::string, application::ports::SymbolId> index_;
  std::vector<std::string> symbols_;
};

}  // namespace darthex::adapters
