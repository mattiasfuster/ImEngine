function(enable_security_flags project_name)
  if(MSVC)
    # /GS: Buffer Security Check
    # /guard:cf: Control Flow Guard
    # /SDL: Security Development Lifecycle checks

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # clang-cl: /SDL might not be supported or cause issues
        target_compile_options(${project_name} INTERFACE /GS /guard:cf)
    else()
        target_compile_options(${project_name} INTERFACE /GS /guard:cf /SDL)
    endif()

    # Linker flags for security
    # /DYNAMICBASE: ASLR
    # /NXCOMPAT: DEP
    target_link_options(${project_name} INTERFACE /DYNAMICBASE /NXCOMPAT /GUARD:CF)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # -fstack-protector-strong: Stack smashing protection
    # -D_FORTIFY_SOURCE=2: Buffer overflow detection
    # -fPIE: Position Independent Executable (for ASLR)
    target_compile_options(${project_name} INTERFACE
      -fstack-protector-strong
      -fPIE
    )

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES ".*Clang-CL")
        target_compile_definitions(${project_name} INTERFACE _FORTIFY_SOURCE=2)
    endif()

    target_link_options(${project_name} INTERFACE -Wl,-z,relro -Wl,-z,now)
  endif()
endfunction()

