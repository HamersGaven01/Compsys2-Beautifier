//
// Created by derek on 1/30/24.
//

#include "Grammar.hpp"
#include "Token.hpp"

using namespace std;


string factor (std::ifstream &is)
{
    //ID | ID LPAREN exprlist RPAREN |
    //NUM REAL | NUM INT |
    //LPAREN expr RPAREN
    Token tok;
    tok.get(is);
    int pos = is.tellg();
    if (tok.type() == ID)
    {
        tok.get(is);
        if(tok.type() == LPAREN)
        {
            return "(" + exprlist(is) + ")" ;
        }
        is.seekg(pos);
        return tok.value();
    }
    else if(tok.type() == NUM_INT || tok.type() == NUM_REAL)
    {
        return tok.value();
    }
    else if(tok.type() == LPAREN)
    {
        return "(" + expr(is) + ")" ;
    }
}

string declaration(std::ifstream &is)
{
    //type idlist SEMICOLON
    string tokType = type(is);
    int pos = is.tellg();
    //Token tok;
    //tok.get(is);

    string vars;

    list<string> myList = idlist(is);
    for (auto v : myList)
    {
        vars += v;
    }

    return tokType + vars + ";" ;
}

string compound(ifstream &is)
{
    //BEGIN stmtlist END
    return "BEGIN" + stmtlist(is) + "END" ;
}

string stmtlist(ifstream &is)
{
    //stmt | stmt SEMICOLON stmtlist
    string lhs = stmt(is);
    int pos = is.tellg();

    Token tok;
    tok.get(is);

    if(tok.type() == SEMICOLON)
    {
        return lhs + tok.value() + stmtlist(is);
    }
    else
    {
        is.seekg(pos);
        return lhs;
    }

}

string stmt (ifstream &is)
{
    //ID | ID LPAREN exprlist RPAREN |
    //ID ASSIGNOP expr |
    //IF expr THEN compound ELSE compound |
    //WHILE LPAREN expr RPAREN compound |
    //compound

    
}

list<string> idlist (std::ifstream &is)
{
    Token tok;
    tok.get(is);
    string lhs = tok.value();



    int pos = is.tellg();

    if(tok.type() != ID)
    {
        cerr << "type expected : ID .... Type received : " << tok << endl;
        exit(-1);
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

string type (ifstream &is)
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

string term (std::ifstream &is)
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

string exprlist(ifstream &is)
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

string simpexpr(ifstream &is)
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

string expr(ifstream &is)
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