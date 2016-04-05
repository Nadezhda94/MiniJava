#include "common.h"
#include "Structs/Ast.h"
#include "Structs/Symbol.h"
#include "ASTVisitors/Printer.h"
#include "ASTVisitors/SymbolTableBuilder.h"
#include "ASTVisitors/TypeChecker.h"
#include "ASTVisitors/Translator.h"
#include "IRVisitors/Printer.h"
extern FILE * yyin;
extern int yyparse();
extern CProgramRuleNode* root;
Symbol::CStorage symbolsStorage;
using Translate::CTranslator;


void testBuilder(CSymbolTableBuilder& table_vis){
    auto itClass = table_vis.table.classInfo.begin();
    auto itClassEnd = table_vis.table.classInfo.end();
    std::cout << "____________TABLE_BUILDER__________" << std::endl;
    for (;itClass != itClassEnd; ++itClass) {
        std::cout << itClass -> name << std::endl;
        auto itVarsClass = itClass->vars.begin();
        auto itVarsClassEnd = itClass->vars.end();
        std::cout<< "\tvars:" << std::endl;
        for (;itVarsClass != itVarsClassEnd; ++itVarsClass) {
            std::cout << "\t\t" << itVarsClass->name << " " << itVarsClass->type << std::endl;
        }
        auto itMthd = itClass->methods.begin();
        auto itMthdEnd = itClass->methods.end();
        std::cout << "\tmethods:" <<std::endl;
        for (; itMthd != itMthdEnd; ++itMthd) {
            std::cout << "\t\t" << itMthd->name << std::endl;
            std::cout << "\t\t" << "params:" << std::endl;
            auto itParams = itMthd->params.begin();
            auto itParamsEnd = itMthd->params.end();
            for (;itParams != itParamsEnd; ++itParams) {
                std::cout << "\t\t\t" << itParams->name << " " << itParams->type << std::endl;
            }
            std::cout << "\t\tvars:" << std::endl;
            auto itVars = itMthd->vars.begin();
            auto itVarsEnd = itMthd->vars.end();
            for (;itVars != itVarsEnd; ++itVars) {
                std::cout << "\t\t\t" << itVars->name << " " << itVars->type << std::endl;
            }
            std::cout << "\t\treturnType: " << itMthd->returnType << std::endl;
            std::cout << "____________________________" << std::endl;

        }
    }
}

void storagePrinter() {
    std::cout << "________________Storage_____________" << std::endl;
        auto it = symbolsStorage.symbols.begin();
        auto itEnd = symbolsStorage.symbols.end();
        for (;it !=itEnd; ++it) {
            std:: cout << it->first << " " << it->second.get() << std::endl;
        }
}

int main(int argc, char** argv) {
	try {
        FILE* progrFile;
        progrFile = fopen(argv[1], "r");
        if (progrFile == NULL) {
          throw new invalid_argument("File not found");
        }
        yyin = progrFile;
        yyparse();

        // CPrinter print_vis;
        // root->accept(&print_vis);
        // cout<<endl;

        CSymbolTableBuilder table_vis(&symbolsStorage);
        root->accept(&table_vis);
        std::cout << "____________________________" << std::endl;
        //testBuilder(table_vis);

        CTypeChecker checker_vis(&symbolsStorage, table_vis.table);
        root->accept(&checker_vis);

        std::cout << "____________________________" << std::endl;
        CTranslator traslator_vis(&symbolsStorage, table_vis.table);
        root->accept(&traslator_vis);
        std::cout << "____________________________" << std::endl;

        CIRPrinter ir_print_vis(false);
        for (int i = 0; i < traslator_vis.trees.size(); ++i ) {
            traslator_vis.trees[i]->accept(&ir_print_vis);
        }

        // storagePrinter();
        delete root;
	} catch(const std::exception* e) {
		std::cerr << e->what() << std::endl;
        delete e;
	}
    return 0;
}
