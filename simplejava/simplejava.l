%option c++
%option noyywrap

%{
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

FlexLexer* lexer = new yyFlexLexer();
int curLineNum = 0;
int curPosInLine = 0;

void incrCurPos(int symNumber) {
    curPosInLine += symNumber;
}

%}

IDENT   [A-Za-z_][A-Za-z0-9_]*
INT     ([1-9]([0-9])*)|0
BOOLEAN ("true"|"false")
SPACE   [ \t]+
LINE_SEPARATOR [\n]+


%%
if          {  
                incrCurPos(2);
                std::cout << "IF "; 
            }
else        {  
                std::cout << "ELSE "; 
            }
while       {   
                std::cout << "WHILE "; 
            }
return      {   
                std::cout << "RETURN "; }
public      {   std::cout << "PUBLIC "; }
class       {   std::cout << "CLASS "; }
static      {   std::cout << "STATIC "; }
void        {   std::cout << "VOID "; }
main        {   std::cout << "MAIN "; }
String      {   std::cout << "STRING "; }
System.out.println { std::cout << "PRINT "; }
this        {   std::cout << "THIS ";  }
new         {   std::cout << "NEW ";  }
length      {   std::cout << "LENGTH ";  }
"int[]"     {   std::cout << "ARRAY ";  }
int         {   std::cout << "INT "; }
boolean     {   std::cout << "BOOLEAN "; }
"{"         {   std::cout << "LBRACE "; }
"}"         {   std::cout << "RBRACE "; }
"("         {   std::cout << "LPAREN "; }
")"         {   std::cout << "RPAREN "; }
"["         {   std::cout << "LBRACK "; }
"]"         {   std::cout << "RBRACK "; }
"<"         {   std::cout << "LEQ "; }
"="         {   std::cout << "EQ "; }
"&&"        {   std::cout << "AND "; }
"+"         {   std::cout << "PLUS "; }
"-"         {   std::cout << "MINUS "; }
"*"         {   std::cout << "MULT "; }
";"         {   std::cout << "SEMCOL "; }
","         {   std::cout << "COMMA "; }
"!"         {   std::cout << "BANG "; }
"."         {   std::cout << "DOT "; }
{BOOLEAN}   {
                std::cout << "BOOLEAN(" << yytext << ") "; 
            }

{INT}       {
                std::cout << "INT(" << atoi(yytext) << ") ";
            }

{IDENT}     {   
                curPosInLine += yyleng;
                std::cout << "IDENT(" << yytext << ") ";
            }

{SPACE}     {   ++curPosInLine;    }

{LINE_SEPARATOR}    {   ++curLineNum; 
                        curPosInLine = 0;
           }
.           { 
                std::cout << std::endl <<"UNKNOWN TOKEN(" << yytext << ") in line " << curLineNum << ": " /*<< curPosInLine*/ << std::endl;
                exit(1);
            }       

%%

int main(int argc, char** argv) {
    filebuf fb;
    istream is(fb.open(argv[1], ios::in));
    lexer->yylex(&is, &cout);
    std::cout << "EOF" << std::endl;
}

