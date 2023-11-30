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

static bool ready_to_send = true;

void NetworkReceivedCallback(const uint8_t *buffer, uint16_t buffer_size) {
    Log("Data Received:\r\n" , 16);
    Log(buffer, buffer_size);
    ready_to_send = true;
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

    Log("Idling...\r\n", 11);
    while (1) {
        if (ready_to_send) {
            ready_to_send = false;
            NetworkSendData(addr, "Hello world!\r\n", 14);
        }else{
            Tick();
        }
        Delay(500);  // NOLINT
    }

    return 0;
}
