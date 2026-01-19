# Compile Vulkan shaders (GLSL to SPIR-V) and copy to runtime directories

cmake_minimum_required(VERSION 3.29)

# Find glslc from Vulkan SDK
find_program(Vulkan_GLSLC_EXECUTABLE NAMES glslc HINTS ENV VULKAN_SDK PATH_SUFFIXES Bin bin)
if(NOT Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
endif()

# Find all .vert and .frag files in this directory
file(GLOB SHADER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

# Compile to intermediate directory
set(SHADER_INTERMEDIATE_DIR "${CMAKE_CURRENT_BINARY_DIR}/compiled")
file(MAKE_DIRECTORY ${SHADER_INTERMEDIATE_DIR})

set(COMPILED_SHADERS)
set(ALL_SHADER_OUTPUTS)

# For each shader source file
foreach(SHADER_FILE ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(SPIRV_FILE "${SHADER_INTERMEDIATE_DIR}/${SHADER_NAME}.spv")

    # Compile shader to intermediate directory
    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_INTERMEDIATE_DIR}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} -o ${SPIRV_FILE} ${SHADER_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling shader: ${SHADER_NAME}"
    )

    list(APPEND COMPILED_SHADERS ${SPIRV_FILE})

    # Copy to each configuration's RUNTIME/shaders directory
    foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
        set(RUNTIME_SHADER_DIR "${CMAKE_BINARY_DIR}/bin/${CONFIG}/RUNTIME/shaders")
        set(OUTPUT_SHADER "${RUNTIME_SHADER_DIR}/${SHADER_NAME}.spv")

        add_custom_command(
            OUTPUT ${OUTPUT_SHADER}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_SHADER_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SPIRV_FILE} ${OUTPUT_SHADER}
            DEPENDS ${SPIRV_FILE}
            COMMENT "Copying ${SHADER_NAME} to ${CONFIG} runtime directory"
        )

        list(APPEND ALL_SHADER_OUTPUTS ${OUTPUT_SHADER})
    endforeach()
endforeach()

# Target depends on all compiled and copied shaders
add_custom_target(ImEngine-Shaders ALL
    DEPENDS ${COMPILED_SHADERS} ${ALL_SHADER_OUTPUTS}
)

# Install rules for SDK distribution (uses compiled shaders from intermediate dir)
install(FILES ${COMPILED_SHADERS}
    DESTINATION "bin/shaders"
)
