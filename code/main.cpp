#include "common.h"
#include "ASTVisitors/Printer.h"
#include "ASTVisitors/SymbolTableBuilder.h"
#include "ASTVisitors/TypeChecker.h"
#include "ASTVisitors/Translator.h"

#include "IRVisitors/Printer.h"
#include "IRVisitors/Canonizer.h"
#include "IRVisitors/JumpOptimizer.h"

extern FILE * yyin;
extern int yyparse();
extern CProgramRuleNode* root;
Symbol::CStorage symbolsStorage;

int main(int argc, char** argv) {
	try {
		ofstream ofs;
        FILE* progrFile;
        progrFile = fopen(argv[1], "r");
        if (progrFile == NULL) {
          throw new invalid_argument("File not found");
        }
		cout << "Parsing..." << endl;
        yyin = progrFile;
        yyparse();

		cout<<"Printing AST..."<<endl;
		ofs.open("Logs/Ast.log", ofstream::out);
        CPrinter print_vis(ofs);
        root->accept(&print_vis);
        ofs.close();

		cout << "Building symbol table..." << endl;
        CSymbolTableBuilder table_vis(&symbolsStorage);
        root->accept(&table_vis);

		cout << "Printing symbol table..." << endl;
		ofs.open("Logs/SymbolTable.log", ofstream::out);
		testBuilder(ofs, table_vis);
		ofs.close();

		cout << "Checking types..." << endl;
        CTypeChecker checker_vis(&symbolsStorage, table_vis.table);
        root->accept(&checker_vis);

        cout << "Translating AST to IRT..." << endl;
		Translate::CTranslator traslator_vis(&symbolsStorage, table_vis.table);
        root->accept(&traslator_vis);

		ofs.open("Logs/Canonizer.log", ofstream::out);
        Canon::optimize(ofs, symbolsStorage, traslator_vis.trees);
		ofs.close();
        // storagePrinter();
        delete root;
	} catch(const exception* e) {
		cerr << e->what() << endl;
		delete e;
	}
	return 0;
}
