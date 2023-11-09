#ifndef APP_COMMUNICATIONS_EX
#define APP_COMMUNICATIONS_EX

#include "communications.h"
#include "stdint.h"

typedef struct {
    uint16_t size;
    char *data;
} DataSequence;

uint16_t BlockingRead(CommunicationHandle *handle, uint8_t *buffer,
                      uint16_t bufferSize, DataSequence start,
                      DataSequence *ends, uint16_t endsCount);

#endif /* APP_COMMUNICATIONS_EX */
