MACRO(LS_SUB_DIR result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

function(buildExamples)
    # LS_SUB_DIR(EXAMPLES ${CMAKE_CURRENT_LIST_DIR})
    set(EXAMPLES Camera)

    foreach(EXAMPLE ${EXAMPLES})
        message(STATUS "building example: ${EXAMPLE}")

        file(GLOB_RECURSE headers ${CMAKE_CURRENT_LIST_DIR}/${EXAMPLE}/*.h)
        file(GLOB_RECURSE sources ${CMAKE_CURRENT_LIST_DIR}/${EXAMPLE}/*.cc)
        add_executable(${EXAMPLE} ${headers} ${sources})

        target_include_directories(${EXAMPLE} PRIVATE
          ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.4/include)
        target_link_libraries(${EXAMPLE} PRIVATE base glfw)

        set_target_properties(${EXAMPLE}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}"
        )

        # 'copy' shaders to corresponding build directory
        file(GLOB_RECURSE shaders ${EXAMPLE}/*.vert ${EXAMPLE}/*.frag ${EXAMPLE}/*.geom)
        set(dest_dir "$<TARGET_FILE_DIR:${EXAMPLE}>")
        foreach(shader ${shaders})
            get_filename_component(filename ${shader} NAME)
            add_custom_command(TARGET ${EXAMPLE} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${shader} ${dest_dir}/${filename}
            )
        endforeach()
    endforeach(EXAMPLE)
endfunction(buildExamples)

buildExamples()
