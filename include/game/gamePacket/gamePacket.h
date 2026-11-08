#pragma once

typedef struct {
    unsigned int packetType; // GAME_EVENT_TYPE_PACKET

    union { 
        int type; 
    };
    union { 
        int netid; 
    };
    union { 
        int uid; 
    };
    union { 
        int state; 
    };
    union { 
        float count; 
    };
    union { 
        int id; 
    };
    union { 
        float pos[2]; 
    };
    union { 
        float speed[2]; 
    };
    union { 
        int idk; 
    };
    union {
        float punch[2]; 
    };
    union { 
        unsigned int size;
    };
} gamePacket;
