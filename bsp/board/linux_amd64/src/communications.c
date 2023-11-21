#include "communications.h"

#include <stdio.h>
#include <string.h>

#define DATA_LENGTH 9
#define OK_LENGTH   4

static char internalBuf[1024];
static char body[DATA_LENGTH] = "SomeData";
static char responseOK[OK_LENGTH] = {'O', 'K', '\r', '\n'};
void InitCommunication(CommunicationHandle *handle) {
    printf("Initializing  communication of type %d # %d\n", handle->protocol,
           handle->portNum);
}

static size_t lastWrite = 0;

void SendData(const CommunicationHandle *handle, const uint8_t *data,
              uint16_t size) {
    memcpy(internalBuf, data, size);  // NOLINT
    lastWrite = size;
    printf("Sending data of size %d: %s\n", size, data);
}

void ReceiveData(const CommunicationHandle *handle, uint8_t *buffer,
                 uint16_t bufferSize) {
    memcpy(buffer, internalBuf, lastWrite);
    memcpy(buffer + lastWrite, body, DATA_LENGTH);
    memcpy(buffer + lastWrite + DATA_LENGTH, responseOK, OK_LENGTH);
}