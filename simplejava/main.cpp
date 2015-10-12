#include <stdio.h>
extern FILE * yyin;
extern int yyparse();

int main(int argc, char** argv) {
    FILE* progrFile;
    progrFile = fopen(argv[1], "r");
    yyin = progrFile;
    yyparse();
    return 0;
}