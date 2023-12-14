cmake_minimum_required(VERSION 3.1)

project(vf-files LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

find_package(Qt5 REQUIRED COMPONENTS Core SerialPort CONFIG)
find_package(VeinFramework REQUIRED)

enable_testing()

write_basic_package_version_file(
    VfFilesConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
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

add_subdirectory(vein-component)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
