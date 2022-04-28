/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 53 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DOT_NAME = 258,                /* DOT_NAME  */
    NAME = 259,                    /* NAME  */
    TEXT_CONSTANT = 260,           /* TEXT_CONSTANT  */
    INT_CONSTANT = 261,            /* INT_CONSTANT  */
    FLOAT_CONSTANT = 262,          /* FLOAT_CONSTANT  */
    INTEGER = 263,                 /* INTEGER  */
    FLOAT = 264,                   /* FLOAT  */
    TEXT = 265,                    /* TEXT  */
    AND = 266,                     /* AND  */
    OR = 267,                      /* OR  */
    NOT = 268,                     /* NOT  */
    MULT = 269,                    /* MULT  */
    PLUS = 270,                    /* PLUS  */
    MINUS = 271,                   /* MINUS  */
    DIV = 272,                     /* DIV  */
    GE = 273,                      /* GE  */
    LT = 274,                      /* LT  */
    GT = 275,                      /* GT  */
    LE = 276,                      /* LE  */
    NE = 277,                      /* NE  */
    EQ = 278,                      /* EQ  */
    SEMICOLON = 279,               /* SEMICOLON  */
    COMMIT = 280,                  /* COMMIT  */
    ROLLBACK = 281,                /* ROLLBACK  */
    WITH = 282,                    /* WITH  */
    COMMA = 283,                   /* COMMA  */
    AS = 284,                      /* AS  */
    ROUND_BRACKET_OPEN = 285,      /* ROUND_BRACKET_OPEN  */
    ROUND_BRACKET_CLOSE = 286,     /* ROUND_BRACKET_CLOSE  */
    SELECT = 287,                  /* SELECT  */
    FROM = 288,                    /* FROM  */
    WHERE = 289,                   /* WHERE  */
    CREATE = 290,                  /* CREATE  */
    TABLE = 291,                   /* TABLE  */
    RANGE = 292,                   /* RANGE  */
    PRIMARY = 293,                 /* PRIMARY  */
    KEY = 294,                     /* KEY  */
    INSERT = 295,                  /* INSERT  */
    INTO = 296,                    /* INTO  */
    VALUES = 297,                  /* VALUES  */
    ASSIGN = 298,                  /* ASSIGN  */
    UPDATE = 299,                  /* UPDATE  */
    SET = 300,                     /* SET  */
    DELETE = 301,                  /* DELETE  */
    INFINITY = 302,                /* INFINITY  */
    BETWEEN = 303,                 /* BETWEEN  */
    REDC = 304                     /* REDC  */
  };
  typedef enum yytokentype yytoken_kind_t;
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

#line 137 "parser.tab.h"

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
