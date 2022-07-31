
module RegisterFile(
	input [4:0] readReg1,
	input [4:0] readReg2,
	input [4:0] writeReg,
	input [31:0] writeData,
	input CONTROL_REG_WRITE,
	output reg[31:0] readData1,
	output reg[31:0] readData2);

	reg [31:0] memory[31:0];

	always @(readReg1, readReg2 , writeReg , writeData , CONTROL_REG_WRITE) begin
		$readmemb("_registers.mem",memory,0,31);
		readData1 = memory[readReg1];
		readData2 = memory[readReg2];
		if(CONTROL_REG_WRITE == 1) begin #1 memory[writeReg] = writeData; end
		$writememb("_registers.mem",memory,0,31);
	end
endmodule 

