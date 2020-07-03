//
// Created by admin on 2020/07/03.
//

#ifndef FLASHER24_WINUSB_DEVICE_H
#define FLASHER24_WINUSB_DEVICE_H

/* EPIN */
#include <stdint.h>
#include <usbd_def.h>

#define WINUSB_DEVICE_EPIN_ADDR             0x01U | 0x80U
#define WINUSB_DEVICE_EPIN_SIZE             64
/* EPOUT */
#define WINUSB_DEVICE_EPOUT_ADDR            0x01U
#define WINUSB_DEVICE_EPOUT_SIZE            64

#define WINUSB_DEVICE_CONFIG_DESC_SIZE      9 + 9 + 7 + 7

#ifndef USBD_CUSTOM_OUTREPORT_BUF_SIZE
#define USBD_CUSTOM_OUTREPORT_BUF_SIZE  0x40U /* 64 bytes */
#endif /* USBD_CUSTOM_OUTREPORT_BUF_SIZE */

#ifndef USBD_CUSTOM_REPORT_DESC_SIZE
#define USBD_CUSTOM_REPORT_DESC_SIZE   163U
#endif /* USBD_CUSTOM_REPORT_DESC_SIZE */

#define CUSTOM_DESCRIPTOR_TYPE           0x21U
#define CUSTOM_REPORT_DESC               0x22U

#define CUSTOM_REQ_SET_PROTOCOL          0x0BU
#define CUSTOM_REQ_GET_PROTOCOL          0x03U

#define CUSTOM_REQ_SET_IDLE              0x0AU
#define CUSTOM_REQ_GET_IDLE              0x02U

#define CUSTOM_REQ_SET_REPORT            0x09U
#define CUSTOM_REQ_GET_REPORT            0x01U

typedef enum {
    WinUSB_Device_IDLE = 0U,
    WinUSB_Device_BUSY,
} WinUSB_Device_StateTypeDef;

typedef struct _USBD_WinUSB_Device_Itf {
    uint8_t *pReport;

    int8_t (*Init)(void);

    int8_t (*DeInit)(void);

    int8_t (*OutEvent)(uint8_t event_idx, uint8_t state);

} USBD_WinUSB_Device_ItfTypeDef;

typedef struct {
    uint8_t Report_buf[64];
    uint32_t Protocol;
    uint32_t IdleState;
    uint32_t AltSetting;
    uint32_t IsReportAvailable;
    WinUSB_Device_StateTypeDef state;
} USBD_WinUSB_Device_HandleTypeDef;

extern  USBD_ClassTypeDef  WinUSB_Device_Class;
#endif //FLASHER24_WINUSB_DEVICE_H
