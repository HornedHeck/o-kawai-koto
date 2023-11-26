#include "network.h"

#include "communications_ex.h"
#include "log.h"
#include "stdbool.h"
#include "string.h"

#define CMD_BUFFER_SIZE  100
#define READ_BUFFER_SIZE 4096

#define CMD_STATION_MODE      "AT+CWMODE=1,0\r\n"
#define CMD_STATION_MODE_SIZE (sizeof CMD_STATION_MODE - 1)
#define CMD_CONNECT_WIFI      "AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PWD "\"\r\n"
#define CMD_CONNECT_WIFI_SIZE (sizeof CMD_CONNECT_WIFI - 1)
#define CMD_GET_IP            "AT+CIPSTA?\r\n"
#define CMD_GET_IP_SIZE       (sizeof CMD_GET_IP - 1)
#define CMD_UDP_CONNECT       "AT+CIPSTART=\"UDP\",\""
#define CMD_UDP_CONNECT_SIZE  (sizeof CMD_UDP_CONNECT - 1)
#define CMD_UDP_SEND          "AT+CIPSEND="
#define CMD_UDP_SEND_SIZE     (sizeof CMD_UDP_SEND - 1)

static const CommunicationHandle *hComm;
static uint8_t cmdBuffer[CMD_BUFFER_SIZE];
static uint8_t readBuffer[READ_BUFFER_SIZE];

static uint16_t AppendInetAddr(InetAddr addr, uint8_t *dst, uint16_t startPos);
static uint16_t AppendUnsignedNumber(uint_least16_t number, uint8_t *dst,
                                     uint16_t startPos);

void InitNetwork(const CommunicationHandle *handle) {
    hComm = handle;

    uint16_t bytesRead = 0;
    // Set Station Mode
    if (Execute(hComm, CMD_STATION_MODE, CMD_STATION_MODE_SIZE, readBuffer,
                READ_BUFFER_SIZE, &bytesRead) != RESPONSE_OK) {
        Log("Error response on Set Station mode\r\n", 36);
        return;
    };
    // Connect to WIFI
    if (Execute(hComm, CMD_CONNECT_WIFI, CMD_CONNECT_WIFI_SIZE, readBuffer,
                READ_BUFFER_SIZE, &bytesRead) != RESPONSE_OK) {
        Log("Error response on Connect to Wifi\r\n", 35);
        return;
    }
    // Retrive IP
    if (Execute(hComm, CMD_GET_IP, CMD_GET_IP_SIZE, readBuffer,
                READ_BUFFER_SIZE, &bytesRead) != RESPONSE_OK) {
        Log("Error response on Get IP\r\n", 26);
    }
}

void Connect(InetAddr addr) {
    memcpy(cmdBuffer, CMD_UDP_CONNECT, CMD_UDP_CONNECT_SIZE);
    uint16_t pos = CMD_UDP_CONNECT_SIZE;
    pos += AppendInetAddr(addr, cmdBuffer, pos);
    cmdBuffer[pos++] = '"';
    cmdBuffer[pos++] = ',';
    pos += AppendUnsignedNumber(addr.port, cmdBuffer, pos);
    cmdBuffer[pos++] = '\r';
    cmdBuffer[pos++] = '\n';

    uint16_t bytesRead = 0;
    ResponseStatus response =
        Execute(hComm, cmdBuffer, pos, readBuffer, READ_BUFFER_SIZE, &bytesRead);
}

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize) {
    // Send AT+CIPSEND=<length>
    memcpy(cmdBuffer, CMD_UDP_SEND, CMD_UDP_SEND_SIZE);
    uint16_t pos = CMD_UDP_SEND_SIZE;
    pos += AppendUnsignedNumber(dataSize, cmdBuffer, CMD_UDP_SEND_SIZE);
    cmdBuffer[pos++] = '\r';
    cmdBuffer[pos++] = '\n';
    SendData(hComm, cmdBuffer, pos);
    // Wait for >
    bool readyToSend = false;
    ResponseStatus res = RESPONSE_OK;
    while (res == RESPONSE_OK && !readyToSend) {
        res = ReadByte(hComm, cmdBuffer);
        readyToSend = cmdBuffer[0] == '>';
    }
    if (readyToSend) {
        // Sendig data
        SendData(hComm, data, dataSize);
        ResponseStatus response = ReceiveData(hComm, readBuffer, READ_BUFFER_SIZE); 
        Log("Response status: ", 17);
        Log(&response, 1);
        Log("\r\n", 2);
        
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