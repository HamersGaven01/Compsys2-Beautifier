/**************************************************
 * File: main.cpp                                 *
 * Authors: 63.7 % : S. Blythe, 36.3% : D. Poston *
 * Date: 02/2024                                  *
 * PURPOSE: driver file for project 1 and 2       *
 **************************************************/

/*
 ****** DEREK ********
 * open terminal in CLION - lower left hand corner, terminal symbol
 * navigate to cmake folder within project
 * use ./<filename> (should be 'Solution' in this instance), i.e : ./Solution
 * then ' ' (space)
 * file name to test (provided is test.myl)
 * i.e : ./Solution test.myl
 * */


/**********
 **********  DO NOT MODIFY THIS FILE!!!!! (unless it's project 2, which it is for this version)
 **********/

#include <iostream>
#include <fstream>

#include "Token.hpp"
#include "Grammar.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // check to make sure an input file was specified.
    if (argc!=2)
    {
        cerr << "USAGE: " << argv[0] << " <file>" << endl;
        return -1;
    }

    // open input file
    ifstream ifile(argv[1]);

    // if open was not successful, let user know.
    if (!ifile)
    {
        cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
        return -1;
    }


    Grammar runner;
    string output = runner.runGrammar(ifile);

    if(runner.isValid)
    {
        cout << "Program is VALID =========================" << endl;
        cout << output << endl;
        return 0;
    }
    cout << "Program is NOT VALID ======================" << endl;
    return -1;

}