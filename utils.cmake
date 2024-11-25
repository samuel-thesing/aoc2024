#
# CMAKE UTILITY FUNCTIONS
#

function(target_set_output_dir target)
    set_target_properties(${target} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ARTEFACT_DIR}/${target}/${CMAKE_BUILD_TYPE}/"
            LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ARTEFACT_DIR}/${target}/${CMAKE_BUILD_TYPE}/"
            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ARTEFACT_DIR}/${target}/${CMAKE_BUILD_TYPE}")
endfunction()

function(target_link_dependency target type name)
    if (NOT type STREQUAL SHARED AND
            NOT type STREQUAL STATIC AND
            NOT type STREQUAL SOURCE AND
            NOT type STREQUAL HEADER)
        message( FATAL_ERROR "Bad value for argument type. Can be SHARED, STATIC, SOURCE or HEADER." )
    endif ()

    if (type STREQUAL HEADER)
        target_include_directories(${target} PRIVATE "${PROJECT_DEPENDENCY_DIR}/${name}/include")
    elseif (NOT type STREQUAL SOURCE)
        target_include_directories(${target} PRIVATE "${PROJECT_DEPENDENCY_DIR}/${name}/include")
        target_link_directories(${target} PRIVATE "${PROJECT_DEPENDENCY_DIR}/${name}/lib")
        target_link_libraries(${target} ${name})

        if (type STREQUAL SHARED)
            add_custom_command(TARGET ${target}
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${PROJECT_DEPENDENCY_DIR}/${name}/lib/${name}.dll"
                    "$<TARGET_FILE_DIR:${target}>"
                    COMMENT "Copying files for ${target}...")
        endif ()
    else ()
        file(GLOB deps_${name}_sources CONFIGURE_DEPENDS
                "${PROJECT_DEPENDENCY_DIR}/${name}/src/*.cpp"
                "${PROJECT_DEPENDENCY_DIR}/${name}/src/*.c")

        target_include_directories(${target} PRIVATE "${PROJECT_DEPENDENCY_DIR}/${name}/include")
        target_sources(${target} PRIVATE ${deps_${name}_sources})
    endif ()
endfunction()


function(add_custom_cmake_command target command_name command)
    file(WRITE "${CMAKE_BINARY_DIR}/${command_name}.cmake" "${command}")

    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/${command_name}.cmake"
            COMMENT "Executing command ${command_name}...")
endfunction()

if (OPTS_LINK_STDLIB)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
    add_compile_options(-static-libgcc -static-libstdc++)
endif ()