add_library(musProperty INTERFACE "includes/musProperty.h" "includes/musCheckOperator.h")

target_include_directories(musProperty
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)