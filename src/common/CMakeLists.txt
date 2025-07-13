file(GLOB COMMON_SOURCES "*.c")
add_library(common STATIC ${COMMON_SOURCES})
target_include_directories(common PUBLIC "include")
target_link_libraries(common PRIVATE hal freertos_plus_tcp)