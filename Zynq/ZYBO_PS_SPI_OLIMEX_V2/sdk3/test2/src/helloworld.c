/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xscugic.h"
#include "xspips.h"
#include "xscutimer.h"

#define BUFFER_SIZE 24

//typedef u8 VideoBuffer[BUFFER_SIZE];
u8 WriteBuffer[BUFFER_SIZE];


int main()
{
	int Status;
    init_platform();
    XSpiPs_Config *SPI_ConfigPtr;
    static XSpiPs SpiInstance;

	WriteBuffer[0]	=0b00000111;
	WriteBuffer[1]	=0b00000111;
	WriteBuffer[2]	=0b00000111;
	WriteBuffer[4]	=0b00000111;
	WriteBuffer[5]	=0b00000111;
	WriteBuffer[6]	=0b00000111;
	WriteBuffer[7]	=0b00000111;
	WriteBuffer[8]	=0b00000111;
	WriteBuffer[9]	=0b00000111;
	WriteBuffer[10]	=0b00000111;
	WriteBuffer[11]	=0b00000111;
	WriteBuffer[12]	=0b00000111;
	WriteBuffer[13]	=0b00000111;
	WriteBuffer[14]	=0b00000111;
	WriteBuffer[15]	=0b00000111;
	WriteBuffer[16]	=0b00000111;
	WriteBuffer[17]	=0b00000111;
	WriteBuffer[18]	=0b00000111;
	WriteBuffer[19]	=0b00000111;
	WriteBuffer[20]	=0b00000111;
	WriteBuffer[21]	=0b00000111;
	WriteBuffer[22]	=0b00000111;
	WriteBuffer[23]	=0b00000111;




    SPI_ConfigPtr = XSpiPs_LookupConfig(XPAR_PS7_SPI_1_DEVICE_ID);
    if (SPI_ConfigPtr == NULL) return XST_DEVICE_NOT_FOUND;
    	Status = XSpiPs_CfgInitialize(&SpiInstance, SPI_ConfigPtr, SPI_ConfigPtr->BaseAddress);
    	if (Status != XST_SUCCESS) return XST_FAILURE;

    	// Perform a self-test to check the SPI hardware
    	Status = XSpiPs_SelfTest(&SpiInstance);
    	if (Status != XST_SUCCESS) return XST_FAILURE;

    	// Reset the SPI peripheral
    	XSpiPs_Reset(&SpiInstance);

    	//XSpiPs_SetStatusHandler(&SpiInstance, &SpiInstance, (XSpiPs_StatusHandler)SpiIntrHandler);
    	printf("Setting the SPI device into Master mode...");
    		Status = XSpiPs_SetOptions(&SpiInstance, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION | XSPIPS_CLK_ACTIVE_LOW_OPTION);
    		if (Status != XST_SUCCESS) return XST_FAILURE;
    		printf("DONE!!\n\r");


    		printf("Setting the SPI device CLK pre-scaler...");
    		Status = XSpiPs_SetClkPrescaler(&SpiInstance, XSPIPS_CLK_PRESCALE_64);
    		if (Status != XST_SUCCESS) return XST_FAILURE;
    		printf("DONE!!\n\r");
    		XSpiPs_SetSlaveSelect(&SpiInstance, 0x00);
    		//WriteBuffer[0]= 0b111111111;
    		Status = XSpiPs_Transfer(&SpiInstance, WriteBuffer, NULL, 24);
    		XSpiPs_SetSlaveSelect(&SpiInstance, 0x01);
    print("Hello World\n\r");

    cleanup_platform();
    return 0;
}
