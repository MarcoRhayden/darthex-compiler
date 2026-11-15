#include <catch2/catch_test_macros.hpp>

#include "darthex/domain/source_location.hpp"

using darthex::domain::SourceLocation;

TEST_CASE("SourceLocation default value is line 1, column 1, offset 0") {
  SourceLocation loc;
  CHECK(loc.line == 1);
  CHECK(loc.column == 1);
  CHECK(loc.offset == 0);
}

TEST_CASE("SourceLocation equality") {
  SourceLocation a{2, 5, 10};
  SourceLocation b{2, 5, 10};
  SourceLocation c{3, 1, 11};

  CHECK(a == b);
  CHECK(a != c);
}
