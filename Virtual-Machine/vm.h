#include <bits/stdc++.h>
#include "token.h"
using namespace std;

#define outputTemVM "temp.vm"

class VM
{
private:
    string ifname, ofname;
    vector<Token> tokens;
    string token;
    stack<vector<string>> localVars;
    stack<vector<string>> args;
    int inputLabel=0;
    void threeAddToVMCode();
    void vmCodeToAssembly();
    int get3acTypeOfInstruction(vector<Token>&);
    int getVMTypeOfInstruction(vector<Token>&);
    string getVMCode(string);
    string getAssemblyCode(string);
    string getSegmentPos(Token&);
    string memInit();
    string saveRegisters();
    string loadRegisters();

public:
    VM(string, string);
    void convertToAssembly();
};

VM::VM(string inputFileName, string outputFileName)
{
    ifname = inputFileName;
    ofname = outputFileName;
}

void VM::convertToAssembly()
{
    threeAddToVMCode();
    vmCodeToAssembly();
}

void VM::threeAddToVMCode()
{
    ifstream fin;
    ofstream fout;
    fin.open(ifname);
    fout.open(outputTemVM);
    string is, vms;
    while (getline(fin, is))
    {
        vms = getVMCode(is);
        fout << vms;
    }
    fin.close();
    fout.close();
}

string VM::getVMCode(string s)
{
    string segmentPos;
    // Arithmetic Operations And Assignment Statements

    // 1. Tokenize
    stringstream tokenizer(s);
    tokens.clear();
    while (getline(tokenizer, token, ' '))
    {
        Token t(token);
        tokens.push_back(t);
    }

    // 2. Match Format of 3 address code
    // Assignment - 0
    // Arithmetic, Logical - 1
    // Labels, Branching - 2
    // Function/method definition and return - 3
    // Function/methods call - 4
    // Function/methods parameters - 5
    // Variable declaration - 6
    // Input-Output - 7
    // New line - 8
    // Syntax Error - 9

    int typeOfInstruction = get3acTypeOfInstruction(tokens);

    // 3. Convert to appropriate VM Code
    

    string vmcode = "";
    if (typeOfInstruction == 0)
    {
        // Assignment Operation
        segmentPos = getSegmentPos(tokens.at(2));
        vmcode = "push " + segmentPos + "\n";
        segmentPos = getSegmentPos(tokens.at(0));
        vmcode += "pop " + segmentPos + "\n";
    }
    else if (typeOfInstruction == 1)
    {
        // Arithmetic and Logical Operation
        segmentPos = getSegmentPos(tokens.at(2));
        vmcode = "push " + segmentPos + "\n";
        segmentPos = getSegmentPos(tokens.at(4));
        vmcode += "push " + segmentPos + "\n";

        string opType = tokens.at(3).name;
        if (opType.compare("+") == 0)
            vmcode += "add\n";
        else if (opType.compare("-") == 0)
            vmcode += "sub\n";
        else if (opType.compare("==") == 0)
            vmcode += "eq\n";
        else if (opType.compare("<") == 0)
            vmcode += "lt\n";
        else if (opType.compare("&&") == 0)
            vmcode += "and\n";
        else if (opType.compare("||") == 0)
            vmcode += "or\n";

        segmentPos = getSegmentPos(tokens.at(0));
        vmcode += "pop " + segmentPos + "\n";
    }
    else if(typeOfInstruction == 2)
    {
        // Labels and Branches
        
        // Label
        if(tokens.at(1).name.compare(":") == 0)
            vmcode += "label " + tokens.at(0).name;

        // Unconditional jump
        else if(tokens.at(0).name.compare("goto") == 0)
            vmcode += "goto " + tokens.at(1).name;

        // Conditional jump
        else if(tokens.at(2).name.compare("goto") == 0)
        {
            segmentPos = getSegmentPos(tokens.at(1));
            vmcode += "push " + segmentPos + "\n";
            vmcode += "if-goto " + tokens.at(3).name;
        }
            
        vmcode += "\n";
    }
    else if(typeOfInstruction == 3)
    {
        // Function/method definition and return
        if(tokens.at(0).name.compare("function")==0)
        {
            vector<string> v;
            localVars.push(v);
            for(int i=3; i<tokens.size(); i++)
                v.push_back(tokens.at(++i).name);
            args.push(v);

            vmcode += "function " + tokens.at(1).name + " " + tokens.at(2).name + "\n";

        }

        // Return
        else if(tokens.at(0).name.compare("return")==0)
        {

            // Void function
            if(tokens.size()==1)
            {
                vmcode += "push NULL\n";
                vmcode += "return\n";
            }

            // Non-void function
            else if(tokens.size()==2)
            {
                segmentPos = getSegmentPos(tokens.at(1));
                vmcode += "push " + segmentPos + "\n";
                vmcode += "return\n";
            }

            localVars.pop();
            args.pop();
        }
        
    }
    else if(typeOfInstruction == 4)
    {
        // Function/methods call

        //Void call
        if(tokens.at(0).name.compare("call") == 0)
            vmcode += "call " + tokens.at(1).name + " " + tokens.at(2).name + "\n";

        //Non-void call
        else if(tokens.at(2).name.compare("call") == 0)
        {
            vmcode += "call " + tokens.at(3).name + " " + tokens.at(4).name + "\n";
            segmentPos = getSegmentPos(tokens.at(0));
            vmcode += "pop " + segmentPos + "\n";
        }
    }
    else if(typeOfInstruction == 5)
    {
        // Function/methods parameters
        if(tokens.at(0).name.compare("pushParam") == 0)
        {
            segmentPos = getSegmentPos(tokens.at(1));
            vmcode += "push " + segmentPos + "\n";
        }
    }
    else if(typeOfInstruction == 6)
    {
        // Variable Declaration
        localVars.top().push_back(tokens.at(1).name);
    }
    else if(typeOfInstruction == 7)
    {
        // Input-Output
        segmentPos = getSegmentPos(tokens.at(1));
        
        // print
        if(tokens.at(0).name.compare("print") == 0)
            vmcode += "print " + segmentPos + "\n";
        
        // read
        else if(tokens.at(0).name.compare("read") == 0)
            vmcode += "read " + segmentPos + "\n";
    }
    else if(typeOfInstruction == 8)
    {
        // New line
        vmcode += "\n";
    }
    else
    {
        //Syntax Error
        cout << "Three Address Code Syntax ERROR : -" << endl;
        cout << s << endl;
    }
    return vmcode;
}

