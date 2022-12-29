cmake_minimum_required (VERSION 3.18.0)

project (Mathyw VERSION 1.0)

add_library(${PROJECT_NAME}
# dependencies
    "external/glad/glad.c"
    "external/stb/stb_image.c"
# source files
    "src/opengl.cpp"
    "src/monitor.cpp"
    "src/window.cpp"
    "src/clock.cpp"
    "src/vertex_array.cpp"
    "src/shader.cpp"
    "src/transformation.cpp"
    "src/value_tracker.cpp"
    "src/texture.cpp"
    "src/font.cpp"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Download Git submodules
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodule as needed
    option(GIT_SUBMODULE "Check submodule during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}.")
        endif()
    endif()
endif()

# Check submodules
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodule was not download! GIT_SUBMODULE was turned off or failed.")
endif()

# Add external submodules
set(GLFW_BUILD_DOCS OFF)
add_subdirectory("external/glfw")
add_subdirectory("external/freetype")

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC "external/glfw/include"
    PUBLIC "external/glad/include"
    PUBLIC "external/stb/include"
    PUBLIC "external/freetype/include"
    PUBLIC "include"
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    glfw
    freetype
)

# Create c++ library test
option(MATHYW_BUILDTEST "Build Mathyw test" OFF)
if (MATHYW_BUILDTEST)
    add_subdirectory("test")
endif()

# Build Mathyw examples
option(MATHYW_BUILDEXAMPLES "Build Mathyw examples" OFF)
if (MATHYW_BUILDEXAMPLES)
    add_subdirectory("examples")
endif()