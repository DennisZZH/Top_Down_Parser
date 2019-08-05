
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
    while(input_string[current_index] == ' '){
        current_index++;
    }
    Token target = T_EOF;
    unsigned long int i = current_index;
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
    tokens.push_back(toConsume);
    if(toConsume == T_NUMBER){
        num_list.push_back(value);
    }
    current_index += current_token_length;
    if(toConsume == T_NEWLN) line++;
}

int Scanner::lineNumber() {
    return line;
}

long long int Scanner::getNumberValue() {
    return value;
}

// Parser implementation

Parser::Parser(bool eval) : evaluate(eval) {
   line = 1;
}

void Parser::parse() {
    start();
    if(evaluate){
        do_evaluate();
    }
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
        scanner.eatToken(T_PLUS);
        term();
        expression_prime();
    }else if(scanner.nextToken() == T_MINUS){
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
        scanner.eatToken(T_MULTIPLY);
        factor();
        term_prime();
    }else if(scanner.nextToken() == T_DIVIDE){
        scanner.eatToken(T_DIVIDE);
        factor();
        term_prime();
    }else if(scanner.nextToken() == T_MODULO){
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
        scanner.eatToken(T_NUMBER);
    }else if(scanner.nextToken() == T_EOF){
        return;
    }else{
        parseError(scanner.lineNumber(), scanner.nextToken());
    }
}

void Parser::calculate_two_num(){
    long long int result = 0;
    long long int a = num_stack.top();
    num_stack.pop();
    long long int b = num_stack.top();
    num_stack.pop();
    Token token = operator_stack.top();
    operator_stack.pop();
            
    switch (token)
    {
                case T_PLUS: result = b + a; break;
                case T_MINUS: result = b - a; break;
                case T_MULTIPLY: result = b * a; break;
                case T_DIVIDE:
                {
                    if(a == 0){
                        divideByZeroError(line, b);
                    }else{
                        result = b / a;
                    }
                    break;
                }
                case T_MODULO: result = b % a; break;
    }
    if(result > INT_MAX || result < INT_MIN){
        outOfBoundsError(line, result);
    }

    num_stack.push(result);
}

void Parser::do_evaluate(){
    for(auto i = scanner.tokens.begin(); i != scanner.tokens.end() ; i++) {
        Token tok = *i;
        if(tok == T_NUMBER){
            num_stack.push( *( scanner.num_list.begin() ) );
            scanner.num_list.pop_front();
        }else if(tok == T_PLUS || tok == T_MINUS || tok == T_MULTIPLY || tok == T_DIVIDE || tok == T_MODULO){
            while (!operator_stack.empty() && isPrecidence(tok, operator_stack.top())){                            
                calculate_two_num();                        
            }                                                                 
            operator_stack.push(tok); 
        }else if(tok == T_OPENPAREN){
            operator_stack.push(tok);
        }else if(tok == T_CLOSEPAREN){
            while (operator_stack.top() != T_OPENPAREN){
                calculate_two_num(); 
            }
            operator_stack.pop();  
        }else if(tok == T_SEMICOLON){
            while ( !operator_stack.empty() && num_stack.size() > 1 ){
                calculate_two_num(); 
            }
            if (!num_stack.empty()) {
                if(num_stack.top() > INT_MAX || num_stack.top() < INT_MIN){
                    outOfBoundsError(line, num_stack.top());
                }
                print_result += to_string(num_stack.top());
                print_result += "\n"; 
                num_stack.pop();
            } 
            while (!operator_stack.empty()) operator_stack.pop();     
        }else if(tok == T_NEWLN){
             line = line + 1;
        }
    }               
         
    while ( !operator_stack.empty() && num_stack.size() > 1 ){calculate_two_num(); }
        if (!num_stack.empty()) {
            if(num_stack.top() > INT_MAX || num_stack.top() < INT_MIN){
                outOfBoundsError(line, num_stack.top());
            }
            print_result += to_string(num_stack.top());   
        }

    cout<<print_result;
}

bool Parser::isPrecidence(Token a, Token b){
    if(b == T_OPENPAREN) return false;
    else if(b == T_MULTIPLY || b == T_DIVIDE || b == T_MODULO){
            return true;
    }
    return false;
}
