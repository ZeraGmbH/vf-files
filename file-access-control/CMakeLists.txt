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

# announce headers - target perspective
target_include_directories(file-access-control-lib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/file-access-control-lib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# install library
install(TARGETS file-access-control-lib
    EXPORT file-access-control-libExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
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
