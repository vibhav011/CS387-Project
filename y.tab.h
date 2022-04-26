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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    DOT_NAME = 258,
    NAME = 259,
    TEXT_CONSTANT = 260,
    INT_CONSTANT = 261,
    FLOAT_CONSTANT = 262,
    INT = 263,
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
    TWO = 304
  };
#endif
/* Tokens.  */
#define DOT_NAME 258
#define NAME 259
#define TEXT_CONSTANT 260
#define INT_CONSTANT 261
#define FLOAT_CONSTANT 262
#define INT 263
#define FLOAT 264
#define TEXT 265
#define AND 266
#define OR 267
#define NOT 268
#define MULT 269
#define PLUS 270
#define MINUS 271
#define DIV 272
#define GE 273
#define LT 274
#define GT 275
#define LE 276
#define NE 277
#define EQ 278
#define SEMICOLON 279
#define COMMIT 280
#define ROLLBACK 281
#define WITH 282
#define COMMA 283
#define AS 284
#define ROUND_BRACKET_OPEN 285
#define ROUND_BRACKET_CLOSE 286
#define SELECT 287
#define FROM 288
#define WHERE 289
#define CREATE 290
#define TABLE 291
#define RANGE 292
#define PRIMARY 293
#define KEY 294
#define INSERT 295
#define INTO 296
#define VALUES 297
#define ASSIGN 298
#define UPDATE 299
#define SET 300
#define DELETE 301
#define INFINITY 302
#define BETWEEN 303
#define TWO 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 8 "parser.y"

    int int_val;
    string* str;
    Range* range;
    Constant* constant;
    Column_Desc* col_desc;
    Update_pair* update_pair;
    Temp_Table* table;
    pair<string, vector<string> >* name_cols;
    vector<Temp_Table*>* vec_table;
    vector<string>* vec_string;
    vector<Column_Desc*>* vec_col_desc;
    vector<Update_pair*>* vec_update_pair;
    ExprAST* exprast;
    CondAST* logast;
    RelAST* relast;

#line 173 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
