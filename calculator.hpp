#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>

using namespace std;
// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    int value;
    string input_string;
    int current_token_length;
    int current_index;

public:
    Token nextToken();
    void eatToken(Token);
    int lineNumber();
    int getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    
    bool evaluate;
    void start();
    void list();
    void list_prime();
    void expression();
    void expression_prime();
    void term();
    void term_prime();
    void factor();

    void match(Token token);
    bool isInFollow(string symbol, Token token);
    void eliminate_newline();

public:
    void parse();
    
    Parser(bool);
};
