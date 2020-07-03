//
// Created by admin on 2020/07/02.
//

#include "winusb_desc.h"

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t WinUSB_OSFeatureDesc[WINUSB_OS_FEATURE_DESC_LEN] __ALIGN_END = {
        /* Header Section */
        LOBYTE(WINUSB_OS_FEATURE_DESC_LEN),
        HIBYTE(WINUSB_OS_FEATURE_DESC_LEN),
        0, 0,          /* dwLength,Descriptor length */
        0, 1,          /* bcdVersion,BCD Version 1.00 */
        4, 0,          /* wIndex,Extended compat ID descriptor */
        1,             /* bCount,Number of function sections */
        0, 0, 0, 0, 0, 0, 0, /* Reserved */
        /* Function Section */
        0,             /* bFirstInterfaceNumber  */
        1,             /* Reserved */
        'W', 'I', 'N', 'U', 'S', 'B', 0, 0,     /* compatibleID */
        0, 0, 0, 0, 0, 0, 0, 0,                 /* subCompatibleID */
        0, 0, 0, 0, 0, 0, /* Reserved */
};

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t WinUSB_OSPropertyDesc[WINUSB_OS_PROPERTY_DESC_LEN] __ALIGN_END = {
        /* Header Section */
        LOBYTE(WINUSB_OS_PROPERTY_DESC_LEN),
        HIBYTE(WINUSB_OS_PROPERTY_DESC_LEN),
        0, 0,  /* dwLength */
        0x00, 0x01,   /* bcdVersion */
        0x05, 0x00,   /* wIndex */
        0x01, 0x00,   /* wCount */
        /* Custom Property Sections */
        /* Device Icon */
        0x6E, 0x00, 0x00, 0x00,   /* dwSize */
        0x12, 0, 0, 0,   /* dwPropertyDataType */
        0x0C, 0,        /* wPropertyNameLength */
        'I', 0,         /* bPropertyData */
        'c', 0,
        'o', 0,
        'n', 0,
        'P', 0,

        'a', 0,
        't', 0,
        'h', 0,
        0, 0,
        0x4E, 0, 0, 0,  /* dwPropertyDataLength */
        '%', 0,
        'S', 0,
        'y', 0,
        's', 0,
        't', 0,
        'e', 0,
        'm', 0,
        'R', 0,
        'o', 0,
        'o', 0,
        't', 0,
        '%', 0,
        '\\', 0,
        's', 0,
        'y', 0,
        's', 0,
        't', 0,
        'e', 0,
        'm', 0,
        '3', 0,
        '2', 0,
        '\\', 0,
        's', 0,
        'h', 0,
        'e', 0,
        'l', 0,
        'l', 0,
        '3', 0,
        '2', 0,
        '.', 0,
        'd', 0,
        'l', 0,
        'l', 0,
        ',', 0,
        '-', 0,
        '2', 0,
        '3', 0,
        '3', 0,
        0, 0,
        /* Device Label */
        0x32, 0x00, 0x00, 0x00,   /* dwSize */
        0x01, 0, 0, 0,   /* dwPropertyDataType */
        0x0C, 0,        /* wPropertyNameLength */
        'L', 0,         /* bPropertyData */
        'a', 0,
        'b', 0,
        'e', 0,
        'l', 0,
        0, 0,
        0x18, 0, 0, 0,  /* dwPropertyDataLength */
        'M', 0,
        'o', 0,
        '1', 0,
        '0', 0,
        ' ', 0,
        'D', 0,
        'e', 0,
        'v', 0,
        'i', 0,
        'c', 0,
        'e', 0,
        0, 0,
        /* Device Interface GUID */
        0x84, 0x00, 0x00, 0x00,   /* dwSize */
        0x1, 0, 0, 0,   /* dwPropertyDataType */
        0x28, 0,        /* wPropertyNameLength */
        'D', 0,         /* bPropertyData */
        'e', 0,
        'v', 0,
        'i', 0,
        'c', 0,
        'e', 0,
        'I', 0,
        'n', 0,
        't', 0,
        'e', 0,
        'r', 0,
        'f', 0,
        'a', 0,
        'c', 0,
        'e', 0,
        'G', 0,
        'U', 0,
        'I', 0,
        'D', 0,
        0, 0,
        // a34ed565-91c4-49ad-bf74-0c44aff12cf3
        0x4E, 0, 0, 0,  // ; property data length
        '{', 0,
        'a', 0,
        '3', 0,
        '4', 0,
        'e', 0,
        'd', 0,
        '5', 0,
        '6', 0,
        '5', 0,
        '-', 0,
        '9', 0,
        '1', 0,
        'c', 0,
        '4', 0,
        '-', 0,
        '4', 0,
        '9', 0,
        'a', 0,
        'd', 0,
        '-', 0,
        'b', 0,
        'f', 0,
        '7', 0,
        '4', 0,
        '-', 0,
        '0', 0,
        'c', 0,
        '4', 0,
        '4', 0,
        'a', 0,
        'f', 0,
        'f', 0,
        '1', 0,
        '2', 0,
        'c', 0,
        'f', 0,
        '3', 0,
        '}', 0,
        0, 0,

};

static const uint8_t WinUSB_OSString[8] = {
        'M',
        'S',
        'F',
        'T',
        '1',
        '0',
        '0',
        WINUSB_REQ_MS_VENDOR_CODE,
};

uint8_t *WinUSB_Get_OSStringDesc(uint16_t *length) {
    USBD_GetString((uint8_t *) WinUSB_OSString, USBD_StrDesc, length);
    return USBD_StrDesc;
}

uint8_t *WinUSB_Get_OSFeatureDesc(uint16_t *length) {
    *length = WINUSB_OS_FEATURE_DESC_LEN;
    return WinUSB_OSFeatureDesc;
}

uint8_t *WinUSB_Get_OSPropertyDesc(uint16_t *length) {
    *length = WINUSB_OS_PROPERTY_DESC_LEN;
    return WinUSB_OSPropertyDesc;
}

