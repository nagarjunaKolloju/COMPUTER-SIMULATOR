module Mux5 (
	input [4:0] in1,
	input [4:0] in2, 
	input sel,
	output reg [4:0] out);
	
	always @(in1 or in2 or sel) begin
		if(sel == 0) begin out = in1; end
		else begin out = in2; end
	end
endmodule 

module Mux32 (
	input [31:0] in1,
	input [31:0] in2, 
	input sel,
	output reg [31:0] out);
	
	always @(in1 or in2 or sel) begin
		if(sel == 0) begin out = in1; end
		else begin out = in2; end
	end
endmodule