int VM::get3acTypeOfInstruction(vector<Token> &v)
{
    //New movne
    if(v.empty())
        return 8;

    //Assignment - 0
    if (v.size() == 3 && v.at(1).name.compare("=")==0)
        return 0;

    //Arithmetic and Logical - 1
    else if (v.size() == 5 && v.at(1).name.compare("=")==0 && v.at(2).name.compare("call")!=0)
        return 1;

    // Function/method definition and return - 3
    else if(v.at(0).name.compare("function") == 0 || v.at(0).name.compare("return") == 0)
        return 3;

    // Lables and Branches - 2
    else if(v.size()>1 && (v.at(1).name.compare(":") == 0 || tokens.at(0).name.compare("goto") == 0 || tokens.at(0).name.compare("If") == 0))
        return 2;

    // Function/method calls - 4
    else if(v.size()>1 && (v.at(0).name.compare("call") == 0 || v.at(1).name.compare("=") == 0))
        return 4;

    // Functional Parameters - 5
    else if(v.at(0).name.compare("pushParam") == 0)
        return 5;

    // Variable declaration - 6
    else if(v.at(0).name.compare("int") == 0 || v.at(0).name.compare("char") == 0)
        return 6;

    // Input-Output
    else if(v.at(0).name.compare("print") == 0 || v.at(0).name.compare("read") == 0)
        return 7;

    // Syntax Error
    else
        return 9;
}

