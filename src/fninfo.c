#include <coco.h>
#include <fujinet-fuji.h>

int main(void)
{
    AdapterConfig ac;
    
    memset(&ac, 0, sizeof(AdapterConfig));

    if (fuji_get_adapter_config(&ac)) 
    {
        printf("\n");
        printf("%10s%s\n", "SSID: ", strupr(ac.ssid));
        printf("%10s%s\n", "HOSTNAME: ", strupr(ac.hostname));
        printf("%10s%u.%u.%u.%u\n","IP: ",ac.localIP[0],ac.localIP[1],ac.localIP[2],ac.localIP[3]);
        printf("%10s%u.%u.%u.%u\n","NETMASK: ",ac.netmask[0],ac.netmask[1],ac.netmask[2],ac.netmask[3]);
        printf("%10s%u.%u.%u.%u\n","DNS: ",ac.dnsIP[0],ac.dnsIP[1],ac.dnsIP[2],ac.dnsIP[3]);
        printf("%10s%02X:%02X:%02X:%02X:%02X:%02X\n","MAC: ",ac.macAddress[0],ac.macAddress[1],ac.macAddress[2],ac.macAddress[3],ac.macAddress[4],ac.macAddress[5]);
        printf("%10s%02X:%02X:%02X:%02X:%02X:%02X\n","BSSID: ",ac.bssid[0],ac.bssid[1],ac.bssid[2],ac.bssid[3],ac.bssid[4],ac.bssid[5]);
        printf("%10s%s\n","FNVER: ",strupr(ac.fn_version));
        printf("\n");
    }
    else 
    {
        printf("FAILED TO GET ADAPTER CONFIG.\n");
    }

	return 0;
}