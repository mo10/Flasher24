//
// Created by admin on 2020/07/03.
//

#include "winusb_device.h"
#include <usbd_def.h>
#include <usbd_core.h>
#include <usbd_customhid.h>
#include <winusb_desc.h>

static uint8_t  WinUSB_Device_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  WinUSB_Device_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);

static uint8_t  WinUSB_Device_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  WinUSB_Device_EP0_RxReady(USBD_HandleTypeDef  *pdev);

static uint8_t  WinUSB_Device_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  WinUSB_Device_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  *WinUSB_Device_GetFSCfgDesc(uint16_t *length);
static uint8_t  *WinUSB_Device_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t  *WinUSB_Device_GetUsrStrDescriptor(USBD_HandleTypeDef *pdev, uint8_t index,  uint16_t *length);

static void WinUSB_Device_GetVendorDescriptor(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

USBD_ClassTypeDef  WinUSB_Device_Class = {
        WinUSB_Device_Init,
        WinUSB_Device_DeInit,
        WinUSB_Device_Setup,
        NULL, /*EP0_TxSent*/
        WinUSB_Device_EP0_RxReady, /*EP0_RxReady*/
        WinUSB_Device_DataIn, /*DataIn*/
        WinUSB_Device_DataOut,
        NULL, /*SOF */
        NULL,
        NULL,
        NULL,
        WinUSB_Device_GetFSCfgDesc,
        NULL,
        WinUSB_Device_GetDeviceQualifierDesc,
        WinUSB_Device_GetUsrStrDescriptor,
};

/* USB FS Device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t WinUSB_Device_CfgFSDesc[WINUSB_DEVICE_CONFIG_DESC_SIZE] __ALIGN_END = {
        0x09, /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
        WINUSB_DEVICE_CONFIG_DESC_SIZE,  /* wTotalLength: Bytes returned */
        0x00,
        0x01,         /* bNumInterfaces: 1 interface */
        0x01,         /* bConfigurationValue: Configuration value */
        0x00,         /* iConfiguration: Index of string descriptor describing the configuration */
        0xC0,         /* bmAttributes: bus powered */
        0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus */
        /******************** Interface Descriptor ********************/
        /* 09 */
        0x09,         /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE,/* bDescriptorType: Interface descriptor type */
        0x00,         /* bInterfaceNumber: Number of Interface */
        0x00,         /* bAlternateSetting: Alternate setting */
        0x02,         /* bNumEndpoints */
        0xFF,         /* bInterfaceClass: Vendor define */
        0xFF,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
        0xFF,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
        0,            /* iInterface: Index of string descriptor */
        /******************** Endpoint In Descriptor ********************/
        /* 18 */
        0x07,                       /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,     /* bDescriptorType: Endpoint */
        WINUSB_DEVICE_EPIN_ADDR,    /* bEndpointAddress: Endpoint Address (IN) */
        0x02,                       /* bmAttributes: Bulk endpoint */
        WINUSB_DEVICE_EPIN_SIZE,    /* wMaxPacketSize: 64 Byte max */
        0x00,
        0x00,                       /* bInterval: Polling Interval */
        /******************** Endpoint Out Descriptor ********************/
        /* 25 */
        0x07,                       /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,     /* bDescriptorType: Endpoint */
        WINUSB_DEVICE_EPOUT_ADDR,   /* bEndpointAddress: Endpoint Address (OUT) */
        0x02,                       /* bmAttributes: Bulk endpoint */
        WINUSB_DEVICE_EPOUT_SIZE,   /* wMaxPacketSize: 64 Bytes max  */
        0x00,
        0x00,                       /* bInterval: Polling Interval */
        /* 32 */
};
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t WinUSB_Device_QualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
        USB_LEN_DEV_QUALIFIER_DESC,         /* bLength */
        USB_DESC_TYPE_DEVICE_QUALIFIER,     /* bDescriptorType */
        0x00, 0x02,     /* bcdUSB */
        0x00,           /* bDeviceClass */
        0x00,           /* bDeviceSubClass */
        0x00,           /* bDeviceProtocol */
        0x40,           /* bMaxPacketSize0 */
        0x01,           /* bNumConfigurations */
        0x00,           /* bReserved */
};

