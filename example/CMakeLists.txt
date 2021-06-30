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
    LS_SUB_DIR(EXAMPLES ${CMAKE_CURRENT_LIST_DIR})
    foreach(EXAMPLE ${EXAMPLES})
        message(STATUS "building example: ${EXAMPLE}")
        add_executable(${EXAMPLE} ${CMAKE_CURRENT_LIST_DIR}/${EXAMPLE}/main.cc)
        target_include_directories(${EXAMPLE} PRIVATE
          ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.4/include)
        target_link_libraries(${EXAMPLE} PRIVATE base glfw)
        # 'copy' shaders to corresponding build directory
        file(GLOB_RECURSE shaders *.vert *.frag *.geom)
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
