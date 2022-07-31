module RegisterFile_tb;
	reg [4:0] r1;reg[4:0] r2;reg[4:0] w;
	reg [31:0] wD;wire[31:0] rD1;wire [31:0] rD2;
	reg Control;

	RegisterFile register(r1, r2, w, wD, Control, rD1, rD2);

	initial begin
		r1 = 5'h00;
		r2 = 5'h01;
		w = 5'h00;
		wD = 32'hffffffff;
		Control = 1;
		#100
		Control = 0;
		wD = 32'hf0f0f0f0;
		#100
		Control = 1;
		w = 5'h01;
		r1 = 5'h01;
		#100
		r2 = 5'h00;
		r1 = 5'h01;
		#100
		$finish;
	end
endmodule 