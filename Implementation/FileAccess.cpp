//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
 FileAccess::FileAccess()
 
 NAME
    FileAccess - constructor, opens file
 SYNOPSIS
    FileAccess::FileAccess()
 DESCRIPTION
    Gets the file name from the arguement line.
    Opens the file for reading
 RETURNS
    none
 */
FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}
/*
 FileAccess::~FileAccess()
 
 NAME
    ~FileAccess - destructor, closes file
 SYNOPSIS
    FileAccess::~FileAccess()
 DESCRIPTION
    closes file
 RETURNS
    none
 */
FileAccess::~FileAccess()
{
	m_sfile.close();
}

/*
 FileAccess::GetNextLine()
 
 NAME
    GetNextLine- reads line from file
 SYNOPSIS
    FileAccess::GetNextLine(string &a_buff)
        a_buff --> variable to read line into
 DESCRIPTION
    reads line into the buffer. returns
    status of the read.
 RETURNS
    bool - whether the read was successful
 */
bool FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false;

	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}

/*
 FileAccess::rewind()
 
 NAME
    rewind - returns to begining of file
 SYNOPSIS
    FileAccess::rewind()
 DESCRIPTION
    clears eof flag and returns to begining of file
 RETURNS
    void
 */
void FileAccess::rewind()
{
	// Clean the end of file flag and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}
