#include "log.h"

#include "assert.h"

static CommunicationHandle *hComm;

/* Use only UART 2 */
void InitLog(CommunicationHandle *hCommArg) {
    hComm = hCommArg;
}

void LogD(const uint8_t message[], uint16_t size) {
    SendData(hComm, message, size);
}

void LogS(const char message[], uint16_t size) {
    SendData(hComm, (uint8_t *) message, size);
}