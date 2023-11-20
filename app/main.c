#include "main.h"

#include "basic_operations.h"
#include "communications.h"
#include "init_board.h"
#include "network.h"
#include "string.h"

char data[] = "1234567890";

int main() {
    InitBoard();

    // Open OCD fix for UART mess
    Delay(200);

    CommunicationHandle comHandle = {.portNum = 2, .protocol = UART};
    InitCommunication(&comHandle);

    InetAddr addr = {.addr = {192, 168, 0, 233}, .port = 39039};
    InitNetwork(&comHandle);
    Connect(addr);

    NetworkSendData(addr, data, 10);

    while (1) {
        Delay(200);  // NOLINT
    }

    return 0;
}
