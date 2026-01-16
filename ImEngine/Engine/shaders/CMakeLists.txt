# Install compiled SPIR-V shaders to bin/shaders
install(FILES ${COMPILED_SHADERS}
    DESTINATION "bin/shaders"
)
# Compile Vulkan shaders (GLSL to SPIR-V)
# This CMakeLists.txt is responsible for compiling all shaders in this directory

cmake_minimum_required(VERSION 3.29)

set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/compiled")
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

# Find all .vert and .frag files in this directory
file(GLOB SHADER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

# Function to compile a shader
function(compile_shader SHADER_FILE)
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(SPIRV_FILE "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} -o ${SPIRV_FILE} ${SHADER_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling shader: ${SHADER_NAME}"
    )
    set(COMPILED_SHADERS ${COMPILED_SHADERS} ${SPIRV_FILE} PARENT_SCOPE)
endfunction()

# Find glslc from Vulkan SDK
find_program(Vulkan_GLSLC_EXECUTABLE NAMES glslc HINTS ENV VULKAN_SDK PATH_SUFFIXES Bin bin)
if(NOT Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
endif()

set(COMPILED_SHADERS)
foreach(SHADER_FILE ${SHADER_SOURCES})
    compile_shader(${SHADER_FILE})
endforeach()


add_custom_target(ImEngine-Shaders ALL DEPENDS ${COMPILED_SHADERS})

install(FILES ${COMPILED_SHADERS}
    DESTINATION "bin/shaders"
)
