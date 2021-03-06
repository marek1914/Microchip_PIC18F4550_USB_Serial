/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbmmap.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

#ifndef USBMMAP_H
#define USBMMAP_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04                //Buffer Stall enable
#define _DTSEN      0x08                //Data Toggle Synch enable
#define _INCDIS     0x10                //Address increment disable
#define _KEN        0x20                //SIE keeps buff descriptors enable
#define _DAT0       0x00                //DATA0 packet expected next
#define _DAT1       0x40                //DATA1 packet expected next
#define _DTSMASK    0x40                //DTS Mask
#define _USIE       0x80                //SIE owns buffer
#define _UCPU       0x00                //CPU owns buffer

/* USB Device States - To be used with [byte usb_device_state] */
#define DETACHED_STATE          0
#define ATTACHED_STATE          1
#define POWERED_STATE           2
#define DEFAULT_STATE           3
#define ADR_PENDING_STATE       4
#define ADDRESS_STATE           5
#define CONFIGURED_STATE        6

/* Memory Types for Control Transfer - used in USB_DEVICE_STATUS */
#define _RAM 0
#define _ROM 1

/** T Y P E S ****************************************************************/
typedef union _USB_DEVICE_STATUS
{
    byte _byte;
    struct
    {
        unsigned RemoteWakeup:1;// [0]Disabled [1]Enabled: See usbdrv.c,usb9.c
        unsigned ctrl_trf_mem:1;// [0]RAM      [1]ROM
    } a;
} USB_DEVICE_STATUS;

typedef union _BD_STAT
{
    byte _byte;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1;              //Buffer Stall Enable
        unsigned DTSEN:1;               //Data Toggle Synch Enable
        unsigned INCDIS:1;              //Address Increment Disable
        unsigned KEN:1;                 //BD Keep Enable
        unsigned DTS:1;                 //Data Toggle Synch Value
        unsigned UOWN:1;                //USB Ownership
    } a;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned PID0:1;
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned UOWN:1;
    } b;
    struct{
        unsigned :2;
        unsigned PID:4;                 //Packet Identifier
        unsigned :2;
    } c;
} BD_STAT;                              //Buffer Descriptor Status Register

typedef union _BDT
{
    byte _byte[4];
 
	 struct
    {
        BD_STAT Stat;
        byte Cnt;
        byte ADRL;                      //Buffer Address Low
        byte ADRH;                      //Buffer Address High
    } a;
    struct
    {
        byte StatWhole;
        byte Cnt;
        byte* ADR;                      //Buffer Address
    } b;
} BDT;                                  //Buffer Descriptor Table

typedef struct
{
	BDT out;
	BDT in;
} EndpointType;

/** E X T E R N S ************************************************************/
extern byte usb_device_state;
extern USB_DEVICE_STATUS usb_stat;
extern byte usb_active_cfg;
extern byte usb_alt_intf[MAX_NUM_INT];

/******************************************************************************
 * Section A: Buffer Descriptor Table
 * - 0x400 - 0x4FF(max)
 * - MAX_EP_NUMBER is defined in autofiles\usbcfg.h
 * - BDT data type is defined in system\usb\usbmmap.h
 *****************************************************************************/

#ifdef MAX_EP_NUMBER
USB_MEM(EndpointType ep[MAX_EP_NUMBER], 0x400);
#endif

// Buffers are oversize on Hitech trim to size if you need too but be careful !
USB_MEM(CTRL_TRF_SETUP SetupPkt, 0x480);
USB_MEM(CTRL_TRF_DATA CtrlTrfData, 0x4C0);

#if defined(USB_USE_CDC)
USB_MEM(unsigned char cdc_notice[CDC_INT_EP_SIZE], 0x500);
USB_MEM(unsigned char cdc_data_rx[CDC_BULK_OUT_EP_SIZE], 0x600);
USB_MEM(unsigned char cdc_data_tx[CDC_BULK_IN_EP_SIZE], 0x700);
#endif

#endif //USBMMAP_H
