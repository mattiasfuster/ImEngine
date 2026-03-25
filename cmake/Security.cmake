function(enable_security_flags project_name)
  if(MSVC)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${project_name} PRIVATE /GS /guard:cf)
    else()
        target_compile_options(${project_name} PRIVATE /GS /guard:cf /SDL)
    endif()
    target_link_options(${project_name} PRIVATE /DYNAMICBASE /NXCOMPAT /GUARD:CF)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${project_name} PRIVATE
      -fstack-protector-strong
      -fPIE
    )
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES ".*Clang-CL")
        target_compile_definitions(${project_name} PRIVATE _FORTIFY_SOURCE=2)
    endif()

    target_link_options(${project_name} PRIVATE -Wl,-z,relro -Wl,-z,now)
  endif()
endfunction()

