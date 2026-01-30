cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Sandbox-----]")

set(HEADER_FILES
)

set(SOURCE_FILES
    src/ImEngine-Sandbox.cpp
)

add_executable(ImEngine-Sandbox ${HEADER_FILES} ${SOURCE_FILES})

set_project_warnings(ImEngine-Sandbox)
enable_security_flags(ImEngine-Sandbox)

target_include_directories(ImEngine-Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ImEngine-Sandbox
    PRIVATE ImEngine::Engine
    # Vulkan is now transitive via Engine PUBLIC
)

add_custom_command(TARGET ImEngine-Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:ImEngine-Sandbox>
        $<TARGET_FILE_DIR:ImEngine-Sandbox>
    COMMAND_EXPAND_LISTS
)

install(TARGETS ImEngine-Sandbox
    RUNTIME DESTINATION bin
)
