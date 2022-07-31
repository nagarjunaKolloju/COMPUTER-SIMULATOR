module ALU_tb;
	reg [31:0] in1;
	reg [31:0] in2;
	reg [5:0] Con;
	wire [31:0] out;
	wire zero;
	ALU alu(in1, in2, Con, out, zero);

	initial begin
		$monitor("%d,%d,%d,%d,%d",in1, in2, Con, out, zero);
		in1 = 32'b00000101;
		in2 = 32'b00000011;
		Con = 6'b100000; //ADD
		#100;
		Con = 6'b100001; //ADDU
		#100;
		Con = 6'b100100; //AND
		#100;
		Con = 6'b100101; //OR
		#100;
		Con = 6'b100110; //XOR
		#100;
		Con = 6'b100010; //SUB
		#100;
		Con = 6'b100011; //SUBU
		#100;
		Con = 6'b000000; //SLL
		#100;
		Con = 6'b000100; //SLV 
		#100;
		Con = 6'b000011; //SRA
		#100;
		Con = 6'b000010; //SRL
		#100;
		Con = 6'b000110; //SRLV
		#100;
		Con = 6'b101010; //SLT
		#100;
		Con = 6'b101011; //SLTU
		#200
		Con = 6'b000000;
	end
endmodule 