module RAM(
	input [31:0] address,
	input [31:0] in,
	input CONTROL_MEM_READ,
	input CONTROL_MEM_WRITE,
	output reg [31:0] out);

	reg [31:0] memory [31:0];

	always @(address or in or CONTROL_MEM_READ or CONTROL_MEM_WRITE) begin
		$readmemb("_ram.mem",memory,0,31);
		if (CONTROL_MEM_WRITE == 1) begin memory[address] = in; end
		if (CONTROL_MEM_READ == 1) begin out = memory[address]; end
		$writememb("_ram.mem",memory,0,31);
	end
endmodule