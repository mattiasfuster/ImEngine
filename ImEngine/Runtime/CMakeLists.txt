cmake_minimum_required(VERSION 3.29)

message(STATUS "[-----ImEngine-Runtime-----]")

set(RUNTIME_SOURCES
    src/main.cpp
)

add_executable(ImEngine-Runtime
    ${RUNTIME_SOURCES}
)

# --- Apply Modern Settings ---
set_project_warnings(ImEngine-Runtime)
enable_security_flags(ImEngine-Runtime)

target_link_libraries(ImEngine-Runtime
    PRIVATE ImEngine::Engine
    PRIVATE ImEngine::Core
)

install(TARGETS ImEngine-Runtime
    RUNTIME DESTINATION bin
)

