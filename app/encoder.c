#include "encoder.h"

#include "opus.h"
#include "stdbool.h"

static bool is_initialized = false;
static OpusEncoder *encoder;
static OpusDecoder *decoder;

void InitEncoder() {
    if (is_initialized) {
        return;
    }
    int error = 0;
    encoder = opus_encoder_create(FS, CHANNELS, OPUS_APPLICATION_AUDIO, &error);
    decoder = opus_decoder_create(FS, CHANNELS, &error);
    is_initialized = true;
}

int16_t EncodePacket(const uint16_t *pcm, uint8_t *encoded_data) {
    int32_t encode_result =
        opus_encode(encoder, pcm, FRAME_SIZE, encoded_data, OUTPUT_MAX_SIZE);
    return (int16_t) encode_result;
}

int16_t DecodePacket(const uint8_t *encoded_data, uint16_t encoded_size,
                     uint16_t *pcm) {
    return (int16_t) opus_decode(decoder, encoded_data, encoded_size, pcm,
                                 FRAME_SIZE, FS);
}

bool IsEncodeResultOk(int16_t result) {
    return result >= 0;
}

bool IsDecodeResultOk(int16_t result) {
    return result >= 0;
}

bool IsNeedToSend(int16_t result) {
    return result >= 3;
}
