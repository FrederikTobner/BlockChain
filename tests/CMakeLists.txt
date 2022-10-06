# deletes build folder to ensure there are no makefiles that make the build fail (multiple target patterns 😨)
if( MSVC )
execute_process(COMMAND ${CMAKE_COMMAND} "-E" "remove_directory" ${PROJECT_BINARY_DIR}/src)
endif()

#C++ 14 needed for Google Test Framework
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_PROJ_NAME BlockchainTests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(SOURCES
sha256_test.cc
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(${TEST_PROJ_NAME} ${SOURCES})

target_link_libraries(${TEST_PROJ_NAME} GTest::gtest_main ${PROJECT_NAME})

target_include_directories(${TEST_PROJ_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(GoogleTest)
gtest_discover_tests(${TEST_PROJ_NAME})