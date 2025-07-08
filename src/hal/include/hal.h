#ifndef __FOMODON_HAL_H__
#define __FOMODON_HAL_H__

struct xNetworkInterface;

void hal_test_blink();
void hal_sleep_ms (uint32_t ms);
bool hal_init_hw();
struct xNetworkInterface* hal_get_network_interface();

#endif