#include <stdio.h>
#include <stdexcept>
#include <iostream>
using std::invalid_argument;
using std::cout;
using std::endl;
extern FILE * yyin;
extern int yyparse();

int main(int argc, char** argv) {
    FILE* progrFile;
    progrFile = fopen(argv[1], "r");
    if (progrFile == NULL)
      throw new invalid_argument("File not found");
    yyin = progrFile;
    cout<<"Begin"<<endl;
    yyparse();
    return 0;
}
