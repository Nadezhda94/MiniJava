#include "SymbolsTable.h"

namespace SymbolsTable {
	//--------------------------------------------------------------------------------------------------------------
	// CVarInfo
	//--------------------------------------------------------------------------------------------------------------
	CVarInfo::CVarInfo( const CSymbol* _name, const CSymbol* _type ) : name( _name ), type( _type ) { }

	//--------------------------------------------------------------------------------------------------------------
	// CMethodInfo
	//--------------------------------------------------------------------------------------------------------------
	CMethodInfo::CMethodInfo( const CSymbol* _name, const CSymbol* _returnType ) :
			name( _name ), returnType( _returnType ), vars(), params() { }

	//--------------------------------------------------------------------------------------------------------------
	// CClassInfo
	//--------------------------------------------------------------------------------------------------------------
	CClassInfo::CClassInfo( const CSymbol* _name ) :
			name( _name ), vars(), methods(), parent() { }

	CMethodInfo &CClassInfo::getMethodInfo( const CSymbol* name ) {
		for ( int i = 0; i < methods.size(); i++ )
			if ( name->getString() == methods[i].name->getString())
				return methods[i];
		throw new logic_error("Not found in getMethodInfo");
	}

	//--------------------------------------------------------------------------------------------------------------
	// CTable
	//--------------------------------------------------------------------------------------------------------------

	CTable::CTable() : classInfo() { }

	CClassInfo &CTable::getClassInfo( const CSymbol* name ) {
		for ( int i = 0; i < classInfo.size(); i++ )
			if ( name->getString() == classInfo[i].name->getString())
				return classInfo[i];
		throw new logic_error("Not found in getClassInfo");
	}

	vector<CVarInfo> CTable::getParentVars( const CSymbol* name ) {
		vector<CVarInfo> ans;
		CClassInfo* current = &getClassInfo( name );
		do {
			if ( current->parent != 0 ) {
				current = &getClassInfo( current->parent );
				ans.insert( ans.begin(), current->vars.begin(), current->vars.end());
			}
		} while ( current->parent != 0 );
		return ans;
	}
}