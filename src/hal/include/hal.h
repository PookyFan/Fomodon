#ifndef __FOMODON_HAL_H__
#define __FOMODON_HAL_H__

#include <stdbool.h>
#include <stdint.h>

struct xNetworkInterface;

void hal_test_blink();
void hal_sleep_ms (uint32_t ms);
bool hal_init_hw();
uint32_t hal_rand();
struct xNetworkInterface* hal_get_network_interface();

#endif