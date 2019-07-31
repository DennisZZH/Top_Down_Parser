
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

// Scanner implementation

Scanner::Scanner() : line(1), 
                     value(0) {
    current_index = 0;
    current_token_length = 1;
    input_string = "";
    char single_char;
    while(cin.get(single_char)){
        input_string.push_back(single_char);
    }
}

Token Scanner::nextToken() {
    Token target = T_EOF;
    int i = current_index;
    
    while(input_string[i] == ' '){
        i++;
    }
    current_token_length = 1;
    if( i == input_string.size()){
        return target;
    }else if(input_string[i] == '+'){
        target = T_PLUS;
    }else if(input_string[i] == '-'){
        target = T_MINUS;
    }else if(input_string[i] == '*'){
        target = T_MULTIPLY;
    }else if(input_string[i] == '/'){
        target = T_DIVIDE;
    }else if(input_string[i] == '('){
        target = T_OPENPAREN;
    }else if(input_string[i] == ')'){
        target = T_CLOSEPAREN;
    }else if(input_string[i] == ';'){
        target = T_SEMICOLON;
    }else if(input_string[i] == 'm'){
        if(++i < input_string.size() && input_string[i] == 'o'){
            if(++i < input_string.size() && input_string[i] == 'd'){
                target = T_MODULO;
                current_token_length = 3;
            }else{
                scanError(line, input_string[i]);
            }
        }else{
            scanError(line, input_string[i]);
        }
    }else if(input_string[i] - '0' >= 0 && input_string[i] - '0' <= 9){
        string num = "";
        num.push_back(input_string[i]);
        while(++i < input_string.size() && input_string[i] - '0' >= 0 && input_string[i] - '0' <= 9){
            num.push_back(input_string[i]);
            current_token_length++;
        }
        target = T_NUMBER;
        value = stoi(num);
    }else if(input_string[i] == '\n'){
        target = T_NEWLN;
    }else{
        scanError(line, input_string[i]);
    }
    
    return target;
}

void Scanner::eatToken(Token toConsume) {
    current_index += current_token_length;
    if(toConsume == T_NEWLN) line++;
}

int Scanner::lineNumber() {
    return this->line;
}

int Scanner::getNumberValue() {
    return this->value;
}

// Parser implementation

Parser::Parser(bool eval) : evaluate(eval) {
   
}

void Parser::parse() {
    start();
}

void Parser::start() {
   list();
   break;
}

void Parser::list() {
    expression();
    list_prime();
    break;
}

void Parser::list_prime() {
    
}


