cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(raygo)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler options for GCC and Clang
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -std=c11         # Use the C11 standard
        -Wall            # Enable all warnings
        -Wextra          # Enable extra warnings
        -Wpedantic       # Enforce strict compliance with standards
        -Wshadow         # Warn if a variable declaration shadows one from an outer scope
        -Wstrict-prototypes # Warn if function declarations/definitions are missing prototypes
        -Wold-style-definition # Warn about old-style function definitions
        -Wmissing-prototypes  # Warn if a global function is defined without a prototype
        -Wconversion     # Warn about implicit type conversions
        -Wsign-conversion # Warn about implicit conversions between signed and unsigned
        -Wformat=2       # Extra checks on printf/scanf formats
        -Wdouble-promotion # Warn if a float is implicitly promoted to double
        -fstack-protector-strong # Add stack protection
        -D_FORTIFY_SOURCE=2 # Enable additional runtime checks
        -O2              # Optimize for speed while keeping good debug information
    )
endif()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

# Our Project
add_executable(${PROJECT_NAME} src/main.c)

# Include the src directory for raygui.h
target_include_directories(${PROJECT_NAME} PRIVATE src)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
