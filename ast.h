#ifndef AST_H
#define AST_H

#include<string>
#include "utils.h"
#include "query.h"

# define _AND 0
# define _OR 1
# define _MULT 2
# define _PLUS 3
# define _MINUS 4
# define _DIV 5
# define _GE 6
# define _LT 7
# define _GT 8
# define _LE 9
# define _NE 10
# define _EQ 11

# define _TEXT 0
# define _INT 1
# define _FLOAT 2

class Constant 
{
    public:
    string val;
    int dt;

    Constant(string val, int dt)
    {
        this->val = val;
        this->dt = dt;
    }
};

class Range
{
    public:
    Value lower_bound;
    Value upper_bound;

    Range(Constant lower, Constant upper)
    {
        if(lower.dt == _INT)
        {
            this->lower_bound.int_val = atoi(lower.val.c_str());
            this->upper_bound.float_val = atoi(upper.val.c_str());
        }
        else if(upper.dt == _FLOAT)
        {
            this->lower_bound.float_val = atof(lower.val.c_str());
            this->upper_bound.float_val = atof(upper.val.c_str());
        }
        else
        {
            this->lower_bound.str_val = &lower.val;
            this->upper_bound.str_val = &upper.val;
        }
    }
};

class AST
{
    public:
    virtual ~AST();
    virtual bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class BinArithAST: AST
{
    AST *lhs, rhs;
    int op;

    public:
    BinArithAST(AST *lhs, AST *rhs, int op);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};  

class BinLogAST: AST
{
    AST *lhs, rhs;
    int op;

    public:
    BinLogAST(AST *lhs, AST *rhs, int op);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
}; 

class BinRelAST: AST
{
    AST *lhs, rhs;
    int op;

    public:
    BinRelAST(AST *lhs, AST *rhs, int op);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class UnaryLogAST: AST
{
    AST *child;
    
    public:
    UnaryLogAST(AST *child);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class ConstAST: AST
{
    Constant data;

    public:
    ConstAST(Constant data);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class ColAST: AST
{
    string col;

    public:
    ColAST(string name);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

#endif