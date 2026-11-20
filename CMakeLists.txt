if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.16)
    project(property VERSION 1.0.0 LANGUAGES CXX) # x-release-please-version
endif()

add_library(musProperty INTERFACE "includes/musProperty.h" "includes/musCheckOperator.h")

target_include_directories(musProperty
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
        $<INSTALL_INTERFACE:include>
)

if (CMAKE_VERSION VERSION_GREATER 3.8)
    set_property(TARGET musProperty PROPERTY CXX_STANDARD 23)
    set_property(TARGET musProperty PROPERTY CXX_STANDARD_REQUIRED ON)
    set_property(TARGET musProperty PROPERTY CXX_EXTENSIONS OFF)
endif()

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(PROPERTY_BUILD_TESTS "Build property unit tests" ON)
    if (PROPERTY_BUILD_TESTS)
        enable_testing()
        add_executable(test_property "tests/test_property.cpp")
        target_link_libraries(test_property PRIVATE musProperty)
        set_property(TARGET test_property PROPERTY CXX_STANDARD 23)
        set_property(TARGET test_property PROPERTY CXX_STANDARD_REQUIRED ON)
        set_property(TARGET test_property PROPERTY CXX_EXTENSIONS OFF)
        add_test(NAME property.test_property COMMAND test_property)
    endif()

    include(GNUInstallDirs)
    install(FILES
        "includes/musProperty.h"
        "includes/musCheckOperator.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    install(FILES
        "LICENSE.txt"
        "README.md"
        "CMakeLists.txt"
        DESTINATION "."
    )
endif()