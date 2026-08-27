file(GLOB_RECURSE LOGGER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE LOGGER_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(SmL STATIC
    ${LOGGER_SOURCES}
    ${LOGGER_HEADERS}
)
set_target_properties(SmL
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        LINKER_LANGUAGE CXX
)
target_include_directories(SmL
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)