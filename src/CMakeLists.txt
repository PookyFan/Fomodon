add_subdirectory(common)
add_subdirectory(hal)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} freertos_plus_tcp FreeRTOS-Kernel-Heap4 common hal)
pico_add_extra_outputs(${PROJECT_NAME})