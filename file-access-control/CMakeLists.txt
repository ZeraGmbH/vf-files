set(SET_TEST_DATA_PATH  "${PROJECT_SOURCE_DIR}/file-access-control/tests/test-data")
add_definitions(-DTEST_DATA_PATH="${SET_TEST_DATA_PATH}")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/file-access-control/src/fileaccesscontrol.cpp
    )

set(HEADERS
    ${PROJECT_SOURCE_DIR}/file-access-control/src/fileaccesscontrol.h
    )

# create library
add_library(file-access-control-lib SHARED
    ${SOURCES}
    ${HEADERS}
)

# link dependencies
target_link_libraries(file-access-control-lib
    PUBLIC
    Qt5::Core
    )

# set target Version
set_target_properties(file-access-control-lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(file-access-control-lib PROPERTIES SOVERSION ${VfFiles_VERSION_MAJOR})

# generate export header
include(GenerateExportHeader)
generate_export_header(file-access-control-lib)


# install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/file-access-control-lib_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/file-access-control-lib
    )

# announce headers - target perspective
target_include_directories(file-access-control-lib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/file-access-control-lib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )


# install library
install(TARGETS file-access-control-lib
    EXPORT file-access-control-libExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    file-access-control-libConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# configure *Config.cmake
configure_file(file-access-control-libConfig.cmake.in file-access-control-libConfig.cmake @ONLY)

# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/file-access-control-libConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/file-access-control-libConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/file-access-control-lib
    )

# install targets cmake-files
install(EXPORT file-access-control-libExport
    FILE file-access-control-libTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/file-access-control-lib
    )

# install public headers
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/file-access-control-lib
    )

add_subdirectory(tests)
