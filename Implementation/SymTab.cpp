//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

void AddSymbol( char *a_symbol, int a_loc );

DESCRIPTION

This function will place the symbol "a_symbol" and its location "a_loc"
in the symbol table.
*/
void SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {

		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

/*
 SymbolTable::DisplaySymbolTable()
 
 NAME
    DisplaySymbolTable - prints out symbol table
 SYNOPSIS
    void SymbolTable::DisplaySymbolTable()
 DESCRIPTION
    Prints symbol table from data in the symbol table
    map. Prints out errors for duplicates and insufficient
    memory
 RETURNS
    void
 */
void SymbolTable::DisplaySymbolTable()
{
    int symNum = 0;
    cout << left << "\nSymbol Table:\n\n" << setw(10) << "Symbol #" << setw(12) << "Symbol" << "Location\n";
    for ( auto st = m_symbolTable.begin(); st != m_symbolTable.end(); st++) {
        cout << setw(10) <<  symNum  << setw(12) << st->first;
        if ( st->second == multiplyDefinedSymbol ) {
            cout << "\n\tError: Multiply Defined Symbol\n";
        }
        else if ( st->second >= 10000 )
        {
            cout << "\n\tError: Insufficient memory\n";
        }
        else
        {
            cout << st->second << "\n";
        }
        symNum++;
    }
    cout << "\n\n";
}

/*
 SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
 
 NAME
    LookupSymbol - gets the address of a label
 SYNOPSIS
    bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
        a_symbol --> the label to lookup
        a_loc --> variable to return the address of the label
 DESCRIPTION
    look for label in map. if found, get the address and return true.
    otherwise return false to signify the label was unfound.
 RETURNS
    bool - whether the label was found in the table and it's location returned
 */
bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
{
    map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end())
    {
        a_loc = st->second;
        return true;
    }
    else
    {
        return false;
    }
}