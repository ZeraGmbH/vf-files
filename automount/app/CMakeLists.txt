file(GLOB HEADERS
    *.h
    )

file(GLOB SOURCES
    *.cpp
    )

file(GLOB RESOURCES
    *.qrc
    )

add_executable(zenux-automount
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
    )

target_link_libraries(zenux-automount
    PRIVATE
    Qt5::Core
)

install(TARGETS zenux-automount
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

