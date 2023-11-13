#include "communications.h"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"

UART_HandleTypeDef huart2;

void InitCommunication(CommunicationHandle *handle) {
    if (handle->protocol == UART && handle->portNum == 2) {
        huart2.Instance = USART2;
        huart2.Init.BaudRate = 115200;  // NOLINT
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart2) == HAL_OK) {
            handle->hwHandle = &huart2;
        }
    }
}

void SendData(const CommunicationHandle *handle, const uint8_t *data,
              uint16_t size) {
    HAL_UART_Transmit(handle->hwHandle, data, size, 50);  // NOLINT
}

void ReceiveData(const CommunicationHandle *handle, uint8_t *buffer,
                 uint16_t bufferSize) {
    HAL_UART_Receive(handle->hwHandle, buffer, bufferSize, 1000);  // NOLINT
}