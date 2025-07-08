file(GLOB COMMON_SOURCES "*.c")
add_library(common STATIC ${COMMON_SOURCES})
target_link_libraries(common PRIVATE freertos_plus_tcp)