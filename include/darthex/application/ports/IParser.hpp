#pragma once

#include <optional>
#include <vector>

#include "darthex/domain/syntax_error.hpp"
#include "darthex/domain/token.hpp"

namespace darthex::application::ports {

using ParseResult = std::optional<domain::SyntaxError>;

struct IParser {
  virtual ~IParser() = default;
  virtual ParseResult parse(const std::vector<domain::Token>& tokens) = 0;
};

}  // namespace darthex::application::ports
