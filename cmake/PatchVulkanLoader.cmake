# Patch script for Vulkan-Loader to fix the assembler check with clang-cl

if(NOT LOADER_CMAKE)
    message(FATAL_ERROR "LOADER_CMAKE variable not set")
endif()

if(EXISTS "${LOADER_CMAKE}")
    message(STATUS "Patching ${LOADER_CMAKE} to bypass assembler check...")

    file(READ "${LOADER_CMAKE}" LOADER_CONTENT)

    # Replace the condition that checks if ASM compiler works
    # Original: if(ASM_COMPILER_WORKS EQUAL 0)
    # Patched:  if(TRUE)  # Force success
    string(REPLACE "if(ASM_COMPILER_WORKS EQUAL 0)" "if(TRUE)" LOADER_CONTENT "${LOADER_CONTENT}")

    file(WRITE "${LOADER_CMAKE}" "${LOADER_CONTENT}")

    message(STATUS "Vulkan-Loader patched successfully")
else()
    message(WARNING "Could not find ${LOADER_CMAKE} to patch")
endif()

