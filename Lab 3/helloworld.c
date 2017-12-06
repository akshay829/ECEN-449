/*Created by Chris Padilla for ECEN 449 Lab 3*/
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
#include <xparameters.h>
#include <multiply.h>
#include "platform.h"

void print(char *str);

int main() {
    init_platform();
	int result; //result of multiplication
    //NOTE TO ME: MULTIPLY_mWriteReg(BaseAddress, RegOffset, Data);
    
	int i; //in C, for loop index needs to be delcared externally
    for (i = 0; i <= 16; i++) { //squares all numbers from 0 to 16 and displays results to console
		//first we load our firt number into slv_reg0 (base + offset 0)
    	MULTIPLY_mWriteReg(XPAR_MULTIPLY_0_S00_AXI_BASEADDR, 0, i);
    	printf("%d stored in slv_reg0\n", i);
    	
		//then we load our second number into slv_reg1 (base + offset 4)
		MULTIPLY_mWriteReg(XPAR_MULTIPLY_0_S00_AXI_BASEADDR, 4, i);
    	printf("%d stored in slv_reg1\n", i);
    	//NOTE TO ME: MULTIPLY_mReadReg
		//lastly, load into slv_reg2 (base + offset 8)
    	result = MULTIPLY_mReadReg(XPAR_MULTIPLY_0_S00_AXI_BASEADDR, 8);
    	printf("The result is: %d\n\n", result);
    }

    cleanup_platform();
    return 0;
}
