//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
	// Nothing else to do here.
}

/*
 Assembler::PassI()

 NAME
    PassI - establishes the location of the labels
 SYNOPSIS
    void Assembler::PassI()
 DESCRIPTION
    Reads from file and parses each line. Creates
    symbol table using labels and keeping track of
    the current address.
 RETURNS
    void
 */
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (;;) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		m_inst.LocationNextInstruction(loc);
	}
}

// Need to declare static variable in order to use it
vector<string> Errors::m_ErrorMsgs;

/*
 Assembler::PassII()

 NAME
    PassII - generate the translation
 SYNOPSIS
    void Assembler::PassII()
 DESCRIPTION
    Reads from file and parses each line. prints out
    the translation using the current address and gets
    the instruction code with the symbol table. prints
    appropriate errors after the offending line.
 RETURNS
    void
 */
void Assembler::PassII()
{
    //return to beginning of source file
    m_facc.rewind();

    int loc = 0; // Tracks the location of the instructions to be generated.

    cout << left << "Translation of Program:\n\n" << setw(10) << "Location" << setw(10) << "Contents" << "Original Statement\n";

    // Flag is set when end statement is reached
    bool endflag = false;
    // clear errors from PassI
    Errors::InitErrorReporting();

    // Successively process each line of source code.
	for (;;)
    {
		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff))
        {

            if (endflag != true) {
                cout << "\t" << "Error: Missing End Statement\n";
            }
			break;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

        string instructCode;
        // Check if blank line or comment
        if (st == Instruction::ST_Comment || st == Instruction::ST_End)
        {
            if (st == Instruction::ST_End) {
                endflag = true;
            }
            //print line out
            cout << setw(20) << "" << buff << "\n";
        }
        else // Must be a machine or assembly instruction
        {
            if (!endflag)
            {
                if (loc < 10000) // within memory
                {
                    instructCode = m_inst.GetInstruction(m_symtab);
                    //print out translation
                    cout << setw(10) << loc << setw(10) << instructCode << buff << "\n";
                    Errors::DisplayErrors();

                    // Set up emulator
                    string::size_type x = instructCode.find('?');
                    if ( x == string::npos && instructCode != "")
                    {
                        m_emul.InsertMemory( loc, stoi(instructCode) );
                    }
                    else if (instructCode != "")
                    {
                        m_emul.InsertMemory( loc, -1 );
                    }
                }
                else
                {
                    cout << setw(10) << "????" << setw(10) << instructCode << buff << "\n";
                }
            }
            else
            {
                cout << setw(20) << "" << buff << "\n\tError: Statement comes after end statement\n";
            }
        }
        if (!endflag) // if past end statement, address is irrelevent
        {
            // Compute the location of the next instruction.
            if ( !m_inst.LocationNextInstruction(loc) )
            {
                Errors::DisplayErrors();
            }
        }
    }
    cout << "\n\n";
}

/*
 Assembler::RunProgramInEmulator()

 NAME
    RunProgramInEmulator - runs emulator
 SYNOPSIS
    void Assembler::RunProgramInEumlator()
 DESCRIPTION
    Call to run the emulator. Print if error is
    encountered.
 RETURNS
    void
 */
void Assembler::RunProgramInEmulator()
{
    cout << "Results from emulating program:\n\n";
    if ( !m_emul.RunProgram() )
    {
        cout << "Program exited due to an error\n";
    }
}