string VM::getSegmentPos(Token &t)
{
    string s = "";
    
    if(t.name.at(0)=='_')
    {
        s += "temp " + t.name.substr(2);
        return s;
    }

    if(t.name.at(0)=='\'' || isdigit(t.name.at(0)))
    {
        s += "constant " + t.name;
        return s;
    }

    vector <string> v = args.top();
    auto it = find(v.begin(), v.end(), t.name);
    if(it!=v.end())
    {
        s += "argument " + to_string(it-v.begin());
        return s;
    }

    v = localVars.top();
    it = find(v.begin(), v.end(), t.name);
    if(it!=v.end())
    {
        s += "local " + to_string(it-v.begin());
        return s;
    }

    cout<<"Not found!!! -> "<<t.name<<" -> addng to local"<<endl;

    s += "local " + to_string(localVars.top().size());
    localVars.top().push_back(t.name);
    return s;
}

void VM::vmCodeToAssembly()
{
    ifstream fin;
    ofstream fout;
    fin.open(outputTemVM);
    fout.open(ofname);
    string is, arm;

    arm = memInit();
    fout << arm;

    while (getline(fin, is))
    {
        arm = getAssemblyCode(is);
        fout << arm;
    }

    fin.close();
    fout.close();
}

// 0-10 - gen
// 11-pc,
//12,13,14,15  - special

string VM::memInit()
{
    string armcode = "";

    // SP,LCL,ARG init
    armcode += "mov r0, 0\n";
    armcode += "mov r14, 256\n";
    armcode += "str r14, [r0, 0]\n";
    armcode += "str r14, [r0, 1]\n";
    armcode += "str r14, [r0, 2]\n";
    
    // Reg Stack pointer(RSP) init
    armcode += "mov r14, 16389\n";
    armcode += "str r14, [r0, 16388]\n";
    return armcode;
}

