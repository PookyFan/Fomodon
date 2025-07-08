#include <stdio.h>
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "task.h"
#include "hal.h"

static NetworkEndPoint_t endpoint;

static void hang()
{
    while(true);
}

static bool setup_network()
{
    static const uint8_t mac_addr[] = {0xFE, 0xD0, 0x69, 0x04, 0x20, 0x01};
    static const uint8_t default_net_mask[] = {255, 255, 255, 0};
    static const uint8_t default_ip[] = {192, 168, 0, 1};

    //Since there are "hacky" interface implementations (like ENC28J60 driver),
    //we don't call FreeRTOS_IPInit() directly, but do its job here instead
    NetworkInterface_t* interface_ptr = hal_get_network_interface();
    memset(&endpoint, 0, sizeof(endpoint));
    pxFillInterfaceDescriptor(0, interface_ptr);
    FreeRTOS_FillEndPoint(interface_ptr, &endpoint, default_ip, default_net_mask, default_ip, default_ip, mac_addr);
    endpoint.bits.bWantDHCP = (ipconfigUSE_DHCP ? pdTRUE : pdFALSE);
    return pdPASS == FreeRTOS_IPInit_Multi();
}

static void test_conn_task(void*)
{
    printf("[tcon] Initializing network...\n");

    if(!setup_network())
    {
        printf("[tcon] Error initializing network!\n");
        hang();
    }

    printf("[tcon] Start checking our IP\n");
    while(true)
    {
        hal_sleep_ms(5000);

        BaseType_t is_up = FreeRTOS_IsNetworkUp();
        uint32_t ip = FreeRTOS_GetIPAddress();
        char ip_str[16] = {0};
        FreeRTOS_inet_ntoa(ip, ip_str);
        printf("[tcon] Our IP: %s (raw 0x%08x), network is %s\n", ip_str, ip, is_up ? "up" : "down");
    }
}

static void blink_task(void*)
{
    hal_test_blink();
}

void main()
{
    if(!hal_init_hw())
        hang();

    printf("Starting blink task\n");
    if(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY ==
        xTaskCreate(blink_task, "blinkTask", 256, NULL, 1, NULL))
    {
        printf("Could not create blink task!\n");
        hang();
    }

    printf("Starting test task\n");
    if(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY ==
        xTaskCreate(test_conn_task, "testTask", 256, NULL, 1, NULL))
    {
        printf("Could not create test task!\n");
        hang();
    }

    printf("Starting scheduler\n");
    vTaskStartScheduler();
    hang();
}