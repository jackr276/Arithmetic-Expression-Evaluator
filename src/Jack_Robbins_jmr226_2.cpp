/**
 * Author: Jack Robbins, jmr226
 * CS 610, programming assignment 2, Recursive descent parser/interpreter
 *
 * Implementation uses this BNF formula:
 * <expression> ::= <term> + <expression> | <term> - <expression> | <term>
 * <term> ::= <factor> * <term> | <factor> / <term> | <factor>
 * <factor> ::= ( <expression> ) | <operand>
 * <operand> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#define SPACES 5 

/**
 * Define a node for our parse tree. The parse tree will be built as we interpret, 
 * so we will build it as we are traversing it using recursive descent
 */
struct parse_tree_node{
	char token;
	parse_tree_node* lchild;
	parse_tree_node* rchild;
};


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
 * Operands are single digits
 * BNF Rule: <operand> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */
int operand(std::stringstream& in, struct parse_tree_node** curr_node){
	//Consume the next token
	char literal = seek(in);
	
	//Reserve space for the node
	*curr_node = (struct parse_tree_node*)malloc(sizeof(parse_tree_node));

	//If we get a digit, we know it went well
	if(is_digit(literal)){
		//Consume from the in stream
		consume_token(in, literal);

		//This should be a child on our parse tree
		(*curr_node)->token = literal;
		(*curr_node)->lchild = NULL;
		(*curr_node)->rchild = NULL;

		//Convert to a number
		return literal - '0';
	}

	//Otherwise, we have a bad token
	std::cerr << literal << " is not a valid literal" << std::endl;
	std::cerr << "Invalid expression" << std::endl;
	exit(-1);
}


/**
 * Our recrusive descent step, factor can be an expression in parenthesis or just an operand
 * BNF Rule: <factor>  ::=  ( <expression> ) | <operand>
 */
int factor(std::stringstream& in, struct parse_tree_node** curr_node){
	//Function prototype	
	int expression(std::stringstream& in, struct parse_tree_node** curr_node);
	int value;

	//If we see an open parenthesis, we know we have an expression
	if(consume_token(in, '(')) {
		//Evaluate the expression
		value = expression(in, curr_node);

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
	} else {
		//Otherwise we just have an operand
		value = operand(in, curr_node);	
	}

	return value;
}


/**
 * Our division and multiplication rule. This will always appear deeper
 * in the parse tree than addition and subtraction, meaning it has higher precedence
 * BNF Rule: <term> ::= <factor> * <term> | <factor> / <term> | <factor>
 */
int term(std::stringstream& in, struct parse_tree_node** curr_node){
	//Reserve space for the node
	*curr_node = (struct parse_tree_node*)malloc(sizeof(parse_tree_node));

	//We must see a valid factor first
	int value = factor(in, &((*curr_node)->lchild));

	//Multiplication case
	if(consume_token(in, '*')){
		(*curr_node)->token = '*';
		//Get the term on the RHS
		value *= term(in, &((*curr_node)->rchild));
	//Division case
	} else if (consume_token(in, '/')){
		//runtime error checking
		int divisor = term(in, &((*curr_node)->rchild));
		if(divisor == 0){
			std::cerr << "Arithmetic Error: divide by 0" << std::endl;
			exit(-1);
		}
		
		(*curr_node)->token = '/';
		value /= divisor;
	}

	return value;
}


/**
 * The entry point to our parse tree
 * BNF Rule: <expression>  ::=  <term> + <expression>   |   <term>  -  <expression>   | <term>
 */
int expression(std::stringstream& in, struct parse_tree_node** curr_node){
	//Reserve space for the node
	*curr_node = (struct parse_tree_node*)malloc(sizeof(parse_tree_node));

	//grab the first term's value
	int value = term(in, &((*curr_node)->lchild));

	//Addition case
	if(consume_token(in, '+')){
		(*curr_node)->token = '+';
		//Get the expression on the RHS
		value += expression(in, &((*curr_node)->rchild));
	//Subtraction case
	} else if (consume_token(in, '-')){
		(*curr_node)->token = '-';	
		//Get the expression on the RHS
		value -= expression(in, &((*curr_node)->rchild));
	}	

	return value;
}


/**
 * Entry point to the recursive-descent parser. We start with an expression and the internals
 * are parsed recursively
 */
int parse_interpret(std::stringstream& in, struct parse_tree_node** root){
	return expression(in, root);
}


void print_tree_vertical(struct parse_tree_node* root, int space){
	if(root == NULL){
		return;
	}

	print_tree_vertical(root->rchild, space);

	space += SPACES;

	for(int i = SPACES; i < space; i++){
		std::cout << " ";
	}

	std::cout << root->token;

	std::cout << std::endl;

	print_tree_vertical(root->lchild, space);
}


/**
 * Entry point main function. Simply grabs input from the user and makes the 
 * appropriate calls. Command line arguments are not used
 */
int main(void){
	//Entry message
	std::cout << "Welcome to the arithmetic expression evaluator" << std::endl;
	std::cout << "\n=====================================================================================================" << std::endl;
	std::cout << "Expressions must obey the following BNF grammar" << std::endl;
	std::cout << "* <expression>  ::= <term> + <expression>   |   <term>  -  <expression>   | <term>\n";
	std::cout << "* <term>        ::= <factor> * <term> | <factor> / <term> | <factor>\n";
	std::cout << "* <factor>      ::= ( <expression> ) | <operand>\n";
	std::cout << "* <operand>     ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9\n";
	std::cout << "=====================================================================================================\n" << std::endl;

	//Grab the users input
	std::string input;
	std::cout << "Enter the arithmetic expression to evaluate: ";
	std::getline(std::cin, input);

	//We will use a stream to go character by character
	std::stringstream in(input);

	struct parse_tree_node** root = (struct parse_tree_node**)malloc(sizeof(struct parse_tree_node*)); 

	//Make a call to parse_interpret with the input stream
	int result = parse_interpret(in, root);

	//Display result nicely
	std::cout << "\nExpression result: " << input << " = " << result << std::endl;

	//Display the expression tree
	std::cout << "\nExpression tree: " << std::endl;
	print_tree_vertical(*root, 0);
	std::cout << std::endl;

	return 0;
}
