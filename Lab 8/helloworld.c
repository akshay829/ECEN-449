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
#include "xparameters.h"
#include "ir_demod.h"

void print(char *str);

void pause() {
	long long int i = 0;
	while (i < 10000000) i++;
	//printf("delay over\n");
}

int main() {
    init_platform();

    printf("Hello World\n\n");

    u32 count;
    u32 message;
    int uniqueCount = 0;
    for (;;) { //forever
    	//printf("Interrupt: %d\n", (int)IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG2_OFFSET) & 0x00000001);

    	u32 interrupt = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG2_OFFSET) & 0x00000001;
    	if  (interrupt) {
    		u32 statusControl = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG2_OFFSET);
    		message = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG0_OFFSET);
    		count = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG1_OFFSET);
    		
			printf("IR Signal Received: 0x%x\n", (unsigned int)message);
    		printf("Total messages received: 0x%x\n", (int)count);
    		printf("Value of Status/Control Register: 0x%x\n\n", (unsigned int)statusControl);
    		//
    		IR_DEMOD_mWriteReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG2_OFFSET, 0x80000000);
    		
			printf("Resetting Interrupt...\n");
			statusControl = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG2_OFFSET) & 0x00000001;
			pause(); //uncomment if i want to see the interrupt every time on the oscilloscope
			printf("Value of Status/Control Register: 0x%x\n\n", (unsigned int)statusControl);
		}

    }
    cleanup_platform();
    return 0;
}
