#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>
#include <stack>
#include <list>

using namespace std;
// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    long value;
    int current_token_length;
    int current_index;

public:
    string input_string;
    vector<Token> tokens;
    list<long> num_list;

    Token nextToken();
    void eatToken(Token);
    int lineNumber();
    long getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    stack<Token> operator_stack;
    stack<long> num_stack;
    string print_result;
    int line;

    bool evaluate;
    void start();
    void list();
    void list_prime();
    void expression();
    void expression_prime();
    void term();
    void term_prime();
    void factor();

    bool isInFollow(string symbol, Token token);
    void eliminate_newline();

    void do_evaluate();
    void calculate_two_num();
    bool isPrecidence(Token a, Token b);

public:
    void parse();
    Parser(bool);
};
