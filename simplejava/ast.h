

class CInvokeExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(CVisitor* visit) {
       visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CLengthExpressionNode: public CExpressionNode {
public:
    CLengthExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
       visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

enum ArithmeticOpType {
    PLUS, MINUS, MULT, DIV
};

class CArithmeticExpressionNode: public CExpressionNode {
public:
    CArithmeticExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp, ArithmeticOpType opType) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(CVisitor* visit) {
       visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
    ArithmeticOpType opType;
};

class CCompareExpressionNode: public CExpressionNode {
public:
    CCompareExpressionNode(CExpressionNode* _firstExp, CExpressionNode* _secondExp) : firstExp(_firstExp), secondExp(_secondExp) {}
    void accept(CVisitor* visit) {
        visitor->visit(this);
    }
private:
    CExpressionNode* firstExp;
    CExpressionNode* secondExp;
};

class CNotExpressionNode: public CExpressionNode {
public:
    CNotExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CNewArrayExpressionNode: public CExpressionNode {
public:
    CNewArrayExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CNewObjectExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(const char* _objType) : objType(_objType) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string objType;
};

class CIntExpressionNode: public CExpressionNode {
public:
    CIntExpressionNode(int _value) : value(_value) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    int value;
};

class CBooleanExpressionNode: public CExpressionNode {
public:
    CNewObjectExpressionNode(bool _value) : value(_value) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    bool value;
};

class CIdentExpressionNode: public CExpressionNode {
public:
    CIdentExpressionNode(char* _name) : name(_name) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string name;
};

class CThisExpressionNode: public CExpressionNode {
public:
    CThisExpressionNode(const char* _name) : name(_name) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    string name;
};

class CParenExpressionNode: public CExpressionNode {
public:
    CParenExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CInvokeMethodExpressionNode: public CExpressionNode {
public:
    CInvokeExpressionNode(CExpressionNode* _exp, const char* _name, CExpArgNode* _args): 
                                                    exp(_exp), name(_name), args(_args) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CExpressionNode exp;
    string name;
    CExpArgNode args;
};

class CTailStatsNode: public CStatsNode {
public:
    CTailStatsNode(CStatementNode* _stm) : stm(_stm) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CStatementNode* stm;
};

class CFirstStatsNode: public CStatsNode {
public:
    CFirstStatsNode(CStatsNode* _firstStats,  CStatementNode* _stm) : firstStats(_firstStats), stm(_stm) {}
    void accept(CVisitor* visitor) {
        visitor->visit(this);
    }
private:
    CStatsNode* firstStats;
    CStatementNode* stm;
};

class CFewArgsExpressionNode: public CExpArgNode {
public:
    CFewArgsExpressionNode(CExpressionsNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
       visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};

class CEmptyArgsExpression: public CExpArgNode {
public:
    CEmptyArgsExpression() {}
    void accept(CVisitor* visitor) {
      visitor->visit(this);
    }
};

class CListExpressionNode: public CExpressionsNode {
public:
    CListExpressionNode(CExpressionNode* _prevExps, CExpressionNode* _nextExp): 
                                                            prevExps(_prevExps), nextExp(_nextExp) {}
    void accept(CVisitor* visitor) {
      visitor->visit(this);
    }
private:
    CExpressionNode* prevExps;
    CExpressionNode* nextExp;
};

class CLastListExpressionNode: public CExpressionsNode {
public:
    CLastListExpressionNode(CExpressionNode* _exp) : exp(_exp) {}
    void accept(CVisitor* visitor) {
      visitor->visit(this);
    }
private:
    CExpressionNode* exp;
};


