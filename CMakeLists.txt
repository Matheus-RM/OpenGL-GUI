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
find_package(glm REQUIRED HINTS ${PACKAGES_DIR})


add_executable(prog
                src/main.cpp
                src/App.cpp
                src/Window.cpp
                src/Input.cpp
                src/ShaderLoader.cpp
                src/Renderer.cpp
                src/Interface.cpp
                src/Widget.cpp
                src/Background.cpp
                src/CameraManager.cpp

                src/Util/BinaryIO.cpp
                src/Util/GLHelper.cpp

                src/GLShader.cpp
                src/Factories/GLFactory.cpp
                src/Factories/ShapeFactory.cpp

                src/Time/Clock.cpp
                src/Time/ScopedTimer.cpp
                src/Time/Timer.cpp
                )

target_compile_features(prog PUBLIC cxx_std_17)

target_include_directories(prog PUBLIC "./include")
target_include_directories(prog PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(prog PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(prog PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(prog PUBLIC ${GLM_INCLUDE_DIRS})


target_link_libraries(prog PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(prog PUBLIC SDL2_image)
target_link_libraries(prog PUBLIC SDL2_ttf)
target_link_libraries(prog PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(prog PUBLIC "opengl32")



set_target_properties( prog
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
)

message("Copying dlls and licenses.")

file(GLOB USED_DLL_FILES "${SDL2_PREFIX}/bin/*.dll" "${GLEW_PREFIX}/bin/*.dll")
file(GLOB LICENSE_FILES "${SDL2_PREFIX}/bin/*" "${GLEW_PREFIX}/bin/*.")

list(REMOVE_ITEM LICENSE_FILES ${USED_DLL_FILES})

file(COPY ${USED_DLL_FILES} DESTINATION "${BIN_OUTPUT_DIR}")
file(COPY ${LICENSE_FILES} DESTINATION "${BIN_OUTPUT_DIR}/licenses")