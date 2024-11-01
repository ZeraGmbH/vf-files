file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADERS
    *.h
    )

add_library(VfFiles SHARED
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(VfFiles 
    PUBLIC
    Qt5::Core
    Qt5::SerialPort
    Zera::VfEvent
    Zera::VfComponent
    Zera::VfStorage
    Zera::VfCpp
    )

set_target_properties(VfFiles PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfFiles PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfFiles)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vffiles_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFiles
    )

target_include_directories(VfFiles
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfFiles>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFiles
    )

install(TARGETS VfFiles
    EXPORT VfFilesExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
