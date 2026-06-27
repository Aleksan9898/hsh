cmake_minimum_required(VERSION 3.5)

project(hsh LANGUAGES C)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


set(SOURCES
    main.c
    src/hsh.c
)


add_executable(hsh ${SOURCES})

target_include_directories(hsh PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Тесты
enable_testing()

add_executable(run_unit_tests
    tests/test_parser.c
    src/hsh.c
)

target_include_directories(run_unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME ParserTests
    COMMAND run_unit_tests
)


find_package(PkgConfig REQUIRED)
pkg_check_modules(READLINE readline)


if(READLINE_FOUND)
    message(STATUS "hsh: Readline found. Advanced features enabled.")

    target_include_directories(hsh PRIVATE ${READLINE_INCLUDE_DIRS})
    target_link_directories(hsh PRIVATE ${READLINE_LIBRARY_DIRS})
    target_link_libraries(hsh PRIVATE ${READLINE_LIBRARIES})
    target_compile_definitions(hsh PRIVATE HAVE_READLINE)

    if(TARGET run_unit_tests)
        target_include_directories(run_unit_tests PRIVATE ${READLINE_INCLUDE_DIRS})
        target_link_directories(run_unit_tests PRIVATE ${READLINE_LIBRARY_DIRS})
        target_link_libraries(run_unit_tests PRIVATE ${READLINE_LIBRARIES})
        target_compile_definitions(run_unit_tests PRIVATE HAVE_READLINE)
    endif()
else()
    message(STATUS "hsh: Readline not found. Using standard input mode.")
endif()

include(GNUInstallDirs)

install(
    TARGETS hsh
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
