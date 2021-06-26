set(EXAMPLES
    1-HelloWorld
)

function(buildExamples)
    foreach(EXAMPLE ${EXAMPLES})
        message(STATUS "building example: ${EXAMPLE}")
        add_executable(${EXAMPLE} ${CMAKE_CURRENT_LIST_DIR}/${EXAMPLE}/main.cc)
        target_link_libraries(${EXAMPLE} PRIVATE base glfw)
    endforeach(EXAMPLE)
endfunction(buildExamples)

buildExamples()
