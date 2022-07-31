
module SignExtend16(
	input [15:0] in,
	output reg [31:0] out);
	
	always @(in) begin
		out[15:0] = in;
		out[31:16] = {32{in[15]}};
	end
endmodule