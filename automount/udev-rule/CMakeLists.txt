file(GLOB UDEV_RULES
    *.rules
    )

add_custom_target(vf-files-udev-rules ALL
    DEPENDS ${UDEV_RULES}
    SOURCES ${UDEV_RULES}
    COMMAND echo "Install systemd units..."
    )

get_filename_component(UDEV_RULES_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/../usr/lib/udev/rules.d" ABSOLUTE)

install(
    FILES ${UDEV_RULES}
    DESTINATION ${UDEV_RULES_INSTALL_PATH}
    )
