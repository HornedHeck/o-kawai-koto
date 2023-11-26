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

typedef enum {
    RESPONSE_OK = 0,
    RESPONSE_ERROR,
    RESPONSE_TIMEOUT,
} ResponseStatus;

void InitCommunication(CommunicationHandle *handle);

ResponseStatus SendData(const CommunicationHandle *handle, const uint8_t *data,
                        uint16_t size);

ResponseStatus ReceiveData(const CommunicationHandle *handle, uint8_t *buffer,
                           uint16_t bufferSize);

ResponseStatus ReadByte(const CommunicationHandle *handle, uint8_t *dst);
#endif /* BSP_BOARD_INCLUDE_COMMUNICATIONS */
