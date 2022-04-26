#ifndef AST_H
#define AST_H

#include<string>
#include "utils.h"
#include "receiver/query.h"

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

    Constant(string val, int dt);
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
};

class ExprAST: AST
{
    protected:
    int dt;

    public:
    virtual ~ExprAST();
    virtual string getVal(Table_Row *row1, Table_Row *row2){};
    int getType();
};  

class ConstAST: ExprAST
{
    Constant *data;

    public:
    ConstAST(Constant *data);
    string getVal(Table_Row *row1, Table_Row *row2);
};

class ColAST: ExprAST
{
    string col;
    string table_name;

    public:
    ColAST(string name);
    string getVal(Table_Row* row1, Table_Row *row2);
};

class UnaryArithAST: ExprAST
{
    ExprAST *child;
    public:
    UnaryArithAST(ExprAST *child);
    string getVal(Table_Row *row1, Table_Row *row2);
};

class BinArithAST: ExprAST
{
    ExprAST *lhs, *rhs;
    int op;

    public:
    BinArithAST(ExprAST *lhs, ExprAST *rhs, int op);
    string getVal(Table_Row *row1, Table_Row *row2);
};

class CondAST: AST
{
    public:
    virtual ~CondAST();
    virtual bool check_row(Table_Row *row1, Table_Row *row2=NULL){};
}; 

class RelAST: CondAST
{
    ExprAST *lhs, *rhs;
    int op;

    public:
    RelAST(ExprAST *lhs, ExprAST *rhs, int op);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class BinLogAST: CondAST
{
    CondAST *lhs, *rhs;
    int op;

    public:
    BinLogAST(CondAST *lhs, CondAST *rhs, int op);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

class UnaryLogAST: CondAST
{
    CondAST *child;
    
    public:
    UnaryLogAST(CondAST *child);
    bool check_row(Table_Row *row1, Table_Row *row2=NULL);
};

#endif