#include "network.h"

#include "basic_operations.h"
#include "communications_ex.h"
#include "detectors.h"
#include "log.h"
#include "stdbool.h"
#include "string.h"

#define CMD_BUFFER_SIZE  100
#define READ_BUFFER_SIZE 4096

#define CMD_STATION_MODE             "AT+CWMODE=1,0\r\n"
#define CMD_STATION_MODE_SIZE        (sizeof CMD_STATION_MODE - 1)
#define CMD_CONNECT_WIFI             "AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PWD "\"\r\n"
#define CMD_CONNECT_WIFI_SIZE        (sizeof CMD_CONNECT_WIFI - 1)
#define CMD_GET_IP                   "AT+CIPSTA?\r\n"
#define CMD_GET_IP_SIZE              (sizeof CMD_GET_IP - 1)
#define CMD_UDP_CONNECT              "AT+CIPSTART=\"UDP\",\""
#define CMD_UDP_CONNECT_SIZE         (sizeof CMD_UDP_CONNECT - 1)
#define CMD_UDP_SEND                 "AT+CIPSEND="
#define CMD_UDP_SEND_SIZE            (sizeof CMD_UDP_SEND - 1)
#define CMD_ENABLE_PASSIVE_MODE      "AT+CIPRECVMODE=1\r\n"
#define CMD_ENABLE_PASSIVE_MODE_SIZE (sizeof CMD_ENABLE_PASSIVE_MODE - 1)

// State
static CommunicationHandle *kHCom;
static DataCallback kCallback;
static enum State {
    FRESH,
    INIT,
    SENDING_DATA,
    AWAITING_DATA,
    RECEIVING_DATA_LENGTH,
    RECEIVING_DATA,
    CONSUMING_DATA,
} state = FRESH;

// Buffers
static uint8_t cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t read_buffer[READ_BUFFER_SIZE];

// IT mode variables
static bool is_it_enabled = false;
static bool is_ready_to_send = false;
static bool is_send_error = false;
static bool is_ipd_detected = false;
static uint8_t it_data_dst;

// Data receiving variables
static uint16_t data_counter = 0;
static uint16_t data_length = 0;
static bool data_length_ready = true;

static uint16_t AppendInetAddr(InetAddr addr, uint8_t *dst, uint16_t startPos);

static uint16_t AppendUnsignedNumber(uint_least16_t number, uint8_t *dst,
                                     uint16_t startPos);

static bool ParseLength(uint8_t symbol);

bool DataReadyCallback() {
    switch (state) {
        case FRESH:
        case INIT:
            return false;
            break;
        case SENDING_DATA:
            is_send_error = Detect(it_data_dst, DETECTABLE_ERROR);
            is_ready_to_send = Detect(it_data_dst, DETECTABLE_READY_FOR_DATA);
            if (is_ready_to_send || is_send_error) {
                // Enable await data state to process +IPD;
                state = AWAITING_DATA;
            }
            break;
        case AWAITING_DATA:
            if (Detect(it_data_dst, DETECTABLE_INCOMING_DATA)) {
                state = RECEIVING_DATA_LENGTH;
            }
            break;
        case RECEIVING_DATA_LENGTH:
            if (!ParseLength(it_data_dst)) {
                state = RECEIVING_DATA;
            }
            break;
        case RECEIVING_DATA:
            if (data_counter < data_length) {
                read_buffer[data_counter++] = it_data_dst;
            } else {
                data_counter = 0;
                state = CONSUMING_DATA;
            }
            break;
        case CONSUMING_DATA:
            break;
    }
    return true;
}

void InitNetwork(CommunicationHandle *hCom, const DataCallback callback) {
    if (state != FRESH) {
        return;
    }

    kHCom = hCom;
    kCallback = callback;

    uint16_t bytes_read = 0;
    // Set Station Mode
    if (Execute(kHCom, CMD_STATION_MODE, CMD_STATION_MODE_SIZE, read_buffer,
                READ_BUFFER_SIZE, &bytes_read) != RESPONSE_OK) {
        Log("Error response on Set Station mode\r\n", 36);
        return;
    };
    // Connect to WIFI
    if (Execute(kHCom, CMD_CONNECT_WIFI, CMD_CONNECT_WIFI_SIZE, read_buffer,
                READ_BUFFER_SIZE, &bytes_read) != RESPONSE_OK) {
        Log("Error response on Connect to Wifi\r\n", 35);
        return;
    }
    // Retrive IP
    if (Execute(kHCom, CMD_GET_IP, CMD_GET_IP_SIZE, read_buffer,
                READ_BUFFER_SIZE, &bytes_read) != RESPONSE_OK) {
        Log("Error response on Get IP\r\n", 26);
        return;
    }

    // Enable passive mode
    // if (Execute(kHCom, CMD_ENABLE_PASSIVE_MODE, CMD_ENABLE_PASSIVE_MODE_SIZE,
    //             read_buffer, READ_BUFFER_SIZE, &bytes_read) != RESPONSE_OK) {
    //     Log("Error response on Set Passive mode\r\n", 36);
    //     return;
    // }
    state = INIT;
}

