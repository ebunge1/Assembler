//
//      Implementation of Error class
//

#include "Errors.h"
#include "stdafx.h"

/*
 Errors::RecordError(string a_emsg)
 
 NAME
    RecordError - records error
 SYNOPSIS
    void Errors::RecordError(string a_emsg)
        a_emsg --> the error message to record
 DESCRIPTION
    pushes error message into the vector
 RETURNS
    void
 */
 void Errors::RecordError(string a_emsg)
{
    m_ErrorMsgs.push_back(a_emsg);
}

/*
 Errors::DisplayErrors()
 
 NAME
    DisplayErrors() - prints errors
 SYNOPSIS
    void Errors::DisplayErrors()
 DESCRIPTION
    prints errors on separate lines in the order they
    were recorded. clears the list when done
 RETURNS
    void
 */
void Errors::DisplayErrors()
{
    for (int i = 0; i < m_ErrorMsgs.size(); i++)
    {
        cout << "\t" << m_ErrorMsgs[i] << "\n";
    }
    m_ErrorMsgs.clear();
}

/*
 Errors::InitErrorReporting()
 
 NAME
    InitErrorReporting - clears the vector to start fresh
 SYNOPSIS
    void Errors::InitErrorReporting()
 DESCRIPTION
    clears the vector.
 RETURNS
    void
 */
void Errors::InitErrorReporting()
{
    m_ErrorMsgs.clear();
}