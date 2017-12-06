//Chris Padilla
#include <xparameters.h>
#include <xgpio.h>
#include <xstatus.h>
#include <xil_printf.h>

//Definitions
#define GPIO_DEVICE_ID XPAR_LED_DEVICE_ID //GPIO device that LEDs are connected to
#define WAIT_VAL 10000000 //wait value used to divide the clock

int delay (void);

int main() {
	int count; //how many times we go through the while loop in total
	int count_masked; //the first 4 bits (what is shown on the LEDs)
	XGpio leds; //GPIO output led port
	int status; //tells us if we successfully initialize the GPIO device
	
	status = XGpio_Initialize(&leds, GPIO_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 1, 0x00); //data direction (1 for input, 0 for output)
	if (status != XST_SUCCESS) {
		xil_printf("Initialization failed");
	}
	count = 0;//define count to be 0 before the loop
	while(1) { //the loop repeats until the user manually terminates
		count_masked = count & 0xF; //count bitwise and with 15 to only display first 4 bits on LEDs
		XGpio_DiscreteWrite(&leds, 1, count_masked); //DiscreteWrite is used to give values to outputs (LEDs in this case taking the value of count_masked)
		xil_printf("Value of LEDs = 0x%x\n\r", count_masked); //console output as a checkpoint
		delay(); //delay by 10,000,000 cycles through while loop
		count++; //increment count
	}
	return(0);
}

int delay(void) {
	volatile int delay_count = 0; //volatile prevents compiler from applying optimizations to changing variables
	while(delay_count < WAIT_VAL)
		delay_count++; //keep looping until we reach WAIT_VAL (10,000,000 cycles define above)
	return (0);
}