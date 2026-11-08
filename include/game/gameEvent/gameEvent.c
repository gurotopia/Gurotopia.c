#include "gameEvent.h"
#include "game/gamePacket/gamePacket.h"

#include <stdio.h>

void eventConnect(const eventHandler *handler) {
    if (!handler || !handler->pEvent)
        return;

    const char *pMessage = "action|log\nmsg|hello world!";
    size_t len = strlen(pMessage) + 1;
    gameMessage *pGameMessage = malloc(sizeof(gameMessage) + len);
    if (pGameMessage == NULL)
        return;

    pGameMessage->packetType = GAME_EVENT_TYPE_MESSAGE;
    memcpy(pGameMessage->message, pMessage, len);

    ENetPacket *msgPacket = enet_packet_create(pGameMessage, sizeof(gameMessage) + len, ENET_PACKET_FLAG_RELIABLE);
    if (enet_peer_send(handler->pEvent->peer, 0, msgPacket))
        enet_packet_destroy(msgPacket);
    
    free(pGameMessage);

    unsigned int status = GAME_EVENT_TYPE_HELLO;
    ENetPacket *packet = enet_packet_create(&status, sizeof status, ENET_PACKET_FLAG_RELIABLE);
    if (!packet)
        return;

    if (enet_peer_send(handler->pEvent->peer, 0, packet))
        enet_packet_destroy(packet);
}

void eventReceive(const eventHandler *handler) {
    if (!handler || !handler->pEvent)
        return;

    unsigned int packetType = GAME_EVENT_TYPE_NONE;
    ENetPacket *pPacket = handler->pEvent->packet;

    if (pPacket->dataLength >= sizeof packetType)
        memcpy(&packetType, pPacket->data, sizeof packetType);

    switch (packetType) {
        case GAME_EVENT_TYPE_GENERIC_TEXT: 
        {
            const char *message = (const char *)(pPacket->data + sizeof packetType);
            // @todo read each pipes
            
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

            gamePacket gamePacket = {
                .packetType = GAME_EVENT_TYPE_PACKET
            };
            if (pPacket->dataLength >= sizeof gamePacket)
                memcpy(&gamePacket, pPacket->data, sizeof gamePacket);
            // @todo read extra bytes

            printf("gamePacket.type = %d\n", gamePacket.type);
            break;
        }
    }
    
    enet_packet_destroy(pPacket);
}
