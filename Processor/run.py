import os

allFiles=["Adder.v","ALU_Control.v","ALU.v","And.v","BranchPC.v","Core.v","CPU.v","JumpPC.v","Mux.v","RAM.v","RegisterFile.v","ROM.v","SignExtend.v"]


command1="iverilog -o design "
for i in allFiles:
    command1+=i
    command1+=" "
os.system(command1)

command2="vvp design"
os.system(command2)