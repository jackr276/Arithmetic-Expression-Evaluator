/**
 * Author: Jack Robbins, jmr226
 * CS 610, programming assignment 2, Recursive descent parser/interpreter
 */
#include <cstdlib>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>


/**
 * Peek at the next character in the stream
 */
char peek_next(std::stringstream& in){
	//If we are at the end, stop looking ahead
	if(in.eof()){
		return 0;
	}
	
	//Return the peeked at character without changing the stream
	return in.peek();
}


/**
 * Our seeker function. Returns a peek at the next non whitespace
 * character
 */
char seek(std::stringstream& in){
	//Fast forward with the whitespace
	in >> std::ws;
	return peek_next(in);
}


/**
 * Consume a token if we have what we expect. If we don't, return false
 * to let the caller know we do not have it
 */
bool consume_token(std::stringstream& in, char expected_token){
	//If we do not get what we are looking for, importantly, this
	// may or may not consitute an error
	if(seek(in) != expected_token){
		return false;
	}

	//If we have a match, consume the token from the stream
	in >> expected_token;
	return true;
}


/**
 * Simple helper function, tells us if we have a digit or not
 */
bool is_digit(char ch){
	return ch >= '0' && ch <= '9';
}

/**
 * BNF Rule: <literal>  ::=  0|1|2|3|4|5|6|7|8|9
 */
int literal(std::stringstream& in){
	//Consume the next token
	char literal = seek(in);
	
	if(is_digit(literal)){
		consume_token(in, literal);
		std::cout << "Literal: " << literal << std::endl;
		return literal - '0';
	}

	//Otherwise, we have a bad token
	std::cerr << literal << " is not a valid literal" << std::endl;
	std::cerr << "Invalid expression" << std::endl;
	exit(-1);
}


/**
 * For this rule, we can have 0 or many expressions, or a literal
 * BNF Rule: { <expression> } | <literal>
 */
int term(std::stringstream& in){
	//Declare function prototype
	int expression(std::stringstream& in);

	return literal(in);

	//Temporarily grab the next character
	char ch;
	in >> std::skipws >> ch;	

	//If we don't see a '*' or '/', we have a literal
	if(peek_next(in) != '*' && peek_next(in) != '/'){
		in.putback(ch);
		return literal(in);
	} else {
		return expression(in);
	}
}


/**
 * BNF Rule: <factor>  :==  <term> + <factor>  |  <term> - <factor>  |  <term>
 */
int factor(std::stringstream& in){
	int value = term(in);

	if(consume_token(in, '+')){
		value += factor(in);
		std::cout << "Operator: +" << std::endl;
	} else if (consume_token(in, '-')){
		std::cout << "Operator: -" << std::endl;
		value -= factor(in);
	}

	return value;
}


/**
 * BNF Rule: <expression>  ::=  <factor> * <expression>   |   <factor>  /  <expression>   |   <factor>
 */
int expression(std::stringstream& in){
	//grab the first factor's value
	int value = factor(in);

	if(consume_token(in, '*')){
		std::cout << "Operator: *" << std::endl;
		value *= expression(in);
	} else if (consume_token(in, '/')){
		//Grab the divisor
		int divisor = expression(in);

		//Runtime error checking
		if(divisor == 0){
			std::cerr << "Illegal divide by 0 operation" << std::endl;
			std::cerr << "Invalid expression" << std::endl;
			exit(-1);

		} else {
			std::cout << "Operator: /" << std::endl;
			value /= divisor; 
		}
	}

	return value;
}



int parse_interpret(std::stringstream& in){
	return expression(in);
}


/**
 * Entry point main function. Simply grabs input from the user and makes the 
 * appropriate calls. Command line arguments are not used
 */
int main(void){
	//Grab the users input
	std::string input;
	std::cout << "Enter the arithmetic expression to evaluate: ";
	std::getline(std::cin, input);

	//We will use a stream to go character gy character
	std::stringstream in(input);

	//Make a call to parse_interpret with the input stream
	int result = parse_interpret(in);

	std::cout << "Expression result: " << input << " = " << result << std::endl;



	return 0;
}
