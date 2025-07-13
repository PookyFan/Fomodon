#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/enc28j60-freertos-driver.h"
#include "tusb.h" //todo: needed by tud_cdc_connected(), remove when that call gets removed
#include "hal.h"

static enc28j60_freertos_interface enc28j60_if;

void hal_test_blink()
{
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (1) {
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
    }
}

void hal_sleep_ms(uint32_t time_ms)
{
    sleep_ms(time_ms);
}

bool hal_init_hw()
{
    if(!stdio_init_all())
        return false; //todo: signal that something went wrong?
    
    get_rand_64(); //To initialize RNG module

    //todo: for testing purpose, we wait for serial to be available through USB, remove it later
    while (!tud_cdc_connected()) 
    {
        sleep_ms(100);
    }

    return true;
}

uint32_t hal_rand()
{
    return get_rand_32();
}

struct xNetworkInterface* hal_get_network_interface()
{
    return &enc28j60_if.iface;
}