get_filename_component(SYSTEMD_UNIT_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/../lib/systemd/system" ABSOLUTE)

file(GLOB SYSTEMD_UNITS
    *.service
    )

add_custom_target(vf-files-systemd-units ALL
    DEPENDS ${SYSTEMD_UNITS}
    SOURCES ${SYSTEMD_UNITS}
    COMMAND echo "Install systemd units..."
    )
install(
    FILES ${SYSTEMD_UNITS}
    DESTINATION ${SYSTEMD_UNIT_INSTALL_PATH}
    )