string VM::getAssemblyCode(string s)
{
    // 1. Tokenize
    stringstream tokenizer(s);
    tokens.clear();
    while (getline(tokenizer, token, ' '))
    {
        Token t(token);
        tokens.push_back(t);
    }


    // 2. Match Format of VM code
    // push - 0
    // pop - 1
    // Function/method declaration - 2
    // Function/methods call - 3
    // return - 4
    // Arihmetic, Logical - 5
    // if-goto - 6
    // goto - 7
    // Label - 8
    // print - 9
    // read - 10
    // New line - 11
    // Syntax Error - 12

    int typeOfInstruction = getVMTypeOfInstruction(tokens);
    // cout<<s<<" -> "<<typeOfInstruction<<endl;
    // 3. Convert to appropriate assembly Code
    string armcode = "";
    if (typeOfInstruction == 0)
    {
        // push

        // temp
        if(tokens.at(1).name.compare("temp")==0)
        {
            // _ti <==> r(i%9+1)
            int regNo = (stoi(tokens.at(2).name))%9+ 1;

            // ri -> MEM[SP]
            armcode += "\tldr r14,[r0, 0]\n";
            armcode += "\tstr r" + to_string(regNo) + ", [r14, 0]\n";

            // SP++ 
            armcode += "\tadd r14,r14,1\n";
            armcode += "\tstr r14,[r0, 0]\n";
        }

        else
        {
            // local
            if(tokens.at(1).name.compare("local")==0)
            {
                // r14 <- MEM[LCL + i]
                armcode += "\tldr r13,[r0, 1]\n";
                armcode += "\tldr r14,[r13," + tokens.at(2).name + "]\n";
            }

            // argument
            else if(tokens.at(1).name.compare("argument")==0)
            {
                // r14 <- MEM[ARG + i]
                armcode += "\tldr r13,[r0, 2]\n";
                armcode += "\tldr r14,[r13," + tokens.at(2).name + "]\n";
            }

            // constant
            else if(tokens.at(1).name.compare("constant")==0)
                // r14 <- constant
                armcode += "\tmov r14," + tokens.at(2).name + "\n";

            // r14 -> MEM[SP]
            armcode += "\tldr r13,[r0, 0]\n";
            armcode += "\tstr r14,[r13, 0]\n";

            // SP++
            armcode += "\tadd r13,r13,1\n";
            armcode += "\tstr r13,[r0, 0]\n";

        }
    }
    else if (typeOfInstruction == 1)
    {
        // pop

        // SP--
        armcode += "\tldr r14,[r0, 0]\n";
        armcode += "\tadd r14,r14,-1\n";
        armcode += "\tstr r14,[r0, 0]\n";

        // temp
        if(tokens.at(1).name.compare("temp")==0)
        {
            // _ti <==> r(i%9+1)
            int regNo = (stoi(tokens.at(2).name))%9+1;

            // ri <- MEM[SP-1]
            armcode += "\tldr r" + to_string(regNo) + ",[r14, 0]\n";
        }

        else
        {
            
            // r13 <- MEM[SP-1]
            armcode += "\tldr r13,[r14,0]\n";

            // local
            if(tokens.at(1).name.compare("local")==0)
            {
                // r13 -> MEM[LCL + i]
                armcode += "\tldr r14, [r0, 1]\n";
                armcode += "\tstr r13,[r14," + tokens.at(2).name + "]\n";
            }

            // argument
            else if(tokens.at(1).name.compare("argument")==0)
            {
                // r14 -> MEM[ARG + i]
                armcode += "\tldr r14,[r0, 2])\n";
                armcode += "\tstr r13,[r14," + tokens.at(2).name + "]\n";
            }

        }
    }
    else if(typeOfInstruction == 2)
    {
        // Function/method declaration - 2
        armcode += tokens.at(1).name + ":\n";

        // push 0 in stack k times
        int k = stoi(tokens.at(2).name);
        if(k>0)
        { 
            armcode += "\tldr r14,[r0, 0]\n";
            while(k--)
            {
                armcode += "\tstr r0,[r14, 0]\n";
                // armcode += "\tmov r14,1\n";
                armcode += "\tadd r14,r14,1\n";
            }
            armcode += "\tstr r14,[r0, 0]\n";
        }
    }
    else if(typeOfInstruction == 3)
    {
        // Function/methods call - 3
        
        // push return-addr // save return-addr
        armcode += "\tldr r14,[r0, 0]\n";
        armcode += "\tstr r15,[r14,0]\n";        
        armcode += "\tadd r14,r14,1\n";

        // push LCL // save LCL
        armcode += "\tldr r13,[r0, 1]\n";
        armcode += "\tstr r13,[r14,0]\n";
        // armcode += "\tmov r14,1\n";
        armcode += "\tadd r14,r14,1\n";

        // push ARG // save ARG
        armcode += "\tldr r13,[r0, 2]\n";
        armcode += "\tstr r13,[r14,0]\n";
        armcode += "\tadd r14,r14,1\n";

        // ARG = SP - n - 3 // change ARG
        int n = stoi(tokens.at(2).name)+3;
        armcode += "\tadd r13,r14,-" + to_string(n) + "\n";
        armcode += "\tstr r13,[r0, 2]\n";

        // Update SP
        armcode += "\tstr r14,[r0, 0]\n";

        // LCL = SP // change LCL
        armcode += "\tstr r14,[r0, 1]\n";

        // save r0-r25 values
        armcode += saveRegisters();

        // goto fun // call function
        armcode += "\tb " + tokens.at(1).name + "\n\n";

        // return-addr: // a label
        // armcode += tokens.at(1).name + "_return:\n";
        
    }
    else if(typeOfInstruction == 4)
    {
        // return - 4

        // FRAME = LCL //
        armcode += "\tldr r14,[r0, 1]\n";

        // RET = *(FRAME - 3) // keep return-addr
        armcode += "\tadd r12,r15,r0\n";
        armcode += "\tadd r15,r14,-3\n";
        armcode += "\tldr r15,[r15,0]\n";

        // *ARG = pop() // return value
        armcode += "\tldr r13,[r0, 0]\n";
        armcode += "\tadd r13,r13,-1\n";
        armcode += "\tldr r13,[r13,0]\n";
        armcode += "\tldr r14,[r0, 2]\n";
        armcode += "\tstr r13,[r14,0]\n";

        // SP = ARG + 1 // restore SP of called
        armcode += "\tadd r14,r14,1\n";
        armcode += "\tstr r14,[r0, 0]\n";

        // ARG = *(FRAME - 1) // restore ARG
        armcode += "\tldr r14,[r0, 1]\n";
        armcode += "\tadd r14,r14,-1\n";
        armcode += "\tldr r13,[r14,0]\n";
        armcode += "\tstr r13,[r0, 2]\n";

        // LCL = *(FRAME - 2) // restore LCL
        armcode += "\tadd r14,r14,-1\n";
        armcode += "\tldr r13,[r14,0]\n";
        armcode += "\tstr r13,[r0, 1]\n";

        // Load old register values
        armcode += loadRegisters();

        // goto RET // back to return-addr
        armcode += "\tbr r12\n";
    }
    else if(typeOfInstruction == 5)
    {
        // Arihmetic, Logical - 5

        // r14 <- SP-1
        armcode += "\tldr r14,[r0, 0]\n";
        armcode += "\tadd r14,r14,-1\n";

        // r13 <- MEM[SP-1]
        armcode += "\tldr r13,r14(0)\n";

        // r14 <- SP-2
        armcode += "\tadd r14,r14,-1\n";

        // r12 <- MEM[SP-2]
        armcode += "\tldr r12,r14(0)\n";
        
        // perform operation
        if(tokens.at(0).name.compare("add")==0)
            armcode += "\tadd r12,r12,r13\n";

        else if(tokens.at(0).name.compare("sub")==0)
            armcode += "\tsub r12,r12,r13\n";

        else if(tokens.at(0).name.compare("eq")==0)
            armcode += "\tteq r12,r12,r13\n";

        // else if(tokens.at(0).name.compare("lt")==0)
        //     armcode += "\tlt r12,r12,r13\n";

        else if(tokens.at(0).name.compare("and")==0)
            armcode += "\tand r12,r12,r13\n";

        else if(tokens.at(0).name.compare("or")==0)
            armcode += "\torr r12,r12,r13\n";

        // r12 -> MEM[SP-2]
        armcode += "\tstr r12,[r14,0]\n";

        // r14 <- SP-1
        armcode += "\tadd r14,r14,1\n";
        armcode += "\tstr r14,[r0, 0]\n";
    
    }
    else if(typeOfInstruction == 6)
    {
        // if-goto - 6

        // r13 <- MEM[SP]
        armcode += "\tldr r14,[r0, 0]\n";
        armcode += "\tadd r14,r14,-1\n";
        armcode += "\tldr r13,[r14,0]\n";

        // branch if r13==$one
        armcode += "\tbeq r13,1," + tokens.at(1).name + "\n";
    
    }
    else if(typeOfInstruction == 7)
    {
        // goto - 7
        armcode += "\tb " + tokens.at(1).name + "\n";

    }
    else if(typeOfInstruction == 8)
    {
        // Label
        if(tokens.at(1).name.compare("end")==0)
        {
            armcode = "end:\n";
            armcode += "\tb end\n";
        }
        else
            armcode += tokens.at(1).name + ":\n";
    }
    else if(typeOfInstruction == 9)
    {
        // print
        
        // temp
        if(tokens.at(1).name.compare("temp")==0)
        {
            // _ti <==> r(i%9+1)
            int regNo = (stoi(tokens.at(2).name))%9+1;

            // ri -> MEM[16387]
            armcode += "\tstr r" + to_string(regNo) + ",[r0,16387]\n";
        }

        else
        {
            
            // local
            if(tokens.at(1).name.compare("local")==0)
            {
                // r14 <- MEM[LCL + i]
                armcode += "\tldr r14,[r0, 1]\n";
                armcode += "\tldr r14,[r14," + tokens.at(2).name + "]\n";
            }

            // argument
            else if(tokens.at(1).name.compare("argument")==0)
            {
                // r14 <- MEM[ARG + i]
                armcode += "\tldr r14,[r0, 2]\n";
                armcode += "\tldr r14,[r14," + tokens.at(2).name + "]\n";
            }

            // r14 -> MEM[16387]
            armcode += "\tstr r14,[r0,16387]\n";
        }
    }
    else if(typeOfInstruction == 10)
    {
        // read
        
        // input_i:
        string label = "input_" + to_string(inputLabel++);
        armcode += label + ":\n";

        // r14 <- MEM[16386]
        armcode += "\tldr r14,[r0,16386]\n";
        armcode += "\tbeq r14,r0," + label + "\n";

        // temp
        if(tokens.at(1).name.compare("temp")==0)
        {
            // _ti <==> r(i%9+1)
            int regNo = (stoi(tokens.at(2).name))%9+1;

            // ri <- MEM[16386]
            // armcode += "\tadd r" + to_string(regNo) + ",r14,$zero\n";
            armcode += "\tldr r" + to_string(regNo) + ",[r0, 16386]";
            // armcode += "\tstr r" + to_string(regNo) + ",r14,$zero\n";
        }

        else
        {
            
            // local
            if(tokens.at(1).name.compare("local")==0)
            {
                // r13 <- LCL
                armcode += "\tldr r13,[r0, 1]\n";
            }

            // argument
            else if(tokens.at(1).name.compare("argument")==0)
            {
                // r13 <- ARG
                armcode += "\tldr r13,[r0, 2]\n";
            }

            // r14 -> MEM[16387]
            armcode += "\tstr r14,[r13," + tokens.at(2).name + "]\n";
        }
    }
    else if(typeOfInstruction == 11)
    {
        // New movne
        armcode += "\n";
    }
    else
    {
        //Syntax Error
        cout << "VM Code Syntax ERROR : -" << endl;
        cout << s << endl;
    }
    return armcode;
}

