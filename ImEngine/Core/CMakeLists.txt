cmake_minimum_required(VERSION 4.3.3)
message(STATUS "[-----ImEngine - Core-----]")

set(IME_CORE_HEADERS)

set(IME_CORE_MODULES
        modules/Core.cppm

        modules/Thread/Thread.cppm
        modules/Thread/Worker.cppm
        modules/Thread/Job.cppm

        modules/IO/IO.cppm
        modules/IO/Logging/Logger.cppm
        modules/IO/Logging/Sinks/Sink.cppm
)

set(IME_CORE_SOURCES)


add_library(ImEngine_Core STATIC)

target_sources(ImEngine_Core
    PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${IME_CORE_HEADERS}
    PUBLIC FILE_SET CXX_MODULES BASE_DIRS modules FILES ${IME_CORE_MODULES}
    PRIVATE ${IME_CORE_SOURCES}
)