static uint8_t  WinUSB_Device_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
    USBD_WinUSB_Device_HandleTypeDef *hhid;
    printf("WinUSB_Device_Init\r\n");
    /* Open Endpoints */
    USBD_LL_OpenEP(pdev, WINUSB_DEVICE_EPIN_ADDR, USBD_EP_TYPE_INTR, WINUSB_DEVICE_EPIN_SIZE);
    pdev->ep_in[WINUSB_DEVICE_EPIN_ADDR & 0xFU].is_used = 1U;

    USBD_LL_OpenEP(pdev, WINUSB_DEVICE_EPOUT_ADDR, USBD_EP_TYPE_INTR, WINUSB_DEVICE_EPOUT_SIZE);
    pdev->ep_out[WINUSB_DEVICE_EPOUT_ADDR & 0xFU].is_used = 1U;

    pdev->pClassData = USBD_malloc(sizeof(USBD_CUSTOM_HID_HandleTypeDef));
    if (pdev->pClassData == NULL) {
        return 1U;
    }

    /* 预接收第一个包 */
    USBD_LL_PrepareReceive(pdev, WINUSB_DEVICE_EPOUT_ADDR, hhid->Report_buf,64);

    return 0U;
}
static uint8_t  WinUSB_Device_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
    /* Close Endpoints */
    USBD_LL_CloseEP(pdev, WINUSB_DEVICE_EPIN_ADDR);
    pdev->ep_in[WINUSB_DEVICE_EPIN_ADDR & 0xFU].is_used = 0U;

    USBD_LL_CloseEP(pdev, WINUSB_DEVICE_EPOUT_ADDR);
    pdev->ep_out[WINUSB_DEVICE_EPOUT_ADDR & 0xFU].is_used = 0U;

    /* Free allocated memory */
    if (pdev->pClassData != NULL) {
        ((USBD_WinUSB_Device_ItfTypeDef *) pdev->pUserData)->DeInit();
        USBD_free(pdev->pClassData);
        pdev->pClassData = NULL;
    }
    return USBD_OK;
}
static uint8_t  WinUSB_Device_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {
    USBD_WinUSB_Device_HandleTypeDef *hhid = (USBD_WinUSB_Device_HandleTypeDef *) pdev->pClassData;
    uint16_t len = 0U;
    uint8_t *pbuf = NULL;
    uint16_t status_info = 0U;
    uint8_t ret = USBD_OK;

    printf("WinUSB_Device_Setup\r\n");
    printf("├bmRequest:%#02X\r\n", req->bmRequest);
    printf("└Masked bmRequest:%#02X\r\n", req->bmRequest & USB_REQ_TYPE_MASK);

    switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_VENDOR:
            printf(" └USB_REQ_TYPE_VENDOR bRequest:%#02X\r\n",req->bRequest);
            WinUSB_Device_GetVendorDescriptor(pdev, req);
            break;

        case USB_REQ_TYPE_STANDARD:
            printf(" └USB_REQ_TYPE_STANDARD bRequest:%#02X\r\n",req->bRequest);
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;

        default:
            printf(" └USB_REQ_TYPE_UNKNOWN bRequest:%#02X\r\n",req->bRequest);
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
    }
    return ret;
}


static uint8_t  *WinUSB_Device_GetFSCfgDesc(uint16_t *length)
{
//    printf("WinUSB_Device_GetFSCfgDesc\r\n");

    *length = sizeof(WinUSB_Device_CfgFSDesc);
    return WinUSB_Device_CfgFSDesc;
}

static uint8_t  WinUSB_Device_DataIn(USBD_HandleTypeDef *pdev,
                                       uint8_t epnum) {
    printf("WinUSB_Device_DataIn\r\n");

    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */
    ((USBD_WinUSB_Device_HandleTypeDef *) pdev->pClassData)->state = CUSTOM_HID_IDLE;

    return USBD_OK;
}

static uint8_t  WinUSB_Device_DataOut(USBD_HandleTypeDef *pdev,
                                        uint8_t epnum) {
    printf("WinUSB_Device_DataOut\r\n");

    USBD_WinUSB_Device_HandleTypeDef *hhid = (USBD_WinUSB_Device_HandleTypeDef *) pdev->pClassData;

    ((USBD_WinUSB_Device_ItfTypeDef *) pdev->pUserData)->OutEvent(hhid->Report_buf[0],
                                                               hhid->Report_buf[1]);

    USBD_LL_PrepareReceive(pdev, WINUSB_DEVICE_EPOUT_ADDR, hhid->Report_buf,
                           USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);

    return USBD_OK;
}

static uint8_t WinUSB_Device_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
    printf("WinUSB_Device_EP0_RxReady\r\n");

    USBD_WinUSB_Device_HandleTypeDef     *hhid = (USBD_WinUSB_Device_HandleTypeDef *)pdev->pClassData;

    if (hhid->IsReportAvailable == 1U)
    {
        ((USBD_WinUSB_Device_ItfTypeDef *)pdev->pUserData)->OutEvent(hhid->Report_buf[0],
                                                                  hhid->Report_buf[1]);
        hhid->IsReportAvailable = 0U;
    }

    return USBD_OK;
}

static uint8_t  *WinUSB_Device_GetDeviceQualifierDesc(uint16_t *length)
{
    printf("WinUSB_Device_GetDeviceQualifierDesc\r\n");

    *length = sizeof(WinUSB_Device_GetDeviceQualifierDesc);
    return WinUSB_Device_QualifierDesc;
}

static uint8_t  *WinUSB_Device_GetUsrStrDescriptor(USBD_HandleTypeDef *pdev, uint8_t index,  uint16_t *length) {
    printf("WinUSB_Device_GetUsrStrDescriptor\r\n");
    printf("index:%#02X len:%d", index, *length);
    if (index == 0xEE)
        return WinUSB_Get_OSStringDesc(length);
    *length = 0;
    return NULL;
}

static void WinUSB_Device_GetVendorDescriptor(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {
    uint16_t len;
    uint8_t *pbuf;
    printf("  └WinUSB_Device_GetVendorDescriptor wIndex:%#04X\r\n",req->wIndex);
    printf("  +wValue:%#04X\r\n",req->wValue);
    printf("  +wLength:%#04X\r\n",req->wLength);

    switch (req->wIndex) {
        case WINUSB_REQ_TYPE_COMPATID:
            printf("    └WINUSB_REQ_TYPE_COMPATID \r\n");
            pbuf = WinUSB_Get_OSFeatureDesc(&len);
            break;

        case WINUSB_REQ_TYPE_EXTDESC:
            printf("    └WINUSB_REQ_TYPE_EXTDESC\r\n");
            pbuf = WinUSB_Get_OSPropertyDesc(&len);
            break;

        default:
            printf("    └%#02X\r\n",req->wIndex);
            USBD_CtlError(pdev, req);
            return;
    }
    if ((len != 0) && (req->wLength != 0)) {
        len = MIN(len, req->wLength);
        USBD_CtlSendData(pdev, pbuf, len);
    }
}
