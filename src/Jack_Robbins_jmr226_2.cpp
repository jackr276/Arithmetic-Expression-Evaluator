/**
 * Author: Jack Robbins, jmr226
 * CS 610, programming assignment 2, Recursive descent parser/interpreter
 */
#include <ios>
#include <iostream>
#include <sstream>
#include <string>





int evaluate(std::stringstream& in){
	char ch;
	while(in.get(ch)){
		std::cout << ch << std::endl;
	}
	return 0;
}

int main(void){
	//Grab the users input
	std::string input;
	std::cout << "\nEnter the arithmetic expression to evaluate: ";
	std::getline(std::cin, input);

	//We will use a stream to go character gy character
	std::stringstream in(input);

	return evaluate(in);
}
