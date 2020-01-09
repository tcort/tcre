cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(tcre VERSION 1.0.0 DESCRIPTION "simple regular expressions library"
    LANGUAGES C)

include(GNUInstallDirs)

add_library(tcre SHARED tcre.c)

set_target_properties(tcre PROPERTIES VERSION ${PROJECT_VERSION}
    SOVERSION 1 PUBLIC_HEADER tcre.h)

configure_file(tcre.pc.in tcre.pc @ONLY)

enable_testing(true)
add_executable(tcre.test tcre.test.c)
target_link_libraries(tcre.test tcre)
add_test(tcre.test tcre.test)

install(FILES ${CMAKE_BINARY_DIR}/tcre.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

install(TARGETS tcre LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CPack)
