//
//  Implementation of Emulator class
//

#include "Emulator.h"

/*
 Emulator::InsertMemory()
 
 NAME
    InsertMemory - inserts content into given address
 SYNOPSIS
    void Emulator::InsertMemory(int a_location, int a_contents)
        a_location --> the address in the memory array
        a_contents --> the instruction or data to insert
 DESCRIPTION
    Inserts the given integer into the given address of the
    array if it is within memory
 RETURNS
    void
 */
void Emulator::InsertMemory(int a_location, int a_contents)
{
    if (a_location < 10000)
    {
         m_memory[a_location] = a_contents;
    }
}

/*
 Emulator::RunProgram()
 
 NAME
    RunProgram - runs the emulator
 SYNOPSIS
    bool Emulator::RunProgram()
 DESCRIPTION 
    Iterates through memory and parses each instruction.
    Then reads and executes the instruction.
 RETURNS
    bool - whether the program executed successfully
 */
bool Emulator::RunProgram()
{
    for (int i = 100; i <10000; i++)
    {
        int currentInstruct;
        int command;
        int address;
        
        // stop at halt statement
        currentInstruct = m_memory[i];
        if (currentInstruct == 130000) {
            break;
        }
        
        // -1 indicates an error in translation
        if (currentInstruct == -1) {
            return false;
        }
        
        // separate command and address
        address = currentInstruct % 10000;
        command = (currentInstruct - address) / 10000;
        switch (command)
        {
            case 1: // add
                m_acc += m_memory[address];
                break;
            case 2: // sub
                m_acc -= m_memory[address];
                break;
            case 3: // mult
                m_acc *= m_memory[address];
                break;
            case 4: // div
                m_acc /= m_memory[address];
                break;
            case 5: // load
                m_acc = m_memory[address];
                break;
            case 6: // store
                m_memory[address] = m_acc;
                break;
            case 7: // read
                int input;
                cout << "? ";
                cin >> input;
                m_memory[address] = input % 1000000;
                break;
            case 8: // write
                cout << m_memory[address] << "\n";
                break;
            
            // subtract 1 from address to compensate for i++
            case 9: // b
                i = address-1;
            case 10: // bm
                if (m_acc < 0)
                {
                    i = address-1;
                }
                break;
            case 11: // bz
                if (m_acc == 0)
                {
                    i = address-1;
                }
                break;
            case 12: // bp
                if (m_acc > 0)
                {
                    i = address-1;
                }
                break;
        }
    }
    return true;
}