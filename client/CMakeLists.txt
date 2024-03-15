cmake_minimum_required(VERSION 3.24.2)

# todo: make this better
set(
    CMAKE_BUILD_TYPE
    Release
)

set(
    CXX_STANDARD
    20
)

if (APPLE)
    set(
        DEFAULT_SYSROOT
        /Library/Developer/CommandLineTools/SDKs/MacOSX14.4.sdk
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lcs.exe)

include_directories(include)
include(files.cmake)

find_program(
    CONAN_EXE
    NAME conan conan.exe
    DOC Conan REQUIRED
)

execute_process(
    COMMAND ${CONAN_EXE} install . -of=build --build=missing -s build_type=${CMAKE_BUILD_TYPE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

include(build/conan_toolchain.cmake)

find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} openssl::openssl fmt::fmt)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/libs/libblib.a)