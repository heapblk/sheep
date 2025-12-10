cmake_minimum_required(VERSION 3.15)

project(sheep VERSION 1.0 LANGUAGES CXX)

# Options
option(BUILD_SHARED "Build shared library" ON)
option(BUILD_STATIC "Build static library" ON)

# the c++17 standard is widely supported, thats why its the defaut here
set(CMAKE_CXX_STANDARD 17)

# paths
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# sources
set(HEADER_FILES
        "${INCLUDE_DIR}/fast/fileio.h"
        "${INCLUDE_DIR}/fast/circ_buf.h"
        "${INCLUDE_DIR}/cli/argparse.h"
)

set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/fast/fileio.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/fast/circ_buf.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/argparse.cpp"
)

# publicly exposed header files
set(EXPORT_FILES
        "${INCLUDE_DIR}/fast/fileio.h"
        "${INCLUDE_DIR}/fast/circ_buf.h"
        "${INCLUDE_DIR}/cli/argparse.h"
)

# build as shared lib
if(BUILD_SHARED)
    add_library(sheep_shared SHARED ${SOURCE_FILES})
    set_target_properties(sheep_shared PROPERTIES OUTPUT_NAME "sheep_shared")
    target_include_directories(sheep_shared PUBLIC
			"$<BUILD_INTERFACE:${INCLUDE_DIR}>"
			"$<INSTALL_INTERFACE:include>"
	)
    add_library(sheep::sheep_shared ALIAS sheep_shared)
endif()

# build as static lib
if(BUILD_STATIC)
    add_library(sheep_static STATIC ${SOURCE_FILES})
    set_target_properties(sheep_static PROPERTIES OUTPUT_NAME "sheep")
    target_include_directories(sheep_static PUBLIC
			"$<BUILD_INTERFACE:${INCLUDE_DIR}>"
			"$<INSTALL_INTERFACE:include>"
	)
    add_library(sheep::sheep_static ALIAS sheep_static)
endif()

# installation
include(GNUInstallDirs)

# install headers
install(FILES ${EXPORT_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install libraries
if(BUILD_SHARED)
    install(TARGETS sheep_shared
        EXPORT sheepTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(BUILD_STATIC)
    install(TARGETS sheep_static
        EXPORT sheepTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# export targets for find_package
install(EXPORT sheepTargets
    FILE sheepTargets.cmake
    NAMESPACE sheep::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sheep"
)

# generate sheepConfig.cmake and sheepConfigVersion.cmake
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/sheepConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sheepConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/sheepConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sheep"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/sheepConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/sheepConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sheep"
)

# setup CTest
add_executable(sheep_test "test/sheep_test.cpp")

if(BUILD_STATIC)
    target_link_libraries(sheep_test PRIVATE sheep_static)
elseif(BUILD_SHARED)
    target_link_libraries(sheep_test PRIVATE sheep_shared)
endif()

target_include_directories(sheep_test PRIVATE "../lib/include")

enable_testing()
add_test(NAME sheep_test COMMAND sheep_test)
