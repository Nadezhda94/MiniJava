/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SIMPLEJAVA_TAB_HPP_INCLUDED
# define YY_YY_SIMPLEJAVA_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    BOOLEAN = 259,
    IDENT = 260,
    INT_TYPE = 261,
    BOOLEAN_TYPE = 262,
    EXTENDS = 263,
    EQ = 264,
    PLUS = 265,
    IF = 266,
    ELSE = 267,
    WHILE = 268,
    RETURN = 269,
    PUBLIC = 270,
    CLASS = 271,
    STATIC = 272,
    VOID = 273,
    MAIN = 274,
    STRING = 275,
    PRINT = 276,
    THIS = 277,
    NEW = 278,
    LENGTH = 279,
    ARRAY = 280,
    LBRACE = 281,
    RBRACE = 282,
    LPAREN = 283,
    RPAREN = 284,
    LBRACK = 285,
    RBRACK = 286,
    LEQ = 287,
    AND = 288,
    MINUS = 289,
    MULT = 290,
    DIV = 291,
    SEMCOL = 292,
    COMMA = 293,
    BANG = 294,
    DOT = 295,
    UMINUS = 296,
    UPLUS = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 25 "simplejava.y" /* yacc.c:1909  */

    int intValue;
    bool boolValue;
    char str[4096];

#line 103 "simplejava.tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SIMPLEJAVA_TAB_HPP_INCLUDED  */
