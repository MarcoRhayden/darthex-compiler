#pragma once

#include <variant>

#include "darthex/domain/lexical_error.hpp"
#include "darthex/domain/token.hpp"

namespace darthex::application::ports {

using LexerResult = std::variant<domain::Token, domain::LexicalError>;

struct ILexer {
    virtual ~ILexer() = default;
    virtual LexerResult next() = 0;
};

}  // namespace darthex::application::ports
