add_subdirectory(common)
add_subdirectory(hal)

if(NOT ${IS_TEST_BUILD})
    add_executable(${PROJECT_NAME} main.c)
    target_link_libraries(${PROJECT_NAME} pico_stdlib freertos_plus_tcp FreeRTOS-Kernel-Heap4 common hal)
    pico_add_extra_outputs(${PROJECT_NAME})
endif()