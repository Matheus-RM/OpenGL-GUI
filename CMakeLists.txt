cmake_minimum_required(VERSION 3.2)

project(GUI-Project
        VERSION 1.0
        LANGUAGES CXX)

message("Configuring build mode.")

if(NOT DEFINED BUILD_MODE)
    message(FATAL_ERROR "The build mode wasn't specified.")
endif()

set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin/${BUILD_MODE}")

set(BIN_OUTPUT_DIR "${OUTPUT_DIR}")
set(LIB_OUTPUT_DIR "${OUTPUT_DIR}/lib")

message("Setting flags.")

if(BUILD_MODE STREQUAL "Debug")
    add_compile_options(-Og -Wall)

elseif(BUILD_MODE STREQUAL "Release")
    add_compile_options(-O3 -Wall)

endif()

message("Checking for sublime text configuration.")

if(DEFINED CONFIGURE_ST AND CONFIGURE_ST)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_compile_options(--verbose -target x86_64-pc-windows-gnu --sysroot=C:/Development/MinGW)

endif()

## Libs
set(PACKAGES_DIR "C:/Development/cmake-config")

find_package(SDL2 REQUIRED HINTS ${PACKAGES_DIR})
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED HINTS ${PACKAGES_DIR})


add_executable(prog
                src/main.cpp
                )

target_link_libraries(prog PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(prog PUBLIC SDL2_image)
target_link_libraries(prog PUBLIC SDL2_ttf)
target_link_libraries(prog PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(prog PUBLIC "opengl32")


target_include_directories(prog PUBLIC "./include")

set_target_properties( prog
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
)