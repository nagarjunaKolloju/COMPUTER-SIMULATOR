module Adder_tb;
	reg [31:0] i1;
	reg [31:0] i2;
	wire [31:0] o;

	Adder test(i1, i2, o);
	
	initial begin 
		$monitor("%d,     %d     ,%d",i1,i2,o);
		i1 = 32'h00000001;
		i2 = 32'h00000011;
		#100
		i1 = 32'h00000101;
		i2 = 32'h00001111;
		#100
		i1 = 32'd24;
		i2 = 32'd45;
	end
endmodule 