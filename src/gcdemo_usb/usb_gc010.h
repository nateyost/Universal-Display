#ifndef USB_GC010_H
#define USB_GC010_H
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "./USB/usb.h"
#include "./USB/usb_function_cdc.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_device.h"
#include "usb_config.h"

void USBCBWakeFromSuspend(void);
void USBCB_SOF_Handler(void);
void USBCBErrorHandler(void);
void USBCBCheckOtherReq(void);
void USBCBStdSetDscHandler(void);
void USBCBInitEP(void);
void USBCBSendResume(void);
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size);
void usb_gc010_io_processor(void);
void printf_usb(const char *format, ...);

#endif //USB_GC010_H
