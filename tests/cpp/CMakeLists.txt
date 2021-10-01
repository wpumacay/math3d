# ~~~
# CMake configuration for C++ unittests
# ~~~
if(NOT TINYMATH_BUILD_TESTS)
  return()
endif()

if((NOT TARGET tinymath::tinymath) OR (NOT TARGET Catch2::Catch2))
  tmMessage(
    "Unittests require both tinymath::tinymath and Catch2::Catch2 targets"
    LOG_LEVEL WARNING)
  return()
endif()

include(CTest)
include(${CMAKE_SOURCE_DIR}/third_party/catch2/source/contrib/Catch.cmake)

add_executable(
  TinyMathCppTests
  ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_vec3_type.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_vec3_operations.cpp)
target_link_libraries(TinyMathCppTests PRIVATE tinymath::tinymath
                                               Catch2::Catch2)
catch_discover_tests(TinyMathCppTests)