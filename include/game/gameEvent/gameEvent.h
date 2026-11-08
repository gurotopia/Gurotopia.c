#pragma once

#include "enet/enet.h"

typedef enum : unsigned int {
    GAME_EVENT_TYPE_NONE,
    GAME_EVENT_TYPE_HELLO,
    GAME_EVENT_TYPE_GENERIC_TEXT,
    GAME_EVENT_TYPE_MESSAGE,
    GAME_EVENT_TYPE_PACKET
} gameEventType;

typedef struct {
    unsigned int packetType; // GAME_EVENT_TYPE_MESSAGE

    char message[];
} gameMessage;

typedef struct {
    ENetEvent *pEvent;
} eventHandler;

extern void eventConnect(const eventHandler *handler);
extern void eventReceive(const eventHandler *handler);
static void (*const eventTypes[4])(const eventHandler *) = {
    NULL, // none
    &eventConnect,
    NULL, // disconnect
    &eventReceive
};