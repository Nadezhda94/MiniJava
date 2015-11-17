#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include "ast.h"
#include "CPrintVisitor.h"
#include "CSymbolTableBuilder.h"
#include "CTypeChecker.h"
using std::invalid_argument;
using std::cout;
using std::endl;
extern FILE * yyin;
extern int yyparse();
extern CProgramRuleNode* root;

void testBuilder(CSymbolTableBuilder& table_vis){
    auto it = table_vis.table.classInfo[1].methods.begin();
    auto itEnd = table_vis.table.classInfo[1].methods.end();
    for ( ; it != itEnd; ++it) {
        std::cout << it->name << std::endl;
        auto innerIt = it->vars.begin();
        auto innerItEnd = it->vars.end();
        for (;innerIt != innerItEnd; ++innerIt) {
            std::cerr << innerIt -> name << " " << innerIt->type << std::endl;
        }
    }
    std::cout << table_vis.table.classInfo[1].name << std::endl;
}

int main(int argc, char** argv) {
    FILE* progrFile;
    progrFile = fopen(argv[1], "r");
    if (progrFile == NULL)
      throw new invalid_argument("File not found");
    yyin = progrFile;
    yyparse();

    CPrintVisitor print_vis;
    root->accept(&print_vis);
    cout<<endl;

    CSymbolTableBuilder table_vis;
    root->accept(&table_vis);
    //            testBuilder(table_vis);

    CTypeChecker checker_vis;
    checker_vis.table = table_vis.table;
    root->accept(&checker_vis);

    delete root;
    return 0;
}
