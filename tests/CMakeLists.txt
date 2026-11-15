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
)
target_link_libraries(darthex_tests PRIVATE Catch2::Catch2WithMain darthex_domain)
set_target_properties(darthex_tests PROPERTIES CXX_EXTENSIONS OFF)
darthex_enable_warnings(darthex_tests)
