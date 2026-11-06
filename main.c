#include "enet/enet.h"
#include "include/game/gamePacket/gamePacket.h"
#include "include/game/gameEvent/gameEvent.h"

#include <stdio.h>

ENetHost *host = NULL;

typedef struct {
    ENetEvent *pEvent;
} eventHandler;

void eventConnect(eventHandler *handler) {
    if (!handler || !handler->pEvent)
        return;

    int status = GAME_EVENT_TYPE_HELLO;
    ENetPacket *packet = enet_packet_create(&status, sizeof status, ENET_PACKET_FLAG_RELIABLE);
    if (!packet)
        return;

    if (enet_peer_send(handler->pEvent->peer, 0, packet))
        enet_packet_destroy(packet);
}

void eventReceive(const eventHandler *handler) {
    if (!handler || !handler->pEvent)
        return;

    int packetType = GAME_EVENT_TYPE_NONE;
    ENetPacket *pPacket = handler->pEvent->packet;

    if (pPacket->dataLength >= sizeof packetType)
        memcpy(&packetType, pPacket->data, sizeof packetType);

    switch (packetType) {
        case GAME_EVENT_TYPE_GENERIC_TEXT: 
        {
            const char *message = (const char *)(pPacket->data + sizeof packetType);
            
            printf("generic text: %s\n", message);
            break;
        }
        case GAME_EVENT_TYPE_MESSAGE:
        {
            const char *message = (const char *)(pPacket->data + sizeof packetType);
            
            printf("message: %s\n", message);
            break;
        }
        case GAME_EVENT_TYPE_PACKET: {
            gamePacket gamePacket = {.packet_create = 0x4};
            if (pPacket->dataLength >= sizeof gamePacket)
                memcpy(&gamePacket, pPacket->data, sizeof gamePacket);

            printf("gamePacket.type = %d\n", gamePacket.type);
            break;
        }
    }
    enet_packet_destroy(pPacket);
}

int main(void) {
    if (enet_initialize() < 0)
        return EXIT_FAILURE;

    ENetAddress address;
    enet_address_build_any(&address, ENET_ADDRESS_TYPE_IPV4);
    address.port = 17091;

    host = enet_host_create(ENET_ADDRESS_TYPE_IPV4, &address, 50, 2, 0, 0);
    if (!host)
        return EXIT_FAILURE;
        
    host->usingNewPacketForServer = 1;
    host->checksum = enet_crc32;
    enet_host_compress_with_range_coder(host);

    ENetEvent event;
    while (1) {
        while (enet_host_service(host, &event, 1000) > 0) {
            eventHandler eventHandle = {NULL};

            eventHandle.pEvent = &event;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    eventConnect(&eventHandle);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    eventReceive(&eventHandle);
                    break;
            }
        }
    }
    return EXIT_SUCCESS;
}
