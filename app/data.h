#ifndef APP_DATA
#define APP_DATA
#include "stdint.h"
#include "stdio.h"
FILE *output, *data;

size_t GetPCMData(uint16_t *buffer, size_t buffer_size) {
    return fread(buffer, 2, buffer_size, data);
}

void InitFile(const char *filename) {
    data = fopen("files/pcm.bin", "r");
    output = fopen(filename, "w");
}

void CloseFile() {
    fclose(output);
    fclose(data);
}

void AppendData(uint16_t *buffer, size_t decoded_len) {
    fwrite(buffer, 2, decoded_len, output);
}

// size_t GetPCMData(uint8_t *buffer, size_t buffer_size, size_t offset) {
//     return fread(buffer, 2 * 2, buffer_size, data);
// }

// void InitFile(const char *filename) {
//     data = fopen("files/pcm.bin", "r");
//     output = fopen(filename, "w");
// }

// void CloseFile() {
//     fclose(output);
//     fclose(data);
// }

// void AppendData(uint8_t *buffer, size_t decoded_len) {
//     fwrite(buffer, 2, decoded_len, output);
// }

#endif /* APP_DATA */
