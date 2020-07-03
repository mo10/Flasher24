//
// Created by Mo10 on 2020/07/02.
//

#include <stdint.h>
#include <usart.h>
#include <stdio.h>
#include <usbd_core.h>
#include <usbd_desc.h>
#include <winusb_device.h>

int __io_putchar(uint8_t ch){
    HAL_UART_Transmit(&huart1, (uint8_t * ) & ch, 1, 0xFFFF);
    return ch;
}

extern  USBD_HandleTypeDef hUsbDeviceFS;

void App_Init(){

    printf("hello world\r\n");
    USBD_Stop(&hUsbDeviceFS);
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
    {
        Error_Handler();
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &WinUSB_Device_Class) != USBD_OK)
    {
        Error_Handler();
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
        Error_Handler();
    }
}
