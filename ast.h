#ifndef _AST_H
#define _AST_H

#include<string>
#include "utils.h"
using namespace std;

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
# define __NE 10
# define _EQ 11

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
        if(lower.dt == INT)
        {
            this->lower_bound.int_val = stoi(lower.val);
            this->upper_bound.int_val = stoi(upper.val);
        }
        else if(upper.dt == DOUBLE)
        {
            this->lower_bound.float_val = stof(lower.val);
            this->upper_bound.float_val = stof(upper.val);
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
    virtual ~AST(){};
};

class ExprAST: public AST
{
    protected:
    int dt;

    public:
    virtual ~ExprAST(){};
    virtual string getVal(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL){return "";};
    int getType();
};  

class ConstAST: public ExprAST
{
    Constant *data;

    public:
    ConstAST(Constant *data);
    string getVal(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
};

class ColAST: public ExprAST
{
    string col_name;
    string table_name;

    public:
    ColAST(string name);
    string getVal(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
    string fetch_value(Table_Row *row, Schema *s);
};

class UnaryArithAST: public ExprAST
{
    ExprAST *child;
    public:
    UnaryArithAST(ExprAST *child);
    string getVal(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
};

class BinArithAST: public ExprAST
{
    ExprAST *lhs, *rhs;
    int op;

    public:
    BinArithAST(ExprAST *lhs, ExprAST *rhs, int op);
    string getVal(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
    template <class T>
    string compute(T v1, T v2);
};

class CondAST: public AST
{
    public:
    virtual ~CondAST(){};
    virtual int check_row(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL){return true;};
}; 

class RelAST: public CondAST
{
    ExprAST *lhs, *rhs;
    int op;

    public:
    RelAST(ExprAST *lhs, ExprAST *rhs, int op);
    ~RelAST(){};
    int check_row(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
    template <class T>
    int compare(T v1, T v2);
};

class BinLogAST: public CondAST
{
    CondAST *lhs, *rhs;
    int op;

    public:
    BinLogAST(CondAST *lhs, CondAST *rhs, int op);
    int check_row(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
};

class UnaryLogAST: public CondAST
{
    CondAST *child;
    
    public:
    UnaryLogAST(CondAST *child);
    int check_row(Table_Row *row1, Schema *s1, Table_Row *row2 = NULL, Schema *s2 = NULL);
};

#endif
