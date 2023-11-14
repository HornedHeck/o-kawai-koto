#include "network.h"

#include "communications_ex.h"
#define CMD_BUFFER_SIZE 100

static const CommunicationHandle *hComm;
static uint8_t cmdBuffer[CMD_BUFFER_SIZE];

void Init(const CommunicationHandle *handle) {
    hComm = handle;
    // Set Station Mode
    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, SET_STATION_MODE);
    // Connect to WIFI
    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, CONNECT_TO_WIFI);
    // Retrive IP
    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, GET_IP_ADDR);
}

void Connect(InetAddr addr) {}

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize) {}