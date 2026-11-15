#include <catch2/catch_test_macros.hpp>

#include "darthex/adapters/SymbolTable.hpp"

using darthex::adapters::SymbolTable;

TEST_CASE("SymbolTable starts empty") {
  SymbolTable st;
  CHECK(st.size() == 0);
  CHECK_FALSE(st.find("x").has_value());
}

TEST_CASE("install returns sequential ids starting at zero") {
  SymbolTable st;
  CHECK(st.install("alpha") == 0);
  CHECK(st.install("beta") == 1);
  CHECK(st.install("gamma") == 2);
  CHECK(st.size() == 3);
}

TEST_CASE("install is idempotent - same lexeme returns same id") {
  SymbolTable st;
  const auto id1 = st.install("foo");
  const auto id2 = st.install("foo");
  CHECK(id1 == id2);
  CHECK(st.size() == 1);
}

TEST_CASE("find returns nullopt for unknown lexeme") {
  SymbolTable st;
  st.install("known");
  CHECK_FALSE(st.find("unknown").has_value());
}

TEST_CASE("find returns the correct id after install") {
  SymbolTable st;
  const auto id = st.install("hello");
  const auto found = st.find("hello");
  REQUIRE(found.has_value());
  CHECK(*found == id);
}

TEST_CASE("find works with string_view") {
  SymbolTable st;
  st.install("view");
  std::string_view sv = "view";
  REQUIRE(st.find(sv).has_value());
  CHECK(*st.find(sv) == 0);
}

TEST_CASE("install and find are case-sensitive") {
  SymbolTable st;
  const auto id_lower = st.install("abc");
  const auto id_upper = st.install("ABC");
  CHECK(id_lower != id_upper);
  CHECK(st.size() == 2);
  CHECK(*st.find("abc") == id_lower);
  CHECK(*st.find("ABC") == id_upper);
}

TEST_CASE("multiple distinct identifiers all resolve correctly") {
  SymbolTable st;
  const std::vector<std::string> names = {"x", "y", "count", "total", "result"};
  for (const auto& name : names) {
    st.install(name);
  }
  CHECK(st.size() == names.size());
  for (std::size_t i = 0; i < names.size(); ++i) {
    REQUIRE(st.find(names[i]).has_value());
    CHECK(*st.find(names[i]) == i);
  }
}
