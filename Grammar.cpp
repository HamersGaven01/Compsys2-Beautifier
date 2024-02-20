//
// Created by derek on 1/30/24.
//

#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;

#define NEWLINE '\n'
#define TAB "\t"

bool beginLogic = true;

int amntIndent = 0;
string indents = "  ";

void doIndent()
{
    for(int i = 0; i < amntIndent; i ++)
    {
        indents += "  ";
        //return indents;
    }
}

string Grammar::runGrammar(std::ifstream &is)
{
    return program(is);
}

string Grammar::program (ifstream &is)
{
    //declaration program | compound
    int pos = is.tellg();
    Token tok;
    tok.get(is);
    //cout << tok << endl;
    if (tok.type() != BEGIN)
    {
        is.seekg(pos);
        string lhs = declaration(is);
        return lhs + program(is);
    }
    else
    {
        is.seekg(pos);
        return compound(is);
    }
}

string Grammar::factor (std::ifstream &is)
{
    //ID | ID LPAREN exprlist RPAREN |
    //NUM REAL | NUM INT |
    //LPAREN expr RPAREN
    Token tok;
    tok.get(is);
    string lhs = tok.value();
    int pos = is.tellg();
    if (tok.type() == ID)
    {
        tok.get(is);
        if(tok.type() == LPAREN)
        {
            string parv = exprlist(is);
            string lpar = tok.value();
            tok.get(is);
            if(tok.type() != RPAREN)
            {
                cerr << "ERROR, expected ')' , but got : " << tok <<endl;
                isValid = false;
            }
            return lhs + lpar + parv + tok.value() ;
        }
        is.seekg(pos);
        return lhs;
    }
    else if(tok.type() == NUM_INT || tok.type() == NUM_REAL)
    {
        return tok.value();
    }
    else if(tok.type() == LPAREN)
    {
        string parv = expr(is);
        string lpar = tok.value();
        tok.get(is);
        if(tok.type() != RPAREN)
        {
            cerr << "ERROR, expected ')' , but got : " << tok <<endl;
            isValid = false;
        }
        return lpar + parv + tok.value() ;
    }
    cerr << "NOTHING SHOULD BE HERE" << endl;
    exit(-1);
}

string Grammar::declaration(std::ifstream &is)
{
    string tokType = type(is);
    string vars;
    list<string> myList = idlist(is);
    for (auto v : myList)
    {
        vars += v;
    }
    Token tok;
    tok.get(is);
    if (tok.type() != SEMICOLON)
    {
        cerr << "INCORRECT! We expected ';' but got : " << tok << endl;
        isValid = false;

    }
    return tokType + vars + tok.value() + NEWLINE;
}

string Grammar::compound(ifstream &is)
{
    //BEGIN stmtlist END
    Token tok;
    tok.get(is);
    //cout << tok << endl;
    string lhs = tok.value();

    if(beginLogic)
    {
        beginLogic = false;
        lhs = NEWLINE + tok.value();
    }

    if (tok.type() == BEGIN)
    {
        string inVal = stmtlist(is);
        tok.get(is);
        if (tok.type() != END)
        {
            cerr << " Expected 'END' , but got : " << tok << endl;
            isValid = false;
            //exit(-1);
        }

        //string indentation = doIndent();
        string builder = indents + lhs + NEWLINE + inVal + NEWLINE + indents + tok.value();
        doIndent();
        amntIndent ++;


        return builder;
        //return lhs + NEWLINE + inVal + NEWLINE + tok.value();

    }
    else
    {
        cerr << "Expected 'BEGIN' , but got : " << tok << endl;
        isValid = false;
        //exit(-1);
    }
}

