#ifndef BSP_NETWORK_INCLUDE_NETWORK
#define BSP_NETWORK_INCLUDE_NETWORK

#include <stdint.h>

#include "communications.h"

#define ADDR_SIZE 4

typedef struct {
    uint8_t addr[ADDR_SIZE];
    uint16_t port;
} InetAddr;

typedef void (*DataCallback)(const uint8_t *, uint16_t);

void InitNetwork(CommunicationHandle *hCom, const DataCallback callback);

void Connect(InetAddr addr);

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize);

void Tick();
#endif /* BSP_NETWORK_INCLUDE_NETWORK */
