
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

// Scanner implementation

// You may have to modify this constructor, although it might not be neccessary.
Scanner::Scanner() : line(0), 
                     value(0) {
    current_index = 0;
    current_token_length = 1;
    input_string = "";
    char single_char = ' ';
    while(cin.get(single_char)){
        cout<<single_char;
        input_string.push_back(single_char);
    }
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
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

// You need to fill this method with the appropriate code for it to work as described in the project description.
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

// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval) : evaluate(eval) {
    // WRITEME
}

void Parser::parse() {
    start();
}

void Parser::start() {
    // I am a placeholder. Implement a recursive descent parser starting from me. Add other methods for different recursive steps.
    // Depending on how you designed your grammar, how many levels of operator precedence you need, etc., you might end up with more
    // or less steps in the process.
    //
    // WRITEME
}

// You will need to add more methods for this to work. Don't forget to also define them in calculator.hpp!
// WRITEME

