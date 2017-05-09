//
//  Implementation of Istruction class
//

#include "Instruction.h"
#include "Errors.h"


/*
 Instruction::ParseInstruction(string a_buff)
 
 NAME
    ParseInstruction - initializes member variables from string
 SYNOPSIS
    Instruction::InstructionType Instruction::ParseInstruction(string a_buff)
        a_buff --> string read from file containing instruction in assembly language
 DESCRIPTION
    Parses the given string and initializes member variables to values.
    Proper errors are recorded
 RETURNS
    Instruction::InstructionType - the type of instruction being made
 */
Instruction::InstructionType Instruction::ParseInstruction(string a_buff)
{
    m_instruction = a_buff;
    // Find comment
    string::size_type x = a_buff.find(';');
    // Erase comments
    if ( x != string::npos )
    {
        a_buff.erase( a_buff.begin()+x, a_buff.end() );
    }
    
    // read in each part of string
    istringstream ins(a_buff);
    string a, b, c, d;
    ins >> a >> b >> c >> d;
    // Test if empty line
    if (a == "")
    {
        // Clear Opcode to not interfer with LocationNextInstruction()
        m_OpCode = "";
        m_type = ST_Comment;
        return m_type;
    }
    if (a == "end")
    {
        m_OpCode = "end";
        m_type = ST_End;
        return m_type;
    }
    // Test for label
    if ( isspace( a_buff[0] ) ) // no label
    {
        m_Label = "";
        transform(a.begin(), a.end(), a.begin(), ::tolower);
        m_OpCode = a;
        m_Operand = b;
        // test for extra operand
        if (c != "")
        {
            Errors::RecordError( "Error: Cannot accept more than one operand" );
            m_Operand = "";
            m_OperandValue = 0;
        }
    }
    else // there is a label
    {
        m_Label = a;
        transform(b.begin(), b.end(), b.begin(), ::tolower);
        m_OpCode = b;
        m_Operand = c;
        // test for extra operand
        if (d != "")
        {
            Errors::RecordError( "Error: Cannot accept more than one operand" );
            m_Operand = "";
            m_OperandValue = 0;
        }
        if (m_Label.size() > 10)
        {
            Errors::RecordError( "Error: Invalid label length" );
        }
        if ( !isalpha(m_Label[0]) )
        {
            Errors::RecordError( "Error: Label must begin with a letter" );
        }

    }
    if (m_Operand.size() > 10)
    {
        Errors::RecordError( "Error: Invalid operator length" );
    }
    
    if (m_Operand == "" && m_OpCode != "halt")
    {
        Errors::RecordError( "Error: Missing operand" );
        m_OperandValue = 0;
    }
    
    if (m_OpCode == "dc" || m_OpCode == "ds" || m_OpCode == "org")
    {
        m_type = ST_AssemblerInstr;
    }
    else
    {
        m_type = ST_MachineLanguage;
    }
    VerifyOperandType();
    return m_type;
}

/*
 Instruction::LocationNextInstruction(int &a_loc)
 
 NAME
    LocationNextInstruction - calculates address of next instruction
 SYNOPSIS
    bool Instruction::LocationNextInstruction(int &a_loc)
        a_loc --> the current address and variable to return new address
 DESCRIPTION
    Decide which kind of statement is being made, increment accordingly.
    record error if memory is out of range.
 RETURNS
    bool - whether next address is within memory range
 */
bool Instruction::LocationNextInstruction(int &a_loc)
{
    if (m_OpCode == "end" || m_OpCode == "") {
        return true;
    }
    else if (m_OpCode == "ds" || m_OpCode == "org")
    {
        a_loc += m_OperandValue;
    }
    else if (m_OpCode != "")
    {
        a_loc += 1;
    }
    if (a_loc >= 10000) {
        Errors::RecordError( "Error: Insufficient memory" );
        return false;
    }
    return true;
}

/*
 Instruction::GetInstruction(SymbolTable a_symtab)
 
 NAME
    GetInstruction - gets the translated instruction
 SYNOPSIS
    string Instruction::GetInstruction(SymbolTable a_symtab)
        a_symtab --> the symbol table made from PassI
 DESCRIPTION
    Coverts the operation code word to an number. gets the
    address of the operand from the symbol table. combines these
    into six digit machine instruction. records proper errors.
 RETURNS
 string - the translated instruction
 */
