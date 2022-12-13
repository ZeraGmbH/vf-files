cmake_minimum_required(VERSION 3.1)

# set project name and language
project(vf-files LANGUAGES CXX)

# Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set project Version
set(VfFiles_VERSION_MAJOR "0")
set(VfFiles_VERSION_MINOR "0")
set(VfFiles_VERSION_PATCH "1")

set(PROJECT_VERSION "${VfFiles_VERSION_MAJOR}.${VfFiles_VERSION_MINOR}.${VfFiles_VERSION_PATCH}")

# Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

# Find dependecies
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort CONFIG)
find_package(VfHelpers REQUIRED)
find_package(VfEvent REQUIRED)
find_package(VfComponent REQUIRED)
find_package(VfStorageHash REQUIRED)
find_package(VfCpp REQUIRED)

# add subdirectory
enable_testing()
add_subdirectory(file-access-control)

# create library
add_library(VfFiles SHARED
    ${PROJECT_SOURCE_DIR}/src/vf_files.h
    ${PROJECT_SOURCE_DIR}/src/vf_files.cpp
    ${PROJECT_SOURCE_DIR}/src/dirwatcherentry.h
    ${PROJECT_SOURCE_DIR}/src/dirwatcherentry.cpp
    ${PROJECT_SOURCE_DIR}/src/defaultpathentry.h
    ${PROJECT_SOURCE_DIR}/src/defaultpathentry.cpp
    ${PROJECT_SOURCE_DIR}/src/mountwatcherentry.h
    ${PROJECT_SOURCE_DIR}/src/mountwatcherentry.cpp
    ${PROJECT_SOURCE_DIR}/src/serialdevicewatcher.h
    ${PROJECT_SOURCE_DIR}/src/serialdevicewatcher.cpp
)

# link dependencies
target_link_libraries(VfFiles 
    PRIVATE
    Qt5::Core
    Qt5::SerialPort
    VeinMeta::VfHelpers
    VeinMeta::VfEvent
    VeinMeta::VfComponent
    VeinMeta::VfStorageHash
    VeinMeta::VfCpp
    file-access-control-lib
    )

# set target Version
set_target_properties(VfFiles PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfFiles PROPERTIES SOVERSION ${VfFiles_VERSION_MAJOR})

# generate export header
include(GenerateExportHeader)
generate_export_header(VfFiles)

# install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vffiles_export.h
    ${PROJECT_SOURCE_DIR}/src/vf_files.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFiles
    )

# announce headers - target perspective
target_include_directories(VfFiles
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfFiles>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# install library
install(TARGETS VfFiles
    EXPORT VfFilesExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    VfFilesConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# configure *Config.cmake
configure_file(VfFilesConfig.cmake.in VfFilesConfig.cmake @ONLY)



# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfFilesConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfFilesConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfFiles
    )

# install targets cmake-files
install(EXPORT VfFilesExport
    FILE VfFilesTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfFiles
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFiles
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)


