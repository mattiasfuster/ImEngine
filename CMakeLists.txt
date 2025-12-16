cmake_minimum_required(VERSION 3.29)
project(ImEngine LANGUAGES C CXX)

message(STATUS "[-----ImEngine-----]")

# ---- Global options (robust defaults) ---------------------------------------
option(IMENGINE_BUILD_ENGINE  "Build ImEngine Engine"  ON)
option(IMENGINE_BUILD_SANDBOX "Build ImEngine Sandbox" OFF)
option(IMENGINE_BUILD_EDITOR  "Build ImEngine Editor"  OFF)

# ---- Compiler & CRT setup ----------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  # Use the dynamic runtime (/MD or /MDd)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# ---- Unified output directories per-config ----------------------------------
# e.g. out/build/<preset>/<Config>/{runtime,library,archive,pdb}
foreach(kind RUNTIME LIBRARY ARCHIVE PDB)
  set(CMAKE_${kind}_OUTPUT_DIRECTORY
      "${CMAKE_BINARY_DIR}/$<CONFIG>/${kind}" CACHE PATH "" FORCE)
endforeach()

# ---- Subdirectories ----------------------------------------------------------
add_subdirectory(ImEngine/ThirdParty)

if(IMENGINE_BUILD_ENGINE)
  add_subdirectory(ImEngine/Engine)
endif()

if(IMENGINE_BUILD_SANDBOX)
add_subdirectory(ImEngine/Sandbox)
endif()

if(IMENGINE_BUILD_EDITOR)
add_subdirectory(ImEngine/Editor)
endif()