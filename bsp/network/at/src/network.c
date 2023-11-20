#include "network.h"

#include "communications_ex.h"
#include "stdbool.h"
#include "string.h"

#define CMD_BUFFER_SIZE 100
#define CONNECT_SIZE    19
#define SEND_SIZE       11

static const CommunicationHandle *hComm;
static uint8_t cmdBuffer[CMD_BUFFER_SIZE];
static const char connectCMD[] = "AT+CIPSTART=\"UDP\",\"";
static const char sendCMD[] = "AT+CIPSEND=";

static uint16_t AppendInetAddr(InetAddr addr, uint8_t *dst, uint16_t startPos);
static uint16_t AppendUnsignedNumber(uint_least16_t number, uint8_t *dst,
                                     uint16_t startPos);

void InitNetwork(const CommunicationHandle *handle) {
    hComm = handle;
    // Set Station Mode

    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, SET_STATION_MODE);
    // Connect to WIFI
    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, CONNECT_TO_WIFI);
    // Retrive IP
    ExecuteCommand(hComm, cmdBuffer, CMD_BUFFER_SIZE, GET_IP_ADDR);
}

void Connect(InetAddr addr) {
    memcpy(cmdBuffer, connectCMD, CONNECT_SIZE);
    uint16_t pos = AppendInetAddr(addr, cmdBuffer, CONNECT_SIZE) + CONNECT_SIZE;
    cmdBuffer[pos++] = '"';
    cmdBuffer[pos++] = ',';
    pos += AppendUnsignedNumber(addr.port, cmdBuffer, pos);
    SendData(hComm, cmdBuffer, pos);
}

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize) {
    // Send AT+CIPSEND=<length>
    memcpy(cmdBuffer, sendCMD, SEND_SIZE);
    uint16_t pos =
        AppendUnsignedNumber(dataSize, cmdBuffer, SEND_SIZE) + SEND_SIZE;
    cmdBuffer[pos++] = '\r';
    cmdBuffer[pos++] = '\n';
    SendData(hComm, cmdBuffer, pos);
    // Wait for >
    bool readyToSend = false;
    uint8_t res = 0;
    while (res == 0 && !readyToSend) {
        res = ReadByte(hComm, cmdBuffer);
        readyToSend = cmdBuffer[0] == '>';
    }
    if (readyToSend) {
        // Sendig data
        SendData(hComm, data, dataSize);
    }
}

static uint16_t AppendInetAddr(InetAddr addr, uint8_t *dst, uint16_t startPos) {
    uint16_t pos = startPos;
    pos += AppendUnsignedNumber(addr.addr[0], cmdBuffer, pos);
    for (int i = 1; i < ADDR_SIZE; i++) {
        dst[pos++] = '.';
        pos += AppendUnsignedNumber(addr.addr[i], cmdBuffer, pos);
    }
    return pos - startPos;
}

static uint16_t AppendUnsignedNumber(uint_least16_t number, uint8_t *dst,
                                     uint16_t startPos) {
    uint16_t counter = 0;
    do {
        cmdBuffer[startPos + counter++] = number % 10 + '0';
        number /= 10;
    } while (number > 0);

    for (int i = 0; i < counter / 2; i++) {
        uint8_t tmp = cmdBuffer[startPos + i];
        cmdBuffer[startPos + i] = cmdBuffer[startPos + counter - i - 1];
        cmdBuffer[startPos + counter - i - 1] = tmp;
    }
    return counter;
}