#include "ast.h"

Constant::Constant(string val, int dt)
{
    this->val = val;
    this->dt = dt;
}

int ExprAST::getType()
{
    return this->dt;
}

ConstAST::ConstAST(Constant *data)
{
    this->data = data;
    this->dt = data->dt;
}

string ConstAST::getVal(Table_Row *row1, Table_Row *row2)
{
    return "";
}

ColAST::ColAST(string name)
{
    if(name.find(".") == name.size())
        this->col = name;
    else
    {
        this->table_name = name.substr(0, name.find("."));
        this->col = name.substr(name.find("."), name.size());
    }
}

// All getvals are yet to be implemeted! Currently its written only for testing basic query testing
string ColAST::getVal(Table_Row *row1, Table_Row *row2)
{
    return "";
}

UnaryArithAST::UnaryArithAST(ExprAST *child)
{
    this->child = child;
    if(this->child->getType() != _TEXT)
        this->dt = this->child->getType();
    else
        cerr<<"Can't negate a string type"<<endl;
}

string UnaryArithAST::getVal(Table_Row *row1, Table_Row *row2)
{
    return "";
}

BinArithAST::BinArithAST(ExprAST *lhs, ExprAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
    
    if(this->lhs->getType() != rhs->getType())
        cerr<<"Mismatching types"<<endl;
    else
        this->dt = this->lhs->getType();
}

string BinArithAST::getVal(Table_Row *row1, Table_Row *row2)
{
    return "";
}

RelAST::RelAST(ExprAST *lhs, ExprAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;

    if(this->lhs->getType() != this->rhs->getType())
        cerr<<"Type mismatch"<<endl;
}

// All check_row funcs are not implemented just to check query processing basic
bool check_row(Table_Row *row1, Table_Row *row2)
{
    return true;
}

BinLogAST::BinLogAST(CondAST *lhs, CondAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

bool BinLogAST::check_row(Table_Row *row1, Table_Row *row2)
{
    return true;
}

UnaryLogAST::UnaryLogAST(CondAST *child)
{
    this->child = child;
}

bool UnaryLogAST::check_row(Table_Row *row1, Table_Row *row2)
{
    return true;
}