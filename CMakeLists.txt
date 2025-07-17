cmake_minimum_required(VERSION 3.27)

if(${CMAKE_C_COMPILER} MATCHES "arm-none-eabi")
    set(IS_TEST_BUILD False)
    add_compile_definitions(ENABLE_SMP)
else()
    set(IS_TEST_BUILD True)
    set(PICO_PLATFORM host)
    enable_testing()
    add_compile_definitions(LargestIntegralTypePrintfFormat="%ld")
endif()

#Import Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

set(FREERTOS_PATH $ENV{FREERTOS_PATH})
set(FREERTOS_PATH "${FREERTOS_PATH}" CACHE PATH "Path to RTOS")

if(NOT ${IS_TEST_BUILD})
    #Import FreeRTOS for RP2040
    include(${FREERTOS_PATH}/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
endif()

project(Fomodon)
set(CMAKE_C_STANDARD 11)

#Initialize Raspberry Pi Pico SDK
pico_sdk_init()

set(CFG_DIRECTORY "config")
set(FREERTOS_PLUS_TCP_NETWORK_IF "A_CUSTOM_NETWORK_IF" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE ${CFG_DIRECTORY})
target_link_libraries(freertos_config INTERFACE FreeRTOS-Kernel)

if(NOT ${IS_TEST_BUILD})
    #Set FreeRTOS port
    set(FREERTOS_PORT GCC_RP2040 CACHE STRING "")
endif()

add_subdirectory(${FREERTOS_PATH}/FreeRTOS/Source FreeRTOS_build)
add_subdirectory(${FREERTOS_PATH}/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP FreeRTOS-Plus-TCP_build)

#todo: enable USB serial for testing and debug
pico_enable_stdio_usb(pico_stdio 1)
pico_enable_stdio_uart(pico_stdio 0)

add_subdirectory(src)
if(${IS_TEST_BUILD})
    add_subdirectory(test)
endif()