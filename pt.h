/*
 * pt.h
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

//#include "parse.h"
//#include "lex.h"

class Pt {
protected:
	int	linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }
    //Add two function: GetLeft, GetRight
    Pt *GetLeft() const {return left;}
    Pt *GetRight() const {return right;}

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
    
    int MaxDepth() const
    {
		int lc = 0;
        int rc = 0;
		if( left ) lc += left->MaxDepth();
		if( right ) rc += right->MaxDepth();
		/* Saying that the left side has to be bigger than the right*/
        if (lc > rc)
            return (lc + 1);
        else 
            return (rc + 1);
        return 0;
	}
    int plusCount()const
    {
        int plus=0;
        if( left ) plus += left->plusCount();
		if( right) plus += right->plusCount();
        if(isPlus())
        {
            plus++;
        }
        return plus;
    }
    int EqCount()const
    {
        int EQ=0;
        if( left ) EQ += left->EqCount();
		if( right) EQ += right->EqCount();
        if(isEq())
        {
            EQ++;
        }
        return EQ;
    }
   //std::cout<<"hhhhhhhhhh: "<<plus<<endl;
    

};
//cout<<"hhhhhhhhhh: "<<plus<<endl;
class StmtList : public Pt {
public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
};

class Println : public Pt {
// FIXME add code
public:
	Println(int line, Pt *e) : Pt(line, e) {}
};

class Repeat : public Pt {
// FIXME add code
    public:
	Repeat(int line, Pt *e,Pt *stmt) : Pt(line, e,stmt) {}

};

class Assign : public Pt {
 // FIXME add code
    public:
	Assign(int line, Pt *lhs, Pt *rhs) : Pt(line, lhs, rhs) {}
	virtual bool isEq() const { return true; }
};

class PlusExpr : public Pt {
public:
// FIXME add code
    PlusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
   virtual bool isPlus() const { return true; }
    

};

class MinusExpr : public Pt {
// FIXME add code
    public:
    MinusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    //

};

class TimesExpr : public Pt {
// FIXME add code
    public:
	TimesExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}

};

class DivideExpr : public Pt {
// FIXME add code
    public:
	DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}

};

class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
};

class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    //virtual bool isEq() const { return false; }
};

class Ident : public Pt {
// FIXME add code
    string id;

public:
	Ident(Tok& t) : Pt(t.GetLinenum()), id(t.GetLexeme()) {}

	 virtual bool IsIdent() const { return true; }
   
};

#endif /* PT_H_ */

