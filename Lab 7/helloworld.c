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

int main() {
    init_platform();

    print("Hello World\n\n\r");
    /* Reading from slv_reg0 and slave_reg1 */
    u32 count = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG1_OFFSET);
    u32 message = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG0_OFFSET);

    /* Old message is used to detect a change
     *	If there is a change, we ouput the new message
     *	to the terminal */
    u32 oldMsg =  message;

    int uniqueCount = 0;
    for (;;) { //forever
    	message = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG0_OFFSET);
    	count = IR_DEMOD_mReadReg((u32)XPAR_IR_DEMOD_0_S00_AXI_BASEADDR, IR_DEMOD_S00_AXI_SLV_REG1_OFFSET);
    	if (message != oldMsg) {
    		uniqueCount++;
    		printf("IR Signal Received: 0x%x\n", (unsigned int)message);
    		printf("Total messages received: 0x%x\n", (unsigned int)count);
    		printf("Unique messages received: 0x%x\n\n", (unsigned int)uniqueCount);
    	}
    	oldMsg = message;

    }
    cleanup_platform();
    return 0;
}
