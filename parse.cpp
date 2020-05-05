/*
 * parse.cpp
 */
#include<iostream>
#include "parse.h"
//#include "pt.h"
#include "lex.h"

namespace Parser {
bool pushed_back = false;
Tok	pushed_token;

static Tok GetNextToken(istream& in, int& line) {
	if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}


static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line)
{
	Pt *sl = Sl(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;

	return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line)
{
   // Tok t = Parser::GetNextToken(in, line);
    //cout<<"Token: "<<Parser::GetNextToken(in, line)<<endl;
	Pt *s = Stmt(in, line);
	if( s == 0 )
		return 0;

	if( Parser::GetNextToken(in, line) != SC ) {
		ParseError(line, "Missing semicolon");
		delete s;
		return 0;
	}

	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {
	// FIXME add code
    Pt *s=0;
    Tok t = Parser::GetNextToken(in, line);
    switch( t.GetToken() ) {
    case PRINT:
        s = PrintStmt(in, line);
        break;
    case PRINTLN:
        s = PrintlnStmt(in, line);
        break;
    case REPEAT:
        s = RepeatStmt(in, line);
        break;
    case IDENT:
        Parser::PushBackToken(t);
        s = Expr(in, line);
        break;

    case DONE:
        break;
    default:
        Parser::GetNextToken(in, line);
        break;
            
        
    }
    return s;    
}

Pt *PrintStmt(istream& in, int& line)
{
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Print(l, ex);
}

Pt *Expr(istream& in, int& line) 
{
// FIXME add code
   //Pt *t1 = Prod(in, line);
    Pt *t1 = Sum(in,line);
	if( t1 == 0 )
    {
		return 0;
	}
    //if tok=0 then looks to see if it equals plus or minus
	while ( true )
    {
		Tok t = Parser::GetNextToken(in, line);
		if( t == EQ )
        {
            Pt *t2 = Sum(in, line);
           // bool isEq() const { return true; }
			if( t2 == 0 ) 
        	{
				ParseError(line, "Missing expression after operator");
				return 0;
			}
            else
               return new Assign(t.GetLinenum(), t1, t2);			
		}
        else
            Parser::PushBackToken(t);
			return t1;		
	}
        
}

Pt *Prod(istream& in, int& line)
{
// FIXME add code
   Pt *tProd = Primary(in, line);

	if( tProd == 0 )
    {
		return 0;
	}
    //Checks to see eq star/slash
    while ( true ) 
    {
		Tok t = Parser::GetNextToken(in, line);


		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return tProd;
		}
//checks to if missing ops
		Pt *tProd2 = Prod(in, line);
		if( tProd2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}
//checks to see eq slash or star
		if( t == SLASH )
			tProd = new DivideExpr(t.GetLinenum(), tProd, tProd2);
		else
			tProd = new TimesExpr(t.GetLinenum(), tProd, tProd2);
	} 
}



Pt *PrintlnStmt(istream& in, int& line) {
// FIXME add code
    int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Println(l, ex);

}

Pt *RepeatStmt(istream& in, int& line) 
{
// FIXME add code
    
    Pt *e = Expr(in, line);
    if(e == 0)
    {
        ParseError(line, "Missing expression after loop");
        return 0;
    }
    
    Tok t1 = Parser::GetNextToken(in, line);
    if(t1 != BEGIN)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
    
    //only executes if expr !=0
    Pt *s = Sl(in, line);
    
    if(s ==0)
    {
        ParseError(line, "Missing statement list after BEGIN");
        return 0;
    }
    
   Tok t2 = Parser::GetNextToken(in, line);
    if(t2 != END){
        ParseError(line, "Missing END after statement THIS ONE");
        return 0;
    }
   
    
    return new Repeat(line, e, s);
}


/*
Pt *Expr(istream& in, int& line) {
// FIXME add code

}
*/
Pt *Sum(istream& in, int& line)
{
	Pt *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true )
    {
		Tok t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
   // return t1;
}
/*
Pt *Prod(istream& in, int& line) {
// FIXME add code
}*/

Pt *Primary(istream& in, int& line) 
{
	Tok t = Parser::GetNextToken(in, line);

	if( t == IDENT ) 
    {
		return new Ident(t);
	}
    // FIXME add code
	else if( t == ICONST )
    {
        return new IConst(t);
    }
    else if( t== SCONST )
    {
        return new SConst(t);
    }
    else if( t == LPAREN )
    {
        Pt *ex = Expr(in, line);
      
        
        //CHecks to see () are missing
        if( ex == 0 )
        {
            ParseError(line, "Missing expression after (");
            return 0;
        }
        if( Parser::GetNextToken(in, line) == RPAREN )
            return ex;
 
        ParseError(line, "Missing ) after expression");
        return 0;
    }
        
	 ParseError(line, "Primary expected");
    return 0;	

}


