#include <stdio.h>
#include <iostream>
extern FILE * yyin;
extern int yyparse();

int main(int argc, char** argv) {
    FILE* progrFile;
    progrFile = fopen(argv[1], "r");
    yyin = progrFile;
    std::cout << "blabla";
    std::cout<< yyparse();
    std::cout << "blabla";
    return 0;
}