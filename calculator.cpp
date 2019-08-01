
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
    unsigned long int i = current_index;
    
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
    if(nextToken()!= toConsume){
        mismatchError(line, toConsume, nextToken());
    }
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

bool Parser::isInFollow(string symbol, Token token){
    bool result;

    if(symbol == "start" || symbol == "list" || symbol == "list_prime"){
        result = ( token == T_EOF);
    }else if(symbol == "expression" || symbol == "expression_prime"){
        result = ( token == T_EOF || token == T_SEMICOLON || token == T_CLOSEPAREN);
    }else if(symbol == "term" || symbol == "term_prime"){
        result = (token == T_EOF || token == T_SEMICOLON || token ==T_CLOSEPAREN || token == T_PLUS || token == T_MINUS);
    }else if(symbol == "factor"){
        result = (token == T_EOF || token == T_SEMICOLON || token ==T_CLOSEPAREN || token == T_PLUS || token == T_MINUS || token == T_MULTIPLY || token == T_DIVIDE || token == T_MODULO);
    }

    return result;
}

void Parser::eliminate_newline(){
    while(scanner.nextToken() == T_NEWLN){
        scanner.eatToken(T_NEWLN);
    }
}

void Parser::start() {
    eliminate_newline();
    list();
}

void Parser::list() {
    eliminate_newline();
    expression();
    list_prime();
}

void Parser::list_prime() {
    eliminate_newline();
    if(scanner.nextToken() == T_SEMICOLON){
        scanner.eatToken(T_SEMICOLON);
        expression();
        list_prime();
    }
    else if(isInFollow("list_prime", scanner.nextToken())){
        return;
    }
    else{
        parseError(scanner.lineNumber(), scanner.nextToken());
    }
}

void Parser::expression(){
    eliminate_newline();
    term();
    expression_prime();
}

void Parser::expression_prime(){
    eliminate_newline();
    if(scanner.nextToken() == T_PLUS){
        operator_stack.push(T_PLUS);
        scanner.eatToken(T_PLUS);
        term();
        expression_prime();
    }else if(scanner.nextToken() == T_MINUS){
        operator_stack.push(T_MINUS);
        scanner.eatToken(T_MINUS);
        term();
        expression_prime();
    }else if(isInFollow("expression_prime", scanner.nextToken())){
        return;
    }else{
        parseError(scanner.lineNumber(), scanner.nextToken());
    }
}

void Parser::term(){
    eliminate_newline();
    factor();
    term_prime();
}

void Parser::term_prime(){
    eliminate_newline();
    if(scanner.nextToken() == T_MULTIPLY){
        operator_stack.push(T_MULTIPLY);
        scanner.eatToken(T_MULTIPLY);
        factor();
        term_prime();
    }else if(scanner.nextToken() == T_DIVIDE){
        operator_stack.push(T_DIVIDE);
        scanner.eatToken(T_DIVIDE);
        factor();
        term_prime();
    }else if(scanner.nextToken() == T_MODULO){
        operator_stack.push(T_MODULO);
        scanner.eatToken(T_MODULO);
        factor();
        term_prime();
    }else if(isInFollow("term_prime", scanner.nextToken())){
        return;
    }else{
        parseError(scanner.lineNumber(), scanner.nextToken());
    }
}

void Parser::factor(){
    eliminate_newline();
    if(scanner.nextToken() == T_OPENPAREN){
        scanner.eatToken(T_OPENPAREN);
        expression();
        scanner.eatToken(T_CLOSEPAREN);
    }else if(scanner.nextToken() == T_NUMBER){
        num_stack.push(scanner.getNumberValue());
        scanner.eatToken(T_NUMBER);
    }else{
        parseError(scanner.lineNumber(), scanner.nextToken());
    }
}

void Parser::calculate(){
    int a, b, result;
    Token token;
    while(!num_stack.empty() && !operator_stack.empty()){
        a = num_stack.top();
        num_stack.pop();
        b = num_stack.top();
        num_stack.pop();
        token = operator_stack.top();
        operator_stack.pop();
        
        switch (token)
        {
            case T_PLUS:
            case T_MINUS:
            case T_MULTIPLY:
            case T_DIVIDE:
            case T_MODULO:

        }
        
    }
}


