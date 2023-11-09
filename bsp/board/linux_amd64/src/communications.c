#include "communications.h"

#include "stdio.h"
// NOLINTBEGIN
static uint16_t i = 0;
static char atAnswer[] =
    "AT+GMR\r\n\r\nversion v1.2\r\nB_Version v2.3\r\nOK\r\n";

void InitCommunication(CommunicationHandle *handle) {
    printf("Initializing  communication of type %d # %d\n", handle->protocol,
           handle->portNum);
}

void SendData(CommunicationHandle *handle, const uint8_t *data, uint16_t size) {
    printf("Sending data of size %d: %s\n", size, data);
}

void ReceiveData(CommunicationHandle *handle, uint8_t *buffer,
                 uint16_t bufferSize) {
    for (uint16_t j = 0; j < bufferSize; j++, i = (i + 1) % 45) {
        buffer[j] = atAnswer[i];
    }
}
// NOLINTEND