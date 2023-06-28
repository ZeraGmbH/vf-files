cmake_minimum_required(VERSION 3.1)

project(vf-files LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(VfFiles_VERSION_MAJOR "0")
set(VfFiles_VERSION_MINOR "0")
set(VfFiles_VERSION_PATCH "1")
set(PROJECT_VERSION "${VfFiles_VERSION_MAJOR}.${VfFiles_VERSION_MINOR}.${VfFiles_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

find_package(Qt5 REQUIRED COMPONENTS Core SerialPort CONFIG)
find_package(VeinFramework REQUIRED)
find_package(VfCpp REQUIRED)

enable_testing()

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/vf_files.cpp
    ${PROJECT_SOURCE_DIR}/src/dirwatcherentry.cpp
    ${PROJECT_SOURCE_DIR}/src/defaultpathentry.cpp
    ${PROJECT_SOURCE_DIR}/src/mountwatcherentry.cpp
    ${PROJECT_SOURCE_DIR}/src/serialdevicewatcher.cpp
    ${PROJECT_SOURCE_DIR}/src/fileaccesscontrol.cpp
    )

set(HEADERS
    ${PROJECT_SOURCE_DIR}/src/vf_files.h
    ${PROJECT_SOURCE_DIR}/src/dirwatcherentry.h
    ${PROJECT_SOURCE_DIR}/src/defaultpathentry.h
    ${PROJECT_SOURCE_DIR}/src/mountwatcherentry.h
    ${PROJECT_SOURCE_DIR}/src/serialdevicewatcher.h
    ${PROJECT_SOURCE_DIR}/src/fileaccesscontrol.h
    )

add_library(VfFiles SHARED
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(VfFiles 
    PUBLIC
    Qt5::Core
    Qt5::SerialPort
    Zera::VfHelpers
    Zera::VfEvent
    Zera::VfComponent
    Zera::VfStorageHash
    VeinMeta::VfCpp
    )

set_target_properties(VfFiles PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfFiles PROPERTIES SOVERSION ${VfFiles_VERSION_MAJOR})

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

install(TARGETS VfFiles
    EXPORT VfFilesExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfFilesConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfFilesConfig.cmake.in VfFilesConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfFilesConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfFilesConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfFiles
    )

install(EXPORT VfFilesExport
    FILE VfFilesTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfFiles
    )

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFiles
    )

add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
