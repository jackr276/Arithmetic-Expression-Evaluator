/**
 * Author: Jack Robbins, jmr226
 * CS 610, programming assignment 2, Recursive descent parser/interpreter
 *
 * Here is the BNF for the context-free grammar of the expressions
 *
 * <expression>  ::= <term> {(+ | -) <term>} 
 * <term>  		 ::= <factor> {(* | /) <factor>} 
 * <factor>      ::= ( <expression> ) | <literal>
 * <literal>     ::=  0|1|2|3|4|5|6|7|8|9
 */

#include <cstdlib>
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
		return literal - '0';
	}

	//Otherwise, we have a bad token
	std::cerr << literal << " is not a valid literal" << std::endl;
	std::cerr << "Invalid expression" << std::endl;
	//Hard exit
	exit(1);
}


/**
 * For this rule, we can have 0 or many expressions, or a literal
 * This is the implicit recursion in recursive descent, we can go back up and have more
 * expressions
 * BNF Rule: <factor> ::= ( <expression> ) | <literal>
 */
int factor(std::stringstream& in){
	//Declare function prototype
	int expression(std::stringstream& in);

	//If we see an open parenthesis, we know we have an expression
	if(consume_token(in, '(')) {
		//Evaluate the expression
		int value = expression(in);

		//If we can consume an rparen, everything went well
		if(consume_token(in, ')')){
			return value;
		} else {
			//Otherwise, we have unmatched parenthesis
			std::cerr << "Syntax Error: Unmatched parenthesis" << std::endl;
			std::cerr << "Invalid expression" << std::endl;
			//Hard exit
			exit(1);
		}

	//If we get here, we know it was just a literal
	} else {
		return literal(in);
	}
}


/**
 * BNF Rule: <term>  :==  <factor> {(* | /) <factor>} 
 */
int term(std::stringstream& in){
	//We must see a valid factor first
	int value = factor(in);

	//While we keep seeing * or /, keep multiplying/dividing the next factor
	while(seek(in) == '*' || seek(in) == '/'){
		//multiplication case
		if(consume_token(in, '*')){
			value *= factor(in);
		//Division case, we could have a tripping point here
		} else if(consume_token(in, '/')){
			//grab the next factor
			int f = factor(in);

			//Avoid having a divide by 0 runtime error
			if(f == 0){
				std::cerr << "Error: divide by 0 error" << std::endl;
				std::cerr << "Invalid expression" << std::endl;
				exit(1);
			}
			
			//If it wasn't 0, we can divide just fine
			value /= f;
		}
	}

	return value;
}


/**
 * Expression evaluation. This is the entry point to our program, and it is
 * also the term where we could have some indirect recursion
 * BNF Rule: <expression>  ::= <term> {(+ | -) <term>} 
 */
int expression(std::stringstream& in){
	//We must see a valid term first
	int value = term(in);

	//While we keep seeing + or -, keep adding/subtrating the next term
	while(seek(in) == '+' || seek(in) == '-'){
		//addition case
		if(consume_token(in, '+')){
			value += term(in);
		//Subtraction case
		} else if (consume_token(in, '-')){
			value -= term(in);
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
