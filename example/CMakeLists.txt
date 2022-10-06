set(THIS Example)

add_executable(${THIS} main.c)

target_link_libraries(${THIS}
${PROJECT_NAME}
)

target_include_directories(${THIS} PUBLIC "${PROJECT_SOURCE_DIR}/src")