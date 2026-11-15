#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace darthex::application::ports {

using SymbolId = std::size_t;

struct ISymbolTable {
    virtual ~ISymbolTable() = default;

    // Installs a new identifier or returns the id of the existing entry.
    virtual SymbolId install(std::string lexeme) = 0;

    // Returns the id of an existing entry, or nullopt if not found.
    virtual std::optional<SymbolId> find(std::string_view lexeme) const = 0;
};

}  // namespace darthex::application::ports
