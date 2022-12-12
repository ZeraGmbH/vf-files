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

# install public headers
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/file-access-control-lib
    )
