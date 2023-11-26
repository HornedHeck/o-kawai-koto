#include "communications.h"

#include <stdio.h>
#include <string.h>

void InitCommunication(CommunicationHandle *handle) {}

static uint8_t OK_RESPONSE[] = "AT+CWMODE=1,0\r\nOK\r\n";
static uint8_t ERROR_RESPONSE[] = "AT+CWMODE=1,0\r\nERROR\r\n";
static uint16_t responsePointer = 0;

ResponseStatus SendData(const CommunicationHandle *handle, const uint8_t *data,
                        uint16_t size) {
    if (handle->portNum == 2) {
        return RESPONSE_OK;
    }
    responsePointer = 0;
}

ResponseStatus ReceiveData(const CommunicationHandle *handle, uint8_t *buffer,
                           uint16_t bufferSize) {
    if (handle->portNum == 2) {
        return RESPONSE_OK;
    }
    memcpy(buffer, OK_RESPONSE, 19);
    return RESPONSE_OK;
}

ResponseStatus ReadByte(const CommunicationHandle *handle, uint8_t *dst) {
    if (handle->portNum == 2) {
        return RESPONSE_OK;
    }
    if (responsePointer < 20)
    {
        *dst = OK_RESPONSE[responsePointer++];
        return RESPONSE_OK;
    }else{ //NOLINT
        return RESPONSE_ERROR;
    }
    
}