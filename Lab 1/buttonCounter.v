`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Texas A&M University ECEN 449
// Engineer: Christopher Padilla
// 
// Create Date: 09/07/2017 07:02:37 PM
// Module Name: buttonCounter
// Project Name: Lab 1

// 
// Revision:
// Revision 0.01 - File Created
// 
//////////////////////////////////////////////////////////////////////////////////


module buttonCounter(output [3:0] LEDS,
    input CLOCK, input reset, input [0:1] BUTTONS
    );
	//newClock is going to be our slowed-down clock 
    wire newClock;
	/* counter that counts up from 0 to 15 (values that will be displayed)
	on the LEDs */
    reg [3:0] count;
	//call divider module (see module for comments)
    divider newclock(newClock, CLOCK, reset);
    
	//every second (posedge of newClock) this block will execute
    always@(posedge newClock) begin
        if(reset) begin
                count <= 0;
        end
        
		/* if not resetting, we will count up if button 0 is pressed 
		and down if button 1 is pressed */
        else begin
				//all of this computation is done using binary
                if(BUTTONS[0])
                    count <= count + 1;
                if(BUTTONS[1])
                    count <= count - 1;      
        end
        
        
        
    end
    
	//continuous assignment to LEDS using the count value
    assign LEDS[3:0] = count[3:0];
endmodule
