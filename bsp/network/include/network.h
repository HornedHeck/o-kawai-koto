#ifndef BSP_NETWORK_INCLUDE_NETWORK
#define BSP_NETWORK_INCLUDE_NETWORK

#include <stdint.h>
#include "communications.h"

#define ADDR_SIZE 4

typedef struct {
    uint8_t addr[ADDR_SIZE];
    uint16_t port;
} InetAddr;

void InitNetwork(const CommunicationHandle *handle);

void Connect(InetAddr addr);

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize);

#endif /* BSP_NETWORK_INCLUDE_NETWORK */
