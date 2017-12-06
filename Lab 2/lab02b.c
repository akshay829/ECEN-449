// Christopher Padilla
#include <xparameters.h>
#include <xgpio.h>
#include <xstatus.h>
#include <xil_printf.h>

//Definitions
#define GPIO_DEVICE_ID_LEDS XPAR_LED_DEVICE_ID //device IDs
#define GPIO_DEVICE_ID_SWB XPAR_SWB_DEVICE_ID
#define WAIT_VAL 10000000 //100 MHz clock input (we need to divide it by 10 mil to get 1 Hz)
#define boolean _Bool //boolean is more pleasant to type than _Bool
#define false 0 //vivado requires this in order to compile
#define true 1

int delay(void) { //slows the 100 MHz clock down to 1 MHz
	volatile int delay_count = 0; //tells the compiler that this value may change at any time without any action taken
	while (delay_count < WAIT_VAL) {
		delay_count++;
	}
	return (0);
}

int main() {
	int count = 0; //actual count value (may be greater than 15)
	int count_masked = 0; //count_masked will always be between 0 and 15
	XGpio leds; //LEDs
	XGpio swb; //switches/buttons
	int statuso; //verifying satisfactory status of the output port

	statuso = XGpio_Initialize(&leds, GPIO_DEVICE_ID_LEDS);
	XGpio_SetDataDirection(&leds, 1, 0); //0 is for output
	if (statuso != XST_SUCCESS) {
		xil_printf("Initialization failed (LEDs)");
	}

	XGpio_Initialize(&swb, GPIO_DEVICE_ID_SWB);
	XGpio_SetDataDirection(&swb, 1, 1);//1 is for input
	int switchValue = 0;

	//this is how we keep track of whether variables change
	boolean b0 = false;
	boolean b1 = false;
	boolean b2 = false;
	boolean b3 = false;

	for (;;) { //this loop will go until the user terminates the program manually
		boolean button0 = (0x01 & XGpio_DiscreteRead(&swb, 1) == 0x01) //button 0 is currently pressed (overrides button 1 being pressed)
			&& !((XGpio_DiscreteRead(&swb, 1) & 0x04) == 0x04) //NOT button2
			&& !((XGpio_DiscreteRead(&swb, 1) & 0x08) == 0x08); //NOT button3

		boolean button1 = ((XGpio_DiscreteRead(&swb, 1) & 0x02) == 0x02) //button 1 is being pressed
			&& !((XGpio_DiscreteRead(&swb, 1) & 0x04) == 0x04) //NOT button2
			&& !((XGpio_DiscreteRead(&swb, 1) & 0x08) == 0x08); //NOT button3

		boolean button2 = ((XGpio_DiscreteRead(&swb, 1) & 0x04) == 0x04); //button 2 is being pressed

		boolean button3 = ((XGpio_DiscreteRead(&swb, 1) & 0x08) == 0x08); //button 3 is being pressed

		if (!button2 && !button3) XGpio_DiscreteWrite(&leds, 1, 0); //turn off leds when no relevant buttons are pressed

		if (button0) { //bitwise AND with 1 to find value of last bit
			if (!b0) { xil_printf("Button[0] has been pressed!\n"); b0 = true; }
			if (b1) { b1 = false; xil_printf("Button[1] has been released!\n"); }
			if (b2) { b2 = false; xil_printf("Button[2] has been released!\n"); }
			if (b3) { b3 = false; xil_printf("Button[3] has been released!\n"); }
			count++;
			count_masked = count & 0xF;
			xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
			//XGpio_DiscreteWrite(&leds, 1, count_masked);
			delay();
		}
		else if (button1) {
			count--;
			//checking changes of values
			if (!b1) { b1 = true; xil_printf("Button[1] has been pressed!\n"); }
			if (b0) { b0 = false; xil_printf("Button[0] has been released!\n"); }
			if (b2) { b2 = false; xil_printf("Button[2] has been released!\n");  }
			if (b3) { b3 = false; xil_printf("Button[3] has been released!\n");  }

			count_masked = count & 0xF;
			xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
			//XGpio_DiscreteWrite(&leds, 1, count_masked);
			delay();
		}

		else if (button2) { /*button 2 pressed (show switches)*/
			if (b1) { b1 = false; xil_printf("Button[1] has been released!\n"); }
			if (b0) { b0 = false; xil_printf("Button[0] has been released!\n"); }
			if (!b2) { b2 = true; xil_printf("Button[2] has been pressed!\n"); }
			if (b3) { b3 = false; xil_printf("Button[3] has been released!\n");}

			if ((XGpio_DiscreteRead(&swb, 1) & 0xF0) != switchValue) {
				switchValue = XGpio_DiscreteRead(&swb, 1) & 0xF0;
				xil_printf("You moved a switch! Switch Value: %d\n", switchValue >> 4);
			}
			XGpio_DiscreteWrite(&leds, 1, switchValue >> 4);
			//display leds with switch values

			if ((XGpio_DiscreteRead(&swb, 1) & 0x01) == 0x01) { /*button2 is pressed AND button0 is pressed*/
				if (!b0) { b0 = true; xil_printf("Button[0] has been pressed!\n"); }
				count++;
				count_masked = count & 0xF;
				xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
				delay();
			}
			else if ((XGpio_DiscreteRead(&swb, 1) & 0x02) == 0x02) { /*button2 is pressed AND button1 is pressed*/
				if (!b1) { b2 = true; xil_printf("Button[1] has been pressed!\n"); }
				count--;
				count_masked = count & 0xF;
				xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
				delay();
			}

		}
		else if (button3) {
			//same check
			if (b1) { b1 = false; xil_printf("Button[1] has been released!\n"); }
			if (b0) { b0 = false; xil_printf("Button[0] has been released!\n"); }
			if (b2) { b2 = false; xil_printf("Button[2] has been released!\n");}
			if (!b3) { b3 = true; xil_printf("Button[3] has been pressed!\n"); }
			
			if ((XGpio_DiscreteRead(&swb, 1) & 0x01) == 0x01) { //if button 3 AND button 0
				count++;
				count_masked = count & 0xF;
				XGpio_DiscreteWrite(&leds, 1, count_masked);
				xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
				delay();
			}
			else if ((XGpio_DiscreteRead(&swb, 1) & 0x02) == 0x02) { //if button 3 AND button 1
				if (!b1) { b1 = true; xil_printf("Button[1] has been pressed!\n"); }
				count--;
				count_masked = count & 0xF;
				XGpio_DiscreteWrite(&leds, 1, count_masked);
				xil_printf("Value of LEDs = 0x%x\n\r", count_masked);
				delay();
			}
			else { //just button 3 is pressed; we simply display the current count value
				XGpio_DiscreteWrite(&leds, 1, count_masked);
			}

		}
	}
}


