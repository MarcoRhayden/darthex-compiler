#include "darthex/adapters/SymbolTable.hpp"

namespace darthex::adapters {

application::ports::SymbolId SymbolTable::install(std::string lexeme) {
  auto it = index_.find(lexeme);
  if (it != index_.end()) {
    return it->second;
  }
  const application::ports::SymbolId id = symbols_.size();
  index_.emplace(lexeme, id);
  symbols_.push_back(std::move(lexeme));
  return id;
}

std::optional<application::ports::SymbolId> SymbolTable::find(std::string_view lexeme) const {
  auto it = index_.find(std::string{lexeme});
  if (it == index_.end()) {
    return std::nullopt;
  }
  return it->second;
}

std::size_t SymbolTable::size() const {
  return symbols_.size();
}

const std::vector<std::string>& SymbolTable::symbols() const {
  return symbols_;
}

}  // namespace darthex::adapters
