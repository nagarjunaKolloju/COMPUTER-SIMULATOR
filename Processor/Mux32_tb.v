module Mux32_tb;
	reg [31:0] i1;
	reg [31:0] i2;
	wire [31:0] o;
	reg sel;

	Mux32 test(i1, i2, sel, o);
		
	initial begin
		i1 = 32'd0;
		i2 = 32'd1;
		sel = 0;
		#100
		sel = 1;
		#100
		sel = 0;
	end
endmodule 