void Connect(InetAddr addr) {
    if (state != INIT) {
        return;
    }

    memcpy(cmd_buffer, CMD_UDP_CONNECT, CMD_UDP_CONNECT_SIZE);
    uint16_t pos = CMD_UDP_CONNECT_SIZE;
    pos += AppendInetAddr(addr, cmd_buffer, pos);
    cmd_buffer[pos++] = '"';
    cmd_buffer[pos++] = ',';
    pos += AppendUnsignedNumber(addr.port, cmd_buffer, pos);
    cmd_buffer[pos++] = '\r';
    cmd_buffer[pos++] = '\n';

    uint16_t bytesRead = 0;
    ResponseStatus response = Execute(kHCom, cmd_buffer, pos, read_buffer,
                                      READ_BUFFER_SIZE, &bytesRead);
    if (response == RESPONSE_OK) {
        state = AWAITING_DATA;
        EnableITReceive(kHCom, &it_data_dst, 1, &DataReadyCallback);
        is_it_enabled = true;
    }
}

void NetworkSendData(InetAddr addr, uint8_t *data, uint16_t dataSize) {
    if (state != AWAITING_DATA) {
        return;
    }
    state = SENDING_DATA;

    // Send AT+CIPSEND=<length>
    memcpy(cmd_buffer, CMD_UDP_SEND, CMD_UDP_SEND_SIZE);
    uint16_t pos = CMD_UDP_SEND_SIZE;
    pos += AppendUnsignedNumber(dataSize, cmd_buffer, CMD_UDP_SEND_SIZE);
    cmd_buffer[pos++] = '\r';
    cmd_buffer[pos++] = '\n';
    SendData(kHCom, cmd_buffer, pos);

    // Wait for >

    /* bool readyToSend = false;
    ResponseStatus res = RESPONSE_OK;
    while (res == RESPONSE_OK && !readyToSend) {
        res = ReadByte(kHCom, cmd_buffer);
        readyToSend = cmd_buffer[0] == '>';
    } */

    uint16_t timeout = GetTick() + 500U;
    while (!(is_ready_to_send || is_send_error) && GetTick() < timeout) {
        Delay(1);
    }

    // Sendig data
    if (is_ready_to_send) {
        SendData(kHCom, data, dataSize);
    }
}

void Tick() {
    if (state == CONSUMING_DATA) {
        kCallback(read_buffer, data_length);
        data_length = 0;
        state = AWAITING_DATA;
    }
}

static uint16_t AppendInetAddr(InetAddr addr, uint8_t *dst, uint16_t startPos) {
    uint16_t pos = startPos;
    pos += AppendUnsignedNumber(addr.addr[0], cmd_buffer, pos);
    for (int i = 1; i < ADDR_SIZE; i++) {
        dst[pos++] = '.';
        pos += AppendUnsignedNumber(addr.addr[i], cmd_buffer, pos);
    }
    return pos - startPos;
}

static uint16_t AppendUnsignedNumber(uint_least16_t number, uint8_t *dst,
                                     uint16_t startPos) {
    uint16_t counter = 0;
    do {
        cmd_buffer[startPos + counter++] = number % 10 + '0';
        number /= 10;
    } while (number > 0);

    for (int i = 0; i < counter / 2; i++) {
        uint8_t tmp = cmd_buffer[startPos + i];
        cmd_buffer[startPos + i] = cmd_buffer[startPos + counter - i - 1];
        cmd_buffer[startPos + counter - i - 1] = tmp;
    }
    return counter;
}

static bool ParseLength(uint8_t symbol) {
    if (symbol > '9' || symbol < '0') {
        data_length_ready = true;
        return false;
    }
    data_length = data_length * 10 + (symbol - '0');
    return true;
};