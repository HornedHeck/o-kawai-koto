#ifndef BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX
#define BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX

#include "communications.h"
#include "stdint.h"

typedef struct {
    const char *data;
    const uint16_t size;
} DataSequence;

typedef enum { SET_STATION_MODE = 0, CONNECT_TO_WIFI, GET_IP_ADDR } ATCommand;

uint16_t BlockingRead(const CommunicationHandle *handle, uint8_t *buffer,
                      uint16_t bufferSize, DataSequence start,
                      const DataSequence *ends, uint16_t endsCount);

uint16_t ExecuteCommand(const CommunicationHandle *handle, uint8_t *buffer,
                        uint16_t bufferSize, ATCommand atCommand);

uint16_t ExecuteAtCommand(const CommunicationHandle *handle, uint8_t *buffer,
                          uint16_t bufferSize, const char *atCommand,
                          const uint16_t atCommandSize);

#endif /* BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX */
