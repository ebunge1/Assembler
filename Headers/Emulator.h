//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H
#define _EMULATOR_H

#include "stdafx.h"

class Emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	Emulator() {

		memset(m_memory, 0, MEMSZ * sizeof(int));
	}
	// Records instructions and data into VC3600 memory.
	void InsertMemory(int a_location, int a_contents);

	// Runs the VC3600 program recorded in memory.
	bool RunProgram();

private:


	int m_memory[MEMSZ];		// The memory of the VC3600.
	int m_acc;				//the accumulator
};

#endif
