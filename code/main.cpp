#include "common.h"
#include "ASTVisitors/Printer.h"
#include "ASTVisitors/SymbolTableBuilder.h"
#include "ASTVisitors/TypeChecker.h"
#include "ASTVisitors/Translator.h"

#include "IRVisitors/Printer.h"
#include "IRVisitors/Canonizer.h"
#include "IRVisitors/Optimizer.h"
#include "IRVisitors/CodeGenerator.h"
#include "IRVisitors/RegAlloc.h"

extern FILE * yyin;
extern int yyparse();
extern CProgramRuleNode* root;
Symbol::CStorage symbolsStorage;

int main(int argc, char** argv) {
	try {
		ofstream ofs;
		ofstream gv;
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

		vector<INode*> trees(traslator_vis.trees);
		ofs.open("Logs/IRRaw.log", ofstream::out);
		gv.open("Logs/IRRaw.gv", ofstream::out);
		Canon::Print(ofs, gv, trees);
		gv.close();
		ofs.close();

		cout << "Canonizing IRT..." << endl;
		ofs.open("Logs/IRCanonized.log", ofstream::out);
		gv.open("Logs/IRCanonized.gv", ofstream::out);
		vector<IStm*> canonized_trees;
		Canon::Canonize(trees, canonized_trees);
		Canon::Print(ofs, gv, canonized_trees);
		gv.close();
		ofs.close();

		cout << "Linearizing IRT..." << endl;
		ofs.open("Logs/IRLinearized.log", ofstream::out);
		gv.open("Logs/IRLinearized.gv", ofstream::out);
		vector<shared_ptr<StmtList>> linearized_blocks;
		Canon::Linearize(canonized_trees, linearized_blocks);
		Canon::Print(ofs, gv, linearized_blocks);
		gv.close();
		ofs.close();

		cout << "Tracing IRT..." << endl;
		ofs.open("Logs/IRTraced.log", ofstream::out);
		gv.open("Logs/IRTraced.gv", ofstream::out);
		vector<shared_ptr<StmtList>> traced_blocks;
		Canon::Trace(linearized_blocks, traced_blocks);
		Canon::Print(ofs, gv, traced_blocks);
		gv.close();
		ofs.close();

		cout << "Generating ASM code..." << endl;
		ofs.open("Logs/CodeGen.log", ofstream::out);
		vector<shared_ptr<CInstrList>> blockInstrs;
		CodeGenerator::GenerateCode(ofs, traced_blocks, blockInstrs);
		ofs.close();

		cout << "Flow graph building.." << endl;
		ofs.open("Logs/FlowGraph.log", ofstream::out);
		vector<shared_ptr<FlowGraph::CFlowGraph>> graphs;
		RegAlloc::BuildFlowGraph(ofs, blockInstrs, graphs);
		ofs.close();

		cout << "Interference graph building.." << endl;
		ofs.open("Logs/InterferenceGraph.log", ofstream::out);
		vector<shared_ptr<RegAlloc::CInterferenceGraph>> interferenceGraphs;
		RegAlloc::BuildInterferenceGraph(ofs, graphs, interferenceGraphs);
		ofs.close();

		cout << "SUCCESS" << endl;
        // storagePrinter();
        delete root;
	} catch(const exception* e) {
		cerr << e->what() << endl;
		delete e;
		return 1;
	}
	return 0;
}
