/*******************************************
 * File: Token.cpp                         *
 * Author: Derek Poston                    *
 * Date: 1/2024                            *
 * PURPOSE: implementation for Token       *
 *******************************************/

#include "Token.hpp"

#include <fstream>

#include <iomanip>

using namespace std;

#define NUM_STATES 20
#define START 0
// made MYERROR -1 since ERROR is taken :(
#define MYERROR -1




// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found in the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };

static int **DFA = nullptr;


/******************************************************
 *  just prints out the info describing this Token    *
 *    to specified stream                             *
 *                                                    *
 *   os  - the stream to add the Token to             *
 *                                                    *
 *   returns: the updated stream                      *
 ******************************************************/
ostream&
Token::print(ostream& os) const
{
  os
     << "{ Type:"   << left << setw(10) << TokStr[_type] 
     << " Value:"   << left << setw(10) << _value
     << " Line Number:" << _line_num
     << " }";
  return os;
}

void initDFA()
{
    DFA = new int*[NUM_STATES];
    for(int state = START; state < NUM_STATES; state++)
    {
        DFA[state] = new int[256];
        for (int ch = 0; ch < 256; ch++)
        {
            DFA[state][ch] = MYERROR;
        }
    }

    // all number transitions from start state (NONE or 0) leading to second...
    for(char ch = '0'; ch<='9'; ch ++)
    {
        DFA[START][ (int) ch] = 2;
    }
    // all lowercase transitions from start state (NONE or 0)
    for(char ch = 'a'; ch<='z'; ch ++)
    {
        DFA[START][ (int) ch] = 1;
    }
    // all capital transitions from start state
    for(char ch = 'A'; ch<='Z'; ch ++)
    {
        DFA[START][ (int) ch] = 1;
    }

    // unfortunately have to hardcode these because the labels are taken... :(
    DFA[0][(int)'+'] = 5;
    DFA[0][(int)'-'] = 5;
    DFA[0][(int)'*'] = 6;
    DFA[0][(int)'/'] = 6;
    DFA[0][(int)'<'] = 7;
    DFA[0][(int)'>'] = 7;
    DFA[0][(int)'='] = 9;
    DFA[0][(int)'('] = 10;
    DFA[0][(int)')'] = 11;
    DFA[0][(int)'&'] = 12;
    DFA[0][(int)'|'] = 14;
    DFA[0][(int)';'] = 16;
    DFA[0][(int)'['] = 17;
    DFA[0][(int)']'] = 18;
    DFA[0][(int)','] = 19;

    // all number transitions from first state
    for(char ch = '0'; ch<='9'; ch ++)
    {
        DFA[1][ (int) ch] = 1;
    }
    // all lowercase transitions from first state
    for(char ch = 'a'; ch<='z'; ch ++)
    {
        DFA[1][ (int) ch] = 1;
    }
    // all capital transitions from first state
    for(char ch = 'A'; ch<='Z'; ch ++)
    {
        DFA[1][ (int) ch] = 1;
    }
    // all number transitions from second state (looping on itself)
    for(char ch = '0'; ch<='9'; ch ++)
    {
        DFA[2][ (int) ch] = 2;
    }
    DFA[2][(int)'.'] = 3;
    // all number transitions from third state (leading to 4th)
    for(char ch = '0'; ch<='9'; ch ++)
    {
        DFA[3][ (int) ch] = 4;
    }
    // all number transitions from fourth state (looping on itself)
    for(char ch = '0'; ch<='9'; ch ++)
    {
        DFA[4][ (int) ch] = 4;
    }
    // transition from 7 to 8
    DFA[7][(int)'='] = 8;
    // transition from 9 back to 8
    DFA[9][(int)'='] = 8;
    // transition from 12 to 13 (13 is a final state)
    DFA[12][(int)'&'] = 13;
    // final transition state, rest are final
    DFA[14][(int)'|'] = 15;

}

/******************************************************
 *  Fills in information about this Token by reading  *
 *    it from specified input stream                  *
 *                                                    *
 *   is  - the stream to read the Token from          *
 *                                                    *
 *   returns: nothing                                 *
 *                                                    *
 *     **** YOU MUST DID CODE THIS !!!!!! ****         *
 ******************************************************/
void Token::get(istream &is)
{
  // you must write this code !!!!
    if (!DFA)
    {
        initDFA();
    }

    _value = "";
    char ch;

    ch = is.get(); // get next char from input
    while (isspace(ch) || ch == '#') //   || isComment() ...
    {
        if (ch == '\n')
        {
            _line_num++;
        }
        if (ch == '#')
        {
            while (ch != '\n')
            {
                ch = is.get();
            }
            _line_num++;
        }
        ch = is.get();
        // # some characters
    }
    if (!is)
    {
        _type = EOF_TOK;
        return ;
    }
    is.putback(ch);

    // Start //

    int currentState = START;
    int previousState = MYERROR;

    while (currentState != MYERROR)
    {
        ch = is.get(); // input stream, get next char from input

        // move to next state based on character read
        previousState = currentState;
        currentState = DFA[currentState][(int) ch];

        if (currentState != MYERROR) // if character is a valid part of token...
        {
            _value += ch; // ...add char to lexeme's value
        }

    }

    currentState = previousState;

    // we read one extra character ... put it back for the next read()
    if (is)
    {
        is.putback(ch);
    }


    // each case for each type and the corresponding type
    // using switch fall-though for case 7 to match case 8

    switch(currentState)
    {
        case 1 : _type = ID; break;
        case 2 : _type = NUM_INT; break;
        case 4 : _type = NUM_REAL; break;
        case 5 : _type = ADDOP; break;
        case 6 : _type = MULOP; break;
        case 7 :
        case 8 : _type = RELOP; break;
        case 9 : _type = ASSIGNOP; break;
        case 10 : _type = LPAREN; break;
        case 11 : _type = RPAREN; break;
        case 13 : _type = AND; break;
        case 15 : _type = OR; break;
        case 16 : _type = SEMICOLON; break;
        case 17: _type = LBRACK; break;
        case 18: _type = RBRACK; break;
        case 19: _type = COMMA; break;
        default: _type = ERROR;
    }


    //this was a frustrating set of lines...
    // I couldn't get 249 to work because I had never used static cast before.. thanks google <3
    if (_type == ID)
    {
        for ( int i = 0; i < 9 ; i ++)
        {
            if (_value == reserved [i])
            {
                _type = static_cast<TokenType>(TokenType::INTEGER+i);
            }
        }
    }

}


