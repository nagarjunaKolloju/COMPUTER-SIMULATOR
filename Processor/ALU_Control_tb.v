module ALU_Control_tb;
	reg [5:0] o; reg[5:0] f; wire[5:0] co;
	ALU_Control aluControl(o, f, co);

	initial begin
		o = 6'b000000;
		f = 6'b100100;
		#100
		o = 6'b001000;
		#100
		o = 6'b001001;
		#100
		o = 6'b000000;
	end
endmodule