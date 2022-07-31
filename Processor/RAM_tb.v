module RAM_tb;
	reg [31:0] address;
	reg [31:0] in;
	reg CONTROL_MEM_READ;
	reg CONTROL_MEM_WRITE;
	wire [31:0] out;

	RAM r(address,in,CONTROL_MEM_READ,CONTROL_MEM_WRITE,out);
	
	initial begin
	address = 32'h00000000;
	CONTROL_MEM_WRITE = 1;
	in = 32'h0f0f0f0f;
	#100
	CONTROL_MEM_READ = 1;
	#100
	$finish;
	end

endmodule