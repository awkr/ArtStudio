file(GLOB_RECURSE SOURCE_FILES *.h *.cc)

add_library(base ${SOURCE_FILES})
target_include_directories(base PUBLIC
    .
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/stb_image)
target_link_libraries(base PUBLIC glad)
