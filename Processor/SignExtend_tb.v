module SignExtend16_tb;
	reg [15:0] in;
	wire[31:0] out;
	
	SignExtend16 test(in, out);

	initial begin
	$monitor("%d , %d",in,out);
	in = 16'h0101;
	#100
	in = 8'b11100111;
	#100
	in = 8'b11110000;
	#100
	in = 8'b00000001;
	end
endmodule