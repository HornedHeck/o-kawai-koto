#include "main.h"

#include "basic_operations.h"
#include "communications.h"
#include "init_board.h"

#define BUFFER_SIZE  40   // NOLINT
#define MESSAGE_SIZE 256  // NOLINT

int main() {
    InitBoard();

    CommunicationHandle comHandle = {.portNum = 2, .protocol = UART};
    InitCommunication(&comHandle);

    return 0;
}
