#pragma once

struct Message {
    char name[20];
    int msgtype; // 0 for group, 1 for personal
    char recipient_id[20];
    char group_id[20]; 
    unsigned long long timestamp;
    uint8_t msg[1024];
    int voice_or_text; // 0 for voice, 1 for text message 
};

struct Init {
    char user_id[20];
    char group_id[20];
};