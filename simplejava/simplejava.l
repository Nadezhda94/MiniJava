%option noyywrap

%{
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "simplejava.tab.h"

using namespace std;

//FlexLexer* lexer = new yyFlexLexer();
int curLineNum = 0;
int curPosInLine = 0;

void yyerror(const char *);

void incrCurPos() {
    curPosInLine += yyleng;
}

%}

IDENT   [A-Za-z_][A-Za-z0-9_]*
INT     ([1-9]([0-9])*)|0
BOOLEAN ("true"|"false")
SPACE   [ \t]+
LINE_SEPARATOR [\n]+
END_OF_FILE <<EOF>>


%%
if          {
                incrCurPos();
                return IF;
                //std::cout << "IF ";
            }
else        {
                incrCurPos();
                return ELSE;
                //std::cout << "ELSE ";
            }
while       {
                incrCurPos();
                return WHILE;
                //std::cout << "WHILE ";
            }
return      {
                incrCurPos();
                return RETURN;
                //std::cout << "RETURN ";
            }
public      {
                incrCurPos();
                return PUBLIC;
                //std::cout << "PUBLIC ";
            }
class       {
                incrCurPos();
                return CLASS;
                //std::cout << "CLASS ";
            }
static      {
                incrCurPos();
                return STATIC;
                //std::cout << "STATIC ";
            }
void        {
                incrCurPos();
                return VOID;
                //std::cout << "VOID ";
            }
main        {
                incrCurPos();
                return MAIN;
                //std::cout << "MAIN ";
            }
String      {
                incrCurPos();
                return STRING;
                //std::cout << "STRING ";
            }
System.out.println  {
                        incrCurPos();
                        return PRINT;
                        //std::cout << "PRINT ";
                    }
this        {
                incrCurPos();
                return THIS;
                //std::cout << "THIS ";
            }
new         {
                incrCurPos();
                return NEW;
                //std::cout << "NEW ";
            }
length      {
                incrCurPos();
                return LENGTH;
                //std::cout << "LENGTH ";
            }
"int[]"     {
                incrCurPos();
                return ARRAY;
                //std::cout << "ARRAY ";
            }
int         {
                incrCurPos();
                return INT_TYPE;
                //std::cout << "INT";
            }
boolean     {
                incrCurPos();
                return BOOLEAN_TYPE;
                //std::cout << "BOOLEAN ";
            }
"{"         {
                incrCurPos();
                return LBRACE;
                //std::cout << "LBRACE ";
            }
"}"         {
                incrCurPos();
                return RBRACE;
                //std::cout << "RBRACE ";
            }
"("         {
                incrCurPos();
                return LPAREN;
                //std::cout << "LPAREN ";
            }
")"         {
                incrCurPos();
                return RPAREN;
                //std::cout << "RPAREN ";
            }
"["         {
                incrCurPos();
                return LBRACK;
                //std::cout << "LBRACK ";
            }
"]"         {
                incrCurPos();
                return RBRACK;
                //std::cout << "RBRACK ";
            }
"<"         {
                incrCurPos();
                return LEQ;
                //std::cout << "LEQ ";
            }
"="         {
                incrCurPos();
                return EQ;
                //std::cout << "EQ ";
            }
"&&"        {
                incrCurPos();
                return AND;
                //std::cout << "AND ";
            }
"+"         {
                incrCurPos();
                return PLUS;
                //std::cout << "PLUS ";
            }
"-"         {
                incrCurPos();
                return MINUS;
                //std::cout << "MINUS ";
            }
"*"         {
                incrCurPos();
                return MULT;
                //std::cout << "MULT ";
            }
";"         {
                incrCurPos();
                return SEMCOL;
                //std::cout << "SEMCOL ";
            }
","         {
                incrCurPos();
                return COMMA;
                //std::cout << "COMMA ";
            }
"!"         {
                incrCurPos();
                return BANG;
                //std::cout << "BANG ";
            }
"."         {
                incrCurPos();
                return DOT;
                //std::cout << "DOT ";
            }
{BOOLEAN}   {
                incrCurPos();
                yylval.boolValue = (yytext[0] == 't');
                return BOOLEAN;
                //std::cout << "BOOLEAN(" << yytext << ") ";
            }

{INT}       {
                incrCurPos();
                yylval.intValue = atoi(yytext);
                return INT;
                //std::cout << "INT(" << atoi(yytext) << ") ";
            }

{IDENT}     {
                incrCurPos();
                strcpy(yylval.identName, yytext);
                return IDENT;
                //curPosInLine += yyleng;
                //std::cout << "IDENT(" << yytext << ") ";
            }

{SPACE}     {   ++curPosInLine;    }

{LINE_SEPARATOR}    {   ++curLineNum;
                        curPosInLine = 0;
            }
.           {
                std::cout << std::endl <<"UNKNOWN TOKEN(" << yytext << ") in line " << curLineNum << ": " /*<< curPosInLine*/ << std::endl;
                exit(1);
            }

<<EOF>>     { return END_OF_FILE;}

%%
