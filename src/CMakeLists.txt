file(GLOB_RECURSE SOURCE_FILES *.h *.cc)

add_library(base ${SOURCE_FILES})
target_include_directories(base PUBLIC .)
target_link_libraries(base PUBLIC glad)
