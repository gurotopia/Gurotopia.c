#include "enet/enet.h"
#include "game/gameEvent/gameEvent.h"

ENetHost *host = NULL;

int main(void) {
    if (enet_initialize() < 0)
        return EXIT_FAILURE;

    ENetAddress address = {
        .type = ENET_ADDRESS_TYPE_IPV4,
        .port = 17091
    };
    enet_address_build_any(&address, address.type);
    
    host = enet_host_create(address.type, &address, 50, 2, 0, 0);
    if (!host)
        return EXIT_FAILURE;
        
    host->usingNewPacketForServer = 1;
    host->checksum = enet_crc32;
    if (enet_host_compress_with_range_coder(host) < 0)
        return EXIT_FAILURE;

    ENetEvent event;
    while (1) {
        if (enet_host_service(host, &event, 5000) > 0) {
            eventHandler eventHandle = { 
                .pEvent = &event
            };
            eventTypes[event.type](&eventHandle);
        }
    }
    return EXIT_SUCCESS;
}