string Instruction::GetInstruction(SymbolTable a_symtab)
{
    ostringstream oss;
    if (m_type == ST_MachineLanguage)
    {
        if ( !ConvertOpCode(m_OpCode) )
        {
            Errors::RecordError( "Error: Invalid Operation Code" );
        }
        int loc;
        bool isFound;
        string address;
        if (m_OpCode == "halt")
        {
            address = "";
        }
        else if (m_IsNumericOperand)
        {
            Errors::RecordError( "Error: Machine instructions require symbolic operands" );
            address = "????";
        }
        else
        {
            isFound = a_symtab.LookupSymbol(m_Operand, loc);
            if (!isFound)
            {
                Errors::RecordError( "Error: Label not found" );
                address = "????";
            }
            else
            {
                if (loc < 0) // test if found address is multiplyDefinedSymbol constant
                {
                    Errors::RecordError( "Error: Label multiply declared" );
                    address = "????";
                }
                else if (loc >= 10000)
                {
                    address = "????";
                }
                else
                {
                    address = to_string(loc);
                }
            }
        }
        oss << setw(2) << setfill('0') << m_NumOpCode << setw(4) << setfill('0') << address;
        return oss.str();
    }
    else    // ST_AssemblerInstr
    {
        if (!m_IsNumericOperand)
        {
            Errors::RecordError( "Error: Assembler instructions require numerical operands" );
        }
        if (m_OpCode == "dc")
        {
            if (!m_IsNumericOperand)
            {
                return "??????";
            }
            else
            {
                if (m_OperandValue > 999999) {
                    Errors::RecordError( "Error: Constant is too large" );
                    m_OperandValue %= 1000000; // chop off higher part
                }
                oss << setw(6) << setfill('0') << m_OperandValue;
                return oss.str();
            }
        }
        else
        {
            return "";
        }
    }
}

/*
 Instruction::ConvertOpCode(string &a_OpCode)
 
 NAME
    ConvertOpCode - converts op code word to number
 SYNOPSIS
    bool Instruction::LocationNextInstruction(string &a_OpCode)
        a_Opcode --> the op code word
 DESCRIPTION
    converts the op code from a word to a number to be used
    in the translated instruction
 RETURNS
    bool - whether the op code is valid
 */
bool Instruction::ConvertOpCode(string &a_OpCode)
{
    if (m_OpCode == "add")
    {
        m_NumOpCode = 1;
    }
    else if (m_OpCode == "sub")
    {
        m_NumOpCode = 2;
    }
    else if (m_OpCode == "mult")
    {
        m_NumOpCode = 3;
    }
    else if (m_OpCode == "div")
    {
        m_NumOpCode = 4;
    }
    else if (m_OpCode == "load")
    {
        m_NumOpCode = 5;
    }
    else if (m_OpCode == "store")
    {
        m_NumOpCode = 6;
    }
    else if (m_OpCode == "read")
    {
        m_NumOpCode = 7;
    }
    else if (m_OpCode == "write")
    {
        m_NumOpCode = 8;
    }
    else if (m_OpCode == "b")
    {
        m_NumOpCode = 9;
    }
    else if (m_OpCode == "bm")
    {
        m_NumOpCode = 10;
    }
    else if (m_OpCode == "bz")
    {
        m_NumOpCode = 11;
    }
    else if (m_OpCode == "bp")
    {
        m_NumOpCode = 12;
    }
    else if (m_OpCode == "halt")
    {
        m_NumOpCode = 13;
    }
    else if (m_OpCode != "dc" && m_OpCode != "ds" && m_OpCode != "org" && m_OpCode != "end")
    {
        return false;
    }
    return true;
}

/*
 Instruction::VerifyOperandType()
 
 NAME
    VerifyOperandType - verifies the operand is correct for the op code
 SYNOPSIS
    void Instruction::VerifyOperandType()
 DESCRIPTION
    checks if operand either starts with a letter or is
    composed of only digits. sets the m_IsNumericOperand
    appropriately
 RETURNS
    void
 */
void Instruction::VerifyOperandType()
{
    if (m_Operand == "")
    {
        return;
    }
    else if ( isalpha(m_Operand[0]) )
    {
        m_IsNumericOperand = false;
    }
    else
    {
        for (string::iterator it = m_Operand.begin(); it != m_Operand.end(); it++)
        {
            if ( !isdigit(*it) )
            {
                Errors::RecordError( "Error: Symbolic operand must begin with a letter" );
                m_IsNumericOperand = false;
                return;
            }
        }
        m_IsNumericOperand = true;
        m_OperandValue = stoi(m_Operand);
    }
}