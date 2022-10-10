set(EXAMPLE_NAME ${PROJECT_NAME}Example)

add_executable(${EXAMPLE_NAME} main.c)

target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME})

target_include_directories(${EXAMPLE_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")