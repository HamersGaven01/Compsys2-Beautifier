//
// Created by derek on 1/30/24.
//

#ifndef SOLUTION_GRAMMAR_HPP
#define SOLUTION_GRAMMAR_HPP

#include <fstream>
#include <iostream>
#include <list>

using namespace std;
class Grammar{

private:
    string program(std::ifstream &);
    string declaration(std::ifstream &);
    list<string> idlist(std::ifstream &);
    string type(std::ifstream &);
    string compound(std::ifstream &);
    string stmtlist(std::ifstream &);
    string stmt(std::ifstream &);
    string exprlist(std::ifstream &);
    string expr(std::ifstream &);
    string simpexpr(std::ifstream &);
    string term(std::ifstream &);
    string factor(std::ifstream &);

public:
    string runGrammar(std::ifstream &);
    bool isValid = true;

};


#endif //SOLUTION_GRAMMAR_HPP
