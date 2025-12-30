# Set C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Unified output directories
foreach(kind RUNTIME LIBRARY ARCHIVE PDB)
  set(CMAKE_${kind}_OUTPUT_DIRECTORY
      "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${kind}" CACHE PATH "" FORCE)
endforeach()

# Use folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

