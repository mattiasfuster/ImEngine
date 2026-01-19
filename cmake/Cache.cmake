function(enable_ccache)
  find_program(CCACHE_PROGRAM ccache)

  if(NOT CCACHE_PROGRAM AND WIN32)
    set(CCACHE_VERSION "4.12.2")
    set(CCACHE_FILENAME "ccache-${CCACHE_VERSION}-windows-x86_64")
    set(CCACHE_URL "https://github.com/ccache/ccache/releases/download/v${CCACHE_VERSION}/${CCACHE_FILENAME}.zip")

    set(CCACHE_DL_DIR "${CMAKE_BINARY_DIR}/tools/ccache")
    set(CCACHE_EXE "${CCACHE_DL_DIR}/${CCACHE_FILENAME}/ccache.exe")

    if(NOT EXISTS "${CCACHE_EXE}")
      message(STATUS "ccache not found. Downloading portable version ${CCACHE_VERSION}...")
      file(MAKE_DIRECTORY "${CCACHE_DL_DIR}")

      set(CCACHE_ZIP "${CCACHE_DL_DIR}/ccache.zip")
      file(DOWNLOAD "${CCACHE_URL}" "${CCACHE_ZIP}" SHOW_PROGRESS)

      message(STATUS "Extracting ccache...")
      file(ARCHIVE_EXTRACT INPUT "${CCACHE_ZIP}" DESTINATION "${CCACHE_DL_DIR}")

      # Cleanup zip
      file(REMOVE "${CCACHE_ZIP}")
    endif()

    if(EXISTS "${CCACHE_EXE}")
      set(CCACHE_PROGRAM "${CCACHE_EXE}")
      message(STATUS "Downloaded ccache: ${CCACHE_PROGRAM}")
    endif()
  endif()

  if(CCACHE_PROGRAM)
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" CACHE STRING "C compiler launcher" FORCE)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" CACHE STRING "CXX compiler launcher" FORCE)

    if(MSVC)
      set(ENV{CCACHE_COMPILERCHECK} "content")
      message(STATUS "ccache: Configured for MSVC with content-based compiler check")
    endif()
  else()
    message(WARNING "ccache not found. Build performance might be affected.")
  endif()
endfunction()

