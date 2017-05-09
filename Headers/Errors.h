//
// Class to manage error reporting.
// 
//
#ifndef _ERRORS_H
#define _ERRORS_H


#include "stdafx.h"

using namespace std;

class Errors {

public:

	// Initializes error reports.
	static void InitErrorReporting();

	// Records an error message.
	static void RecordError(string a_emsg);

	// Displays the collected error message.
	static void DisplayErrors();

private:

	static vector<string> m_ErrorMsgs;
};

#endif
