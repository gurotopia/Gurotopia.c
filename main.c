#include "include/enet/enet.h"
#include <stdio.h>

ENetHost *host = NULL;

typedef struct event_handler eventHandler;

struct event_handler {
    void (*eventConnect)(const eventHandler *handler);
    void (*eventReceive)(const eventHandler *handler);

    ENetEvent mEvent;
};

void handleEventConnect(const eventHandler *handler)
{
    if (!handler)
        return;

    int status = 1;
    ENetPacket *packet = enet_packet_create(NULL, sizeof status, ENET_PACKET_FLAG_RELIABLE);
    memcpy(packet->data, &status, sizeof status);
    
    if (enet_peer_send(handler->mEvent.peer, 0, packet)) 
    enet_packet_destroy(packet);

    handler->eventConnect(handler);
}

void handleEventReceive(const eventHandler *handler)
{
    if (!handler)
        return;

    ENetPacket *packet = handler->mEvent.packet;
    switch (packet->data[0])
    {
        case 2:
        {
            const char *message = (const char *)(packet->data + sizeof(int));
            printf("%s\n", message);

            break;
        }
    }
    enet_packet_destroy(packet);

    handler->eventReceive(handler);
}

int main(void)
{
    if (enet_initialize())
    {
        return EXIT_FAILURE;
    }

    ENetAddress address;
    enet_address_set_host(&address, ENET_ADDRESS_TYPE_IPV4, "0.0.0.0");
    address.port = 17091;

    host = enet_host_create(ENET_ADDRESS_TYPE_IPV4, &address, 50, 2, 0, 0);
    if (!host)
    {
        return EXIT_FAILURE;
    }
    host->usingNewPacketForServer = 1;
    host->checksum = enet_crc32;
    enet_host_compress_with_range_coder(host);

    ENetEvent event;
    while (1)
    {
        while (enet_host_service(host, &event, 1000) > 0)
        {
            eventHandler handler = {
                .mEvent = event, 
                .eventConnect = NULL
            };

            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    handleEventConnect(&handler);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    handleEventReceive(&handler);
                    break;
            }
        }
    }
    return EXIT_SUCCESS;
}
