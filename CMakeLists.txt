cmake_minimum_required(VERSION 3.29)
project(ImEngine)

add_executable(ImEngine ImEngine/main.cpp)

add_subdirectory(ImEngine/ThirdParty)
target_link_libraries(ImEngine PUBLIC ImEngine-ThirdParty)

add_subdirectory(ImEngine/Core)
target_link_libraries(ImEngine PUBLIC ImEngine-Core)

add_custom_command(TARGET ImEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
)
