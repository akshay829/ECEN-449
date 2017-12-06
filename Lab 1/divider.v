`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Texas A&M University ECEN 449
// Engineer: Christopher Padilla
// 
// Create Date: 09/07/2017 07:02:37 PM
// Module Name: divider
// Project Name: Lab 1

// 
// Revision:
// Revision 0.01 - File Created
// 
//////////////////////////////////////////////////////////////////////////////////

//This is how I impletemented my clock divdider module
//returns a slowed down version of clock called out_clk
module divider(
    output reg out_clk, input clock, input reset
    );
	//this will up to a 32 bit number, so we define the parameter as such
    parameter n = 31;
    
	//n-bit number that will count up a desired value
    reg[n:0] counter;
    
	//every posedge of ZYBO's clock (125 MHz) this is triggered
    always@(posedge clock) begin
    //reset if reset button is pressed
   
		//we will count up to 125,000,000 for part one
		//(10,000,000 for part two)
        if(counter == 25000000) begin
            //flick on out new clock
			out_clk <= 1;
            counter <= 0;
        end
        else begin
			//if we're not at value 1250000000 keep out_clock low
            out_clk <= 0;
            counter <= counter + 1;
        end   
        end    
    
    
        
endmodule
