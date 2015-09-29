#include <iostream>
using namespace std;


struct VariablesTable{
    const char* const name;
    const int value;
    const VariablesTable* const prev;

    VariablesTable ( const char* _name, const int _value, const VariablesTable* _prev ):
    name(_name), value(_value), prev(_prev){}
};

class Stm{
    public:
        virtual VariablesTable* Execute(VariablesTable* table) const = 0;
        virtual ~Stm(){}
};

class Exp{
    public:
        virtual VariablesTable* Execute(VariablesTable* table) const = 0;
        virtual ~Exp(){}
};

class BinOp{
    public:
        virtual VariablesTable* Execute(VariablesTable* table) const = 0;
        virtual ~BinOp(){}
};

class ExpList{
    public:
        virtual VariablesTable* Execute(VariablesTable* table) const = 0;
        virtual ~ExpList(){}
};

class CompoundStm : public Stm{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            VariablesTable* first_table = first->Execute(table);
            VariablesTable* second_table = second->Execute(first_table);
            return second_table;
        }
        CompoundStm(const Stm* _first, const Stm* _second) : first(_first), second(_second) {}
        ~CompoundStm(){}
    private:
        const Stm* const first;
        const Stm* const second;
};

class AssignStm: public Stm{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            VariablesTable* exp_table = exp->Execute(table);
            VariablesTable* ret_table = new VariablesTable(name, exp_table->value, exp_table);
            return ret_table;
        }
        AssignStm(const Exp* _exp, const char* _name) : exp(_exp), name(_name){}
        ~AssignStm(){}
    private:
        const char* name;
        const Exp* const exp;
};

class PrintStm: public Stm{
    public:
        VariablesTable* Execute(VariablesTable* table) const{

        }
        PrintStm(const Exp** _explist) : explist(_explist) {}
        ~PrintStm(){}
    private:
        const Exp** const explist;
};

class IdExp: public Exp{
    public:
        VariablesTable* Execute(VariablesTable* table) const {}
        IdExp(const Exp* _exp) : exp(_exp) {}
        ~IdExp(){}
    private:
        const Exp* const exp;
};


class NumExp : public Exp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            VariablesTable* ret_table = new VariablesTable(name, num, table);
            return ret_table;
        }
        NumExp(const int _num) : num(_num) {}
        ~NumExp(){}
    private:
        const int num;
};

class OpExp : public Exp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            VariablesTable* left_table = left->Execute(table);
            int left_value = left_table->value;
            VariablesTable* right_table = right->Execute(left_table);
            VariablesTable* ret_table = new VariablesTable(name, left_value, right_table);
            return op->Execute(ret_table);
        }
        OpExp(const Exp* _left, const BinOp* _op, const Exp* _right) : left(_left), op(_op), right(_right) {}
        ~OpExp(){}
    private:
        const Exp* const left;
        const BinOp* const op;
        const Exp* const right;
};

class EseqExp : public Exp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{}
        EseqExp(const Stm* _stm, const Exp* _exp) : stm(_stm), exp(_exp) {}
        ~EseqExp(){}
    private:
        const Stm* const stm;
        const Exp* const exp;
};

class PairExpList : public ExpList{
    public:
        VariablesTable* Execute(VariablesTable* table) const{}
        PairExpList(const Exp* _exp, const ExpList* _explist) : exp(_exp), explist(_explist) {}
        ~PairExpList(){}
    private:
        const Exp* const exp;
        const ExpList* const explist;
};

class LastExpList : public ExpList{
    public:
        VariablesTable* Execute(VariablesTable* table) const{}
        LastExpList(const Exp* _exp) : exp(_exp) {}
        ~LastExpList(){}
    private:
        const Exp* const exp;
};

class Plus : public BinOp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            return new VariablesTable(name,table->value+table->prev->value, table);
        }
        Plus(){}
        ~Plus(){}
};

class Minus : public BinOp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            return new VariablesTable(name,table->value-table->prev->value, table);
        }
        Minus(){}
        ~Minus(){}
};

class Times : public BinOp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            return new VariablesTable(name,table->value*table->prev->value, table);
        }
        Times(){}
        ~Times(){}
};

class Div : public BinOp{
    public:
        VariablesTable* Execute(VariablesTable* table) const{
            const char* name = "anon";
            return new VariablesTable(name,table->value/table->prev->value, table);
        }
        Div(){}
        ~Div(){}
};

int main(){
    VariablesTable* table;
    NumExp left = NumExp(5);
    NumExp right = NumExp(3);
    Plus op = Plus();
    OpExp opexp = OpExp(&left, &op, &right);
    AssignStm stm = AssignStm(&opexp, "a");
    table = stm.Execute(table);
    cout<<table->value<<endl;
}
