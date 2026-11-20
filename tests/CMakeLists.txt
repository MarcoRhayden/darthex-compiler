include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(darthex_tests
  unit/domain/source_location_test.cpp
  unit/domain/token_kind_test.cpp
  unit/domain/token_test.cpp
  unit/domain/lexical_error_test.cpp
  unit/domain/syntax_error_test.cpp
  unit/domain/identifier_test.cpp
  unit/domain/keyword_test.cpp
  unit/domain/char_class_test.cpp
  unit/adapters/lexer_test.cpp
  unit/adapters/parser_test.cpp
  unit/adapters/console_reporter_test.cpp
  unit/adapters/symbol_table_test.cpp
  unit/application/ports/parser_port_test.cpp
  unit/application/services/lex_service_test.cpp
  acceptance/stage_one_test.cpp
)
target_compile_definitions(darthex_tests PRIVATE
  FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
)
target_link_libraries(darthex_tests PRIVATE Catch2::Catch2WithMain darthex_adapters darthex_application)
set_target_properties(darthex_tests PROPERTIES CXX_EXTENSIONS OFF)
darthex_enable_warnings(darthex_tests)

list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(darthex_tests)
