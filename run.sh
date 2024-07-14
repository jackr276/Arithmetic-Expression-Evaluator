# Author: Jack Robbins
# This is a simple runner script for a C++ project

if [[ ! -d ./out ]]; then
	mkdir out
fi

# Clean out old binary files
rm -r out/*


g++  ./src/expression_evaluator.cpp -o ./out/expression_evaluator
./out/expression_evaluator
