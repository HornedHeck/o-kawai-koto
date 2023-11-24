#include "main.h"

#include "basic_operations.h"
#include "communications.h"
#include "init_board.h"
#include "log.h"
#include "network.h"
#include "string.h"

int main() {
    InitBoard();

    // Open OCD fix for UART mess
    Delay(1000);

    CommunicationHandle comHandle = {.portNum = 2, .protocol = UART};
    InitCommunication(&comHandle);
    InitLog(&comHandle);

    // InetAddr addr = {.addr = {192, 168, 0, 233}, .port = 39039};
    // InitNetwork(&comHandle);
    // Connect(addr);

    // NetworkSendData(addr, data, 10);

    while (1) {
        Log("Idling...\r\n", 11);
        Delay(1000);  // NOLINT
    }

    return 0;
}
