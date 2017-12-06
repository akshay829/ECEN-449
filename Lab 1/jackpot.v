`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/12/2017 07:46:35 PM
// Design Name: 
// Module Name: jackpot
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments: Jackpot machine is a fun game where if you catch
// one of the LEDS while on, you win!
// 
//////////////////////////////////////////////////////////////////////////////////

//jackpot module
module jackpot(output [3:0] LEDS, input CLOCK, 
    input reset, input [3:0] SWITCHES 
    );
	//we want the clock to be fast, but not too fast, so we need 
	//a slowed down version of CLOCK called newClock
    wire newClock;
	//initial state of the LEDS will be 1000
    reg[3:0] lights = 4'b1000;
    
	//clock divider that divides clock by about 10,000,000
    divider div(newClock, CLOCK, reset);
	
	//every cycle of newClock, we will execute this block
    always@(posedge newClock) begin
		//reset condition
        if (reset) begin 
            lights <= 4'b1000;
        end
        
        else begin
		/* finds the next state of the LEDS based on 
		their current state */
            if (lights == 4'b1000 & ~SWITCHES[3]) begin
                lights <= 4'b0100;
            end
            else if (lights == 4'b0100 & ~SWITCHES[2]) begin
                lights <= 4'b0010;
            end
            else if (lights == 4'b0010 & ~SWITCHES[1]) begin
                lights <= 4'b0001;
            end
            else if (lights == 4'b0001 & ~SWITCHES[0]) begin
                lights <= 4'b1000;
            end
            else begin 
			/* we will get to this point if the correspoding switch
			is triggered, so ALL LEDS stay on solidly */
                lights <= 4'b1111;
            end 
        end
           
    end
    

	//continuous assignment for LEDS to lights
    assign LEDS[3:0] = lights[3:0];
    
endmodule
