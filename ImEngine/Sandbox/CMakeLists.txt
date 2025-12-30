cmake_minimum_required(VERSION 3.29)
project(ImEngine-Sandbox)

message(STATUS "[-----ImEngine-Sandbox-----]")

set(HEADER_FILES
)
set(SOURCE_FILES
    src/ImEngine-Sandbox.cpp
)

add_executable(ImEngine-Sandbox ${HEADER_FILES} ${SOURCE_FILES})

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Sandbox)
enable_security_flags(ImEngine-Sandbox)

target_include_directories(ImEngine-Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ImEngine-Sandbox
    PRIVATE ImEngine::Engine
    PRIVATE ImEngine::Core
    PRIVATE ImEngine-ThirdParty::glfw
    PRIVATE ImEngine-ThirdParty::vulkan
)

install(TARGETS ImEngine-Sandbox
    RUNTIME DESTINATION bin
)
