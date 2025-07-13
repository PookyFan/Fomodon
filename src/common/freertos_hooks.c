#include "hal.h"
#include "FreeRTOS_DHCP.h"

static const char hostname[] = "fomodon";

const char* pcApplicationHostnameHook()
{
    return hostname;
}

eDHCPCallbackAnswer_t xApplicationDHCPHook(eDHCPCallbackPhase_t /*phase*/, uint32_t /*ip_addr*/)
{
    return eDHCPContinue;
}

BaseType_t xApplicationGetRandomNumber(uint32_t* rand_num_out)
{
    *rand_num_out = hal_rand();
    return pdTRUE;
}