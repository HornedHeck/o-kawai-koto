#include "main.h"

#include "basic_operations.h"
#include "communications.h"
#include "init_board.h"
#include "log.h"
#include "network.h"
#include "string.h"

const struct {
    uint8_t OK;
    uint8_t ERROR;
    uint8_t TIMEOUT;
} ResponseType = {
    .OK = 0,
    .ERROR = 1,
    .TIMEOUT = 2,
};

void NetworkReceivedCallback(const uint8_t *buffer, uint16_t buffer_size) {}

static uint8_t gmr[] = "AT+GMR\r\n";

#define DATA_SIZE 256
static uint8_t it_read_buffer;
static uint8_t counter = 0;
static uint8_t read_buffer[DATA_SIZE];

bool OnReadCompleted() {
    if (counter < DATA_SIZE) {
        read_buffer[counter++] = it_read_buffer;
    }
    return true;
}

int main() {
    InitBoard();

    // Open OCD fix for UART mess
    Delay(1000);

    CommunicationHandle comHandle = {.portNum = 2, .protocol = UART};
    InitCommunication(&comHandle);
    InitLog(&comHandle);

    CommunicationHandle hUART1 = {.portNum = 1, .protocol = UART};
    InitCommunication(&hUART1);

    Log("\r\nSystem Init complete \r\n", 25);

    InetAddr addr = {.addr = {192, 168, 0, 31}, .port = 39039};
    InitNetwork(&hUART1, &NetworkReceivedCallback);
    Connect(addr);

    // Log("Enable IT Mode\r\n", 16);
    // EnableITReceive(&hUART1, &it_read_buffer, 1, &OnReadCompleted);

    Log("Idling...\r\n", 11);
    while (1) {
        if (counter < DATA_SIZE) {
            NetworkSendData(addr, "Hello world!\r\n", 14);
        }
        Delay(5000);  // NOLINT
    }

    return 0;
}
