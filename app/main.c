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
#include "opus.h"
#include "data.h"
#include "stdio.h"
// NOLINTBEGIN

#define TIMESPAN        20
#define FS              48000
#define CHANNELS        2
#define FRAME_SIZE      (FS * TIMESPAN / 1000)
#define BUFFER_SIZE     (FRAME_SIZE * CHANNELS)
#define MAX_FRAME_SIZE  960
#define BUFFER_OUT_SIZE (MAX_FRAME_SIZE * CHANNELS)
#define OUTPUT_MAX_SIZE 4000
#define OUTPUT          "files/t7.bin"

#define FRAMES_COUNT (60 * 1000 / TIMESPAN)

int main() {
    InitBoard();

    printf("Frame time %dms\n", TIMESPAN);
    printf("Clock: %d Hz\n", FS);
    printf("Frame size: %d\n", FRAME_SIZE);
    printf("Channels: %d\n", CHANNELS);

    int opus_size, error;

    uint8_t pcm_buffer[BUFFER_OUT_SIZE * 2] = {0};
    uint16_t buffer[BUFFER_SIZE] = {0};
    uint16_t buffer_out[BUFFER_OUT_SIZE] = {0};
    uint8_t packet[OUTPUT_MAX_SIZE] = {0};

    opus_size = opus_encoder_get_size(CHANNELS);
    uint8_t encoder_mem[opus_size];
    OpusEncoder *encoder = (OpusEncoder *) encoder_mem;
    error = opus_encoder_init(encoder, FS, CHANNELS, OPUS_APPLICATION_AUDIO);
    if (error != 0) {
        printf("Error during int of encoder: %d", error);
        return error;
    }
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(OPUS_BITRATE_MAX));

    opus_size = opus_decoder_get_size(2);
    uint8_t decoder_mem[opus_size];
    OpusDecoder *decoder = (OpusDecoder *) decoder_mem;
    error = opus_decoder_init(decoder, FS, CHANNELS);
    if (error != 0) {
        printf("Error during int of decoder: %d", error);
        return error;
    }

    InitFile(OUTPUT);
    for (size_t i = 0; i < FRAMES_COUNT; i++) {
        size_t read_size =
            GetPCMData(buffer, BUFFER_SIZE);
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
        AppendData(buffer_out, decoded_size * CHANNELS) ;
    }
    CloseFile();

    return 0;
}
// NOLINTEND