#include "pico/enc28j60-freertos-driver.h"

static BaseType_t enc28j60_initialize(NetworkInterface_t* pxInterface)
{
    enc28j60_freertos_interface *iface = (enc28j60_freertos_interface*)pxInterface;
    if(iface->initialized)
        return pdTRUE;

    enc28j60_config *drv_cfg = enc28j60_get_default_config();
    uint8_t* mac_addr = pxInterface->pxEndPoint->xMACAddress.ucBytes;
    memcpy(drv_cfg->mac_addr, mac_addr, ipMAC_ADDRESS_LENGTH_BYTES);
    drv_cfg->irq_pin  = 10;
    drv_cfg->miso_pin = 11;
    drv_cfg->mosi_pin = 12;
    drv_cfg->sclk_pin = 13;
    //Pin sclk_pin + 1 will be used as slave select

    pxInterface->pvArgument = drv_cfg;
    return enc28j60_freertos_initialize(pxInterface);
}

NetworkInterface_t* pxFillInterfaceDescriptor(BaseType_t xEMACIndex, NetworkInterface_t* pxInterface)
{
    enc28j60_freertos_interface* enc28j60 = (enc28j60_freertos_interface*)pxInterface;
    memset(enc28j60, 0, sizeof(*enc28j60));

    enc28j60->iface.pcName = "e0";
    
    enc28j60->iface.pfInitialise = enc28j60_initialize;
    enc28j60->iface.pfOutput = enc28j60_freertos_output_frame;
    enc28j60->iface.pfGetPhyLinkStatus = enc28j60_freertos_get_phy_link_status;
    FreeRTOS_AddNetworkInterface(&enc28j60->iface);

    return pxInterface;
}