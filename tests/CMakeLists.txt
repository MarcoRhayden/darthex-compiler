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
  acceptance/stage_two_test.cpp
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

foreach(fixture IN ITEMS test1 test2 test3 test4 test5 extra1 extra2)
  add_test(
    NAME parser_cli_valid_${fixture}
    COMMAND darthexc "${CMAKE_CURRENT_SOURCE_DIR}/fixtures/parser/valid/${fixture}.dex"
  )
  set_tests_properties(parser_cli_valid_${fixture} PROPERTIES
    PASS_REGULAR_EXPRESSION "Syntax analysis completed successfully."
  )
endforeach()

foreach(fixture IN ITEMS test1 test2 test3 test4 test5)
  add_test(
    NAME parser_cli_invalid_${fixture}
    COMMAND darthexc "${CMAKE_CURRENT_SOURCE_DIR}/fixtures/parser/invalid/${fixture}.dex"
  )
  set_tests_properties(parser_cli_invalid_${fixture} PROPERTIES WILL_FAIL TRUE)
endforeach()
