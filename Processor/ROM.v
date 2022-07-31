
module ROM (
	input [31:0] PC,
	output reg [31:0] instruction);

	reg [7:0] memory [1000:0];

	// initial begin 

	// 	memory[0] = 8'b00111100; // lui $r0, 3
	// 	memory[1] = 8'b00000010;
	// 	memory[2] = 8'b00000000;
	// 	memory[3] = 8'b00000011;

	// 	memory[4] = 8'b00000000; // add $r1, $r0, $r0
	// 	memory[5] = 8'b01000010;
	// 	memory[6] = 8'b00011000;
	// 	memory[7] = 8'b00100000;

	always @(PC) begin
		$readmemb("_rom.mem",memory);
		instruction[7:0] = memory[PC+3];
		instruction[15:8] = memory[PC+2];
		instruction[23:16] = memory[PC+1];
		instruction[31:24] = memory[PC];
	end
endmodule 