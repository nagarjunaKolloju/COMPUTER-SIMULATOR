module ROM_tb;
	reg [31:0] PC;
	wire [31:0] instruction;

	ROM r(PC, instruction);

	initial begin
		PC = 32'h00000000;
		#100
		PC = 32'h00000004;
		#100
		PC = 32'h00000000;
	end
endmodule