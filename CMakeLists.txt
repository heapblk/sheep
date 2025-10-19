cmake_minimum_required(VERSION 3.15)

project(templ VERSION 1.0 LANGUAGES CXX)

# Options
option(BUILD_SHARED "Build shared library" ON)
option(BUILD_STATIC "Build static library" ON)

# the c++17 standard is widely supported, thats why its the defaut here
set(CMAKE_CXX_STANDARD 17)

# paths
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# sources
set(HEADER_FILES
        "${INCLUDE_DIR}/templ.h"
        "${INCLUDE_DIR}/ignore.h"
)

set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/templ.cpp"
)

# publicly exposed header files
set(EXPORT_FILES
		"${INCLUDE_DIR}/templ.h"
)

# build as shared lib
if(BUILD_SHARED)
    add_library(templ_shared SHARED ${SOURCE_FILES})
    set_target_properties(templ_shared PROPERTIES OUTPUT_NAME "templ_shared")
    target_include_directories(templ_shared PUBLIC
			"$<BUILD_INTERFACE:${INCLUDE_DIR}>"
			"$<INSTALL_INTERFACE:include>"
	)
    add_library(templ::templ_shared ALIAS templ_shared)
endif()

# build as static lib
if(BUILD_STATIC)
    add_library(templ_static STATIC ${SOURCE_FILES})
    set_target_properties(templ_static PROPERTIES OUTPUT_NAME "templ")
    target_include_directories(templ_static PUBLIC
			"$<BUILD_INTERFACE:${INCLUDE_DIR}>"
			"$<INSTALL_INTERFACE:include>"
	)
    add_library(templ::templ_static ALIAS templ_static)
endif()

# installation
include(GNUInstallDirs)

# install headers
install(FILES ${EXPORT_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install libraries
if(BUILD_SHARED)
    install(TARGETS templ_shared
        EXPORT templTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(BUILD_STATIC)
    install(TARGETS templ_static
        EXPORT templTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# export targets for find_package
install(EXPORT templTargets
    FILE templTargets.cmake
    NAMESPACE templ::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/templ"
)

# generate templConfig.cmake and templConfigVersion.cmake
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/templConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/templConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/templ"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/templConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/templConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/templ"
)

# setup CTest
add_executable(templ_test "test/templ_test.cpp")

if(BUILD_STATIC)
    target_link_libraries(templ_test PRIVATE templ_static)
elseif(BUILD_SHARED)
    target_link_libraries(templ_test PRIVATE templ_shared)
endif()

target_include_directories(templ_test PRIVATE "../lib/include")

enable_testing()
add_test(NAME templ_test COMMAND templ_test)
