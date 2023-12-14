#include "main.h"

#include "basic_operations.h"
#include "communications.h"
#include "encoder.h"
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
    Log("Data Received:\r\n", 16);
    Log(buffer, buffer_size);
    ready_to_send = true;
}

#include "data.h"
#include "stdio.h"

// NOLINTBEGIN
#define OUTPUT "files/t7.bin"

#define FRAMES_COUNT (60 * 1000 / TIMESPAN)

int main() {
    InitBoard();

    InitOpusWrapper();
    InitFile(OUTPUT);
    for (size_t i = 0; i < FRAMES_COUNT; i++) {
        size_t read_size = GetPCMData(buffer, BUFFER_SIZE);
        if (read_size != BUFFER_SIZE) {
            printf("Error reading pcm src on i: %lu, got %lu", i, read_size);
            return -1;
        }

        int32_t encoded_size =
            opus_encode(encoder, buffer, FRAME_SIZE, packet, OUTPUT_MAX_SIZE);
        if (encoded_size < 0 || encoded_size > OUTPUT_MAX_SIZE) {
            printf("Error encoding on i: %u, got %ld", i, encoded_size);
            return encoded_size;
        } else if (encoded_size < 3) {
            continue;
        }

        int decoded_size = opus_decode(decoder, packet, encoded_size,
                                       buffer_out, MAX_FRAME_SIZE, 0);

        if (decoded_size < 1) {
            printf("Error decoding on i: %u, got %d", i, decoded_size);
            return decoded_size;
        }

        // AppendData(pcm_buffer, decoded_size * CHANNELS);
        AppendData(buffer_out, decoded_size * CHANNELS);
    }
    CloseFile();

    return 0;
}
// NOLINTEND