int VM::getVMTypeOfInstruction(vector<Token> &v)
{
    //New movne
    if(v.empty())
        return 11;

    else if(v.at(0).name.compare("push")==0)
        return 0;
    else if(v.at(0).name.compare("pop")==0)
        return 1;
    else if(v.at(0).name.compare("function")==0)
        return 2;
    else if(v.at(0).name.compare("call")==0)
        return 3;
    else if(v.at(0).name.compare("return")==0)
        return 4;

    // Arithmetic, Logical
    else if(v.size()==1)
        return 5;

    else if(v.at(0).name.compare("if-goto")==0)
        return 6;
    else if(v.at(0).name.compare("goto")==0)
        return 7;
    else if(v.at(0).name.compare("label")==0)
        return 8;
    else if(v.at(0).name.compare("print")==0)
        return 9;
    else if(v.at(0).name.compare("read")==0)
        return 10;
    
    // Syntax Error
    else
        return 12;
}

string VM::saveRegisters()
{
    string armcode = "";
    armcode += "\tldr r14,[r0,16388]\n";
    for(int i=0; i<=10; i++)
    {
        armcode += "\tstr r" + to_string(i) + ",[r14,0]\n";
        armcode += "\tadd r14,r14,1\n";
    }
    armcode += "\tstr r14,[r0,16388]\n";
    return armcode;
}

string VM::loadRegisters()
{
    string armcode = "";
    armcode += "\tldr r14,[r0,16388]\n";
    for(int i=10; i>=0; i--)
    {
        armcode += "\tadd r14,r14,-1\n";
        armcode += "\tldr r" + to_string(i) + ",[r14,0]\n";
    }
    armcode += "\tstr r14,[r0,16388]\n";
    return armcode;
}