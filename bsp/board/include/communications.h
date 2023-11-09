#ifndef BSP_BOARD_INCLUDE_COMMUNICATIONS
#define BSP_BOARD_INCLUDE_COMMUNICATIONS

#include <stdalign.h>

#include "stdint.h"

typedef enum { UART } Protocol;

typedef struct {
    Protocol protocol;
    uint8_t portNum;
    void *hwHandle;
} CommunicationHandle;

void InitCommunication(CommunicationHandle *handle);

void SendData(CommunicationHandle *handle, const uint8_t *data, uint16_t size);

void ReceiveData(CommunicationHandle *handle, uint8_t *buffer,
                 uint16_t bufferSize);

#endif /* BSP_BOARD_INCLUDE_COMMUNICATIONS */
