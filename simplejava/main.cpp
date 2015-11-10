#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include "ast.h"
#include "CSymbolTableBuilder.h"

using std::invalid_argument;
using std::cout;
using std::endl;
extern FILE * yyin;
extern int yyparse();

int main(int argc, char** argv) {
	try {
    FILE* progrFile;
    progrFile = fopen(argv[1], "r");
    if (progrFile == NULL) {
      throw new invalid_argument("File not found");
    }
    yyin = progrFile;
    yyparse();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
    return 0;
}