string Grammar::stmtlist(ifstream &is)
{
    //stmt | stmt SEMICOLON stmtlist
    string lhs = stmt(is);
    int pos = is.tellg();
    Token tok;
    tok.get(is);

    if(tok.type() == SEMICOLON)
    {
        return lhs + tok.value()+ NEWLINE + stmtlist(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }

}

string Grammar::stmt (ifstream &is)
{
    //ID | ID LPAREN exprlist RPAREN |
    //ID ASSIGNOP expr |
    //IF expr THEN compound ELSE compound |
    //WHILE LPAREN expr RPAREN compound |
    //compound
    Token tok;
    tok.get(is);
    string lhs = tok.value();
    int pos = is.tellg();

    if (tok.type() == ID)
    {
        pos = is.tellg();
        tok.get(is);
        if(tok.type() == LPAREN)
        {
            string lpar = tok.value();
            string inPar = exprlist(is);
            tok.get(is);
            if(tok.type() != RPAREN)
            {
                cerr << "ERROR:: expected ')' , but got : " << tok <<endl;
                isValid = false;
            }
            return lhs + lpar + inPar + tok.value()  ;
        }
        else if (tok.type() == ASSIGNOP)
        {
            return lhs + tok.value() + expr(is);
        }
        is.seekg(pos);
        return lhs;
    }
    else if (tok.type() == IF)
    {
        string tokIf = tok.value();
        string condIf = expr(is);
        tok.get(is);
        if(tok.type() != THEN)
        {
            cerr << "OOPS : 'THEN' expected, but got : " << tok << endl;
            isValid = false;
        }
        string tokThen = tok.value();
        string condThen = compound(is);
        tok.get(is);
        if(tok.type() != ELSE)
        {
            cerr << "OOF : 'ELSE' expected, but got : " << tok << endl;
            isValid = false;
        }
        string tokElse = tok.value();
        string condElse = compound(is);

        return  tokIf + condIf +
                NEWLINE +
                tokThen +
                NEWLINE +
                condThen +
                NEWLINE +
                tokElse +
                NEWLINE +
                condElse ;
    }
    else if (tok.type() == WHILE)
    {
        //WHILE LPAREN expr RPAREN compound |

        tok.get(is); // LPAREN
        string lpar = tok.value();
        if(tok.type() != LPAREN)
        {
            cerr << "Needed '(' , we received : " << tok << endl;
            isValid = false;
        }
        string inPar = expr(is);
        
        tok.get(is);
        if(tok.type() != RPAREN)
        {
            cerr << "ERROR! expected ')' , but got : " << tok <<endl;
            isValid = false;
        }
        doIndent();
        string builder = indents + lhs + lpar + inPar + tok.value() + NEWLINE + compound(is) ;

        return builder;
    }
    else
    {
        is.seekg(pos);
        return compound(is);
    }
}

list<string> Grammar::idlist (std::ifstream &is)
{
    Token tok;
    tok.get(is);
    //cout << tok << endl;
    string lhs = tok.value();



    int pos = is.tellg();

    if(tok.type() != ID)
    {
        cerr << "type expected : ID .... Type received : " << tok << endl;
        isValid = false;
        //exit(-1);
    }
    tok.get(is);
    if(tok.type() == COMMA)
    {
        list<string> sublist = idlist(is);
        sublist.push_front(",");
        sublist.push_front(lhs);
        return sublist;

    }
    else
    {

        is.seekg(pos);
        list<string> lastItem;
        lastItem.push_front(lhs);
        return lastItem;
    }
}

string Grammar::type (ifstream &is)
{
    Token tok;
    tok.get(is);
    if(tok.type() == INTEGER)
    {
        return "int ";
    }
    else if (tok.type() == FLOAT)
    {
        return "float ";
    }
    else if (tok.type() == VOID)
    {
        return "void ";
    }
    else
    {
        return "";
    }
}

string Grammar::term (std::ifstream &is)
{
    string lhs = factor(is);
    int pos = is.tellg();

    Token tok;
    tok.get(is);

    if(tok.type() == MULOP)
    {
        return lhs + tok.value() + term(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }

}

string Grammar::exprlist(ifstream &is)
{
    //expr | expr COMMA exprlist
    string lhs = expr(is);
    int pos = is.tellg();

    Token tok;
    tok.get(is);

    if (tok.type() == COMMA)
    {
        return lhs + tok.value() + exprlist(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }
}

string Grammar::simpexpr(ifstream &is)
{
    //term | term ADDOP simpexpr
    string lhs = term(is);
    int pos = is.tellg();

    Token tok;
    tok.get(is);

    if (tok.type() == ADDOP)
    {
        return lhs + tok.value() + simpexpr(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }

}

string Grammar::expr(ifstream &is)
{
    //simpexpr | simpexpr RELOP simpexpr
    string lhs = simpexpr(is);
    int pos = is.tellg();

    Token tok;
    tok.get(is);
    if (tok.type() == RELOP)
    {
        return lhs + tok.value() + simpexpr(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }

}