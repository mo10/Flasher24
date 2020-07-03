//
// Created by admin on 2020/07/02.
//

#ifndef FLASHER24_WINUSB_DESC_H
#define FLASHER24_WINUSB_DESC_H

#include "stm32f0xx_hal.h"
#include <usbd_ctlreq.h>
#include <stdint.h>

#define WINUSB_REQ_MS_VENDOR_CODE       0xA5

#define WINUSB_OS_FEATURE_DESC_LEN      16U + 24U
#define WINUSB_OS_PROPERTY_DESC_LEN     0x0AU + 0x6E + 0X32U + 0x84U

#define WINUSB_REQ_TYPE_COMPATID        0x04
#define WINUSB_REQ_TYPE_EXTDESC         0x05

#define TO_STRING(x) #x


uint8_t *WinUSB_Get_OSStringDesc(uint16_t *length);
uint8_t *WinUSB_Get_OSFeatureDesc(uint16_t *length);
uint8_t *WinUSB_Get_OSPropertyDesc(uint16_t *length);
#endif //FLASHER24_WINUSB_DESC_H
