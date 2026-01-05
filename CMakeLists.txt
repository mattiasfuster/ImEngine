cmake_minimum_required(VERSION 3.29)
project(ImEngine LANGUAGES C CXX)

message(STATUS "[-----ImEngine-----]")

# ---- Module Path ------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---- Global options (robust defaults) ---------------------------------------
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
  # Use the dynamic runtime (/MD or /MDd)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()


# ---- Subdirectories ----------------------------------------------------------
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