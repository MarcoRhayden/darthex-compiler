#pragma once

#include <optional>
#include <string_view>

#include "darthex/domain/token_kind.hpp"

namespace darthex::domain {

// Returns the TokenKind for a reserved word, or nullopt if lexeme is not one.
std::optional<TokenKind> keyword_kind(std::string_view lexeme);

}  // namespace darthex::domain
