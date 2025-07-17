include(FetchContent)
set(ENABLE_ENC28J60_FREERTOS_DRIVER true)

add_definitions(
  -DPICO_USE_MALLOC_MUTEX
)

FetchContent_Declare(
    picoEnc28j60
    GIT_REPOSITORY https://github.com/ljanyst/pico-enc28j60.git
    SOURCE_SUBDIR src
)
FetchContent_MakeAvailable(picoEnc28j60)

if(NOT ${IS_TEST_BUILD})
  add_library(freertos_plus_tcp_network_if STATIC)
  target_sources(freertos_plus_tcp_network_if PRIVATE RP2040/ENC28J60/NetworkInterface.c)
  target_link_libraries(freertos_plus_tcp_network_if PUBLIC pico_enc28j60_freertos_driver pico_stdlib_headers)
endif()


file(GLOB HAL_SOURCES "RP2040/hal.c")
add_library(hal STATIC EXCLUDE_FROM_ALL)
target_sources(hal PRIVATE ${HAL_SOURCES})
target_include_directories(hal PUBLIC "include")
target_link_libraries(hal PUBLIC pico_stdio_usb PRIVATE pico_stdlib_headers pico_rand hardware_pio pico_enc28j60_freertos_driver_headers freertos_plus_tcp)