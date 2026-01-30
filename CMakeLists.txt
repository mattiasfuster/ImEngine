cmake_minimum_required(VERSION 3.29)
project(ImEngine
    VERSION 0.1.0
    DESCRIPTION "Modern C++23 game engine with Vulkan rendering"
    HOMEPAGE_URL "https://github.com/mattiasfuster/ImEngine"
    LANGUAGES C CXX
)

message(STATUS "[-----ImEngine-----]")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(IMENGINE_VULKAN_VERSION "1.3.296" CACHE STRING "Vulkan API version")
set(IMENGINE_VULKAN_TAG "v${IMENGINE_VULKAN_VERSION}" CACHE STRING "Vulkan SDK tag")

option(IMENGINE_BUILD_CORE    "Build ImEngine Core"    ON)
option(IMENGINE_BUILD_ENGINE  "Build ImEngine Engine"  ON)
option(IMENGINE_BUILD_SANDBOX "Build ImEngine Sandbox" OFF)
option(IMENGINE_BUILD_EDITOR  "Build ImEngine Editor"  OFF)

include(StandardSettings)
include(Cache)
include(CompilerWarnings)
include(Security)

enable_ccache()

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()


add_subdirectory(ImEngine/ThirdParty)

if(IMENGINE_BUILD_CORE)
  add_subdirectory(ImEngine/Core)
endif()

if(IMENGINE_BUILD_ENGINE)
  add_subdirectory(ImEngine/Engine)
endif()

if(IMENGINE_BUILD_SANDBOX)
  add_subdirectory(ImEngine/Sandbox)
endif()

if(IMENGINE_BUILD_EDITOR)
  add_subdirectory(ImEngine/Editor)
endif()