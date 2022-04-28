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

string ConstAST::getVal(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    return this->data->val;
}

ColAST::ColAST(string name)
{
    int pos = name.find(".");
    if(pos == string::npos)
    {
        this->col_name = name;
        this->table_name = "";
    }
    else
    {
        this->table_name = name.substr(0, pos);
        this->col_name = name.substr(pos+1, name.size());
    }
}

string ColAST::fetch_value(Table_Row *row, Schema *s)
{
    for(int i=0;i<s->numColumns;i++)
    {
        if(s->columns[i]->name == this->col_name)
        {
            Entry entry = row->getField(i);
            int type = s->columns[i]->type;
            this->dt = type;
            if(type == INT)
                return to_string(entry.int_val);
            else if(type == DOUBLE)
                return to_string(entry.float_val);
            else
                return *(entry.str_val);
        }
    }
    return "";
}

// All getvals are yet to be implemeted! Currently its written only for testing basic query testing
string ColAST::getVal(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    if(s1->table_name == table_name)
        return this->fetch_value(row1, s1);
    if(row2 != NULL && s2->table_name == table_name)
        return this->fetch_value(row2, s2);
    return "";
}

UnaryArithAST::UnaryArithAST(ExprAST *child)
{
    this->child = child;
}

string UnaryArithAST::getVal(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    string val = this->child->getVal(row1, s1, row2, s2);

    if(this->child->getType() != VARCHAR)
        this->dt = this->child->getType();
    else
        cerr<<"Can't negate a string type"<<endl;

    if(val == "" || this->dt == VARCHAR)
        return "";
    if(this->dt == INT)
        return to_string(-stoi(val));
    if(this->dt == DOUBLE)
        return to_string(-stof(val));
    return "";
}

BinArithAST::BinArithAST(ExprAST *lhs, ExprAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

template <class T>
string BinArithAST::compute(T v1, T v2)
{
    if(this->op == _PLUS) return to_string(v1+v2);
    if(this->op == _MINUS) return to_string(v1-v2);
    if(this->op == _MULT) return to_string(v1*v2);
    if(this->op == _DIV) return to_string(v1/v2);
    return "";
}

string BinArithAST::getVal(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    string val1 = this->lhs->getVal(row1, s1, row2, s2);
    string val2 = this->rhs->getVal(row1, s1, row2, s2);

    if(this->lhs->getType() != rhs->getType())
        cerr<<"Mismatching types"<<endl;
    else
        this->dt = this->lhs->getType();

    if(val1 == "" || val2 == "" || this->dt == VARCHAR)
        return "";
    if(this->dt == INT)
    {
        int v1 = stoi(val1), v2 = stoi(val2);
        return this->compute<int>(v1, v2);
    }
    if(this->dt == DOUBLE)
    {
        float v1 = stof(val1), v2 = stof(val2);
        return this->compute<float>(v1, v2);
    }
    return "";
}

RelAST::RelAST(ExprAST *lhs, ExprAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

template <class T>
int RelAST::compare(T v1, T v2)
{
    if(this->op == _GT) return v1 > v2;
    if(this->op == _GE) return v1 >= v2;
    if(this->op == _LT) return v1 < v2;
    if(this->op == _LE) return v1 <= v2;
    if(this->op == _EQ) return v1 == v2;
    if(this->op == _NE) return v1 != v2;
    return true;
}

// All check_row funcs are not implemented just to check query processing basic
int RelAST::check_row(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    string val1 = this->lhs->getVal(row1, s1, row2, s2);
    string val2 = this->rhs->getVal(row1, s1, row2, s2);

    if(this->lhs->getType() != this->rhs->getType())
        cerr<<"Type mismatch"<<endl;

    if(val1 == "" || val2 == "")
        return C_ERROR;
    int dt = this->lhs->getType();
    if(dt == INT)
    {
        int v1 = stoi(val1), v2 = stoi(val2);
        return this->compare<int>(v1, v2);
    }
    if(dt == DOUBLE)
    {
        float v1 = stof(val1), v2 = stof(val2);
        return this->compare<float>(v1, v2);
    }
    if(dt == VARCHAR)
        return this->compare<string>(val1, val2);
    return true;
}

BinLogAST::BinLogAST(CondAST *lhs, CondAST *rhs, int op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

int BinLogAST::check_row(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    int _lhs = this->lhs->check_row(row1, s1, row2, s2);
    int _rhs = this->rhs->check_row(row1, s1, row2, s2);
    if(_lhs == C_ERROR || _rhs == C_ERROR)
        return C_ERROR;
    if(op == _AND)
        return _lhs && _rhs;
    if(op == _OR)
        return _lhs || _rhs;
    return true;
}

UnaryLogAST::UnaryLogAST(CondAST *child)
{
    this->child = child;
}

int UnaryLogAST::check_row(Table_Row *row1, Schema *s1, Table_Row *row2, Schema *s2)
{
    int val = this->child->check_row(row1, s1, row2, s2);
    if(val == C_ERROR)
        return val;
    return !val;
}