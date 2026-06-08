cmake_minimum_required(VERSION 3.16)

project(HotelManagement
    VERSION 1.0.0
    DESCRIPTION "Hotel Management System"
    LANGUAGES CXX
)

# ── Standards ─────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Output dirs ───────────────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ── Build type default ────────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ── Compiler warnings (project-wide) ─────────────────────────────────────────
include(cmake/CompilerWarnings.cmake)

# ── Submodules ────────────────────────────────────────────────────────────────
add_subdirectory(src/core)
add_subdirectory(tests)

# ── Main executable (wired in Phase 5) ───────────────────────────────────────
# add_executable(hotel src/main.cpp)
# target_link_libraries(hotel PRIVATE core)
# apply_project_warnings(hotel)
