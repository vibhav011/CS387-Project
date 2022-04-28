/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 15 "parser.y"

  typedef void* yyscan_t;

#line 52 "parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DOT_NAME = 258,
    NAME = 259,
    TEXT_CONSTANT = 260,
    INT_CONSTANT = 261,
    FLOAT_CONSTANT = 262,
    INTEGER = 263,
    FLOAT = 264,
    TEXT = 265,
    AND = 266,
    OR = 267,
    NOT = 268,
    MULT = 269,
    PLUS = 270,
    MINUS = 271,
    DIV = 272,
    GE = 273,
    LT = 274,
    GT = 275,
    LE = 276,
    NE = 277,
    EQ = 278,
    SEMICOLON = 279,
    COMMIT = 280,
    ROLLBACK = 281,
    WITH = 282,
    COMMA = 283,
    AS = 284,
    ROUND_BRACKET_OPEN = 285,
    ROUND_BRACKET_CLOSE = 286,
    SELECT = 287,
    FROM = 288,
    WHERE = 289,
    CREATE = 290,
    TABLE = 291,
    RANGE = 292,
    PRIMARY = 293,
    KEY = 294,
    INSERT = 295,
    INTO = 296,
    VALUES = 297,
    ASSIGN = 298,
    UPDATE = 299,
    SET = 300,
    DELETE = 301,
    INFINITY = 302,
    BETWEEN = 303,
    REDC = 304
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 26 "parser.y"

    int int_val;
    string* str;
    Range* range;
    Constant* constant;
    ColumnDesc* col_desc;
    Update_Pair* update_pair;
    Temp_Table* table;
    pair<string, vector<string> >* name_cols;
    vector<Temp_Table*>* vec_table;
    vector<string>* vec_string;
    vector<ColumnDesc*>* vec_col_desc;
    vector<Update_Pair*>* vec_update_pair;
    ExprAST* exprast;
    CondAST* logast;
    RelAST* relast;

#line 131 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
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



int yyparse (yyscan_t scanner);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
