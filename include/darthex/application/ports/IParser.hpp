#pragma once

#include <vector>

#include "darthex/domain/token.hpp"

namespace darthex::application::ports {

struct IParser {
    virtual ~IParser() = default;
};

}  // namespace darthex::application::ports
