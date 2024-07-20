# Arithmetic Expression Evaluator
Author: [Jack Robbins](https://www.github.com/jackr276)

At a high level, this project evaluates simple arithmetic expressions mostly correctly, with a few exceptions. It was done as a simple learning exercise, and should not be taken as any sort of calculator.

## A Context-Free Language of arithmetic expressions
This project implements a recursive descent parser/interpreter for a simple arithmetic expression context-free grammar. Context-free grammars are a way of describing context free languages(CFLs), a topic that is too broad to cover entirely here. If you would like to know more about the ins and outs of context free grammars, I have a separate project [here](https://github.com/jackr276/Context-Free-Language-Recognition-with-a-PDA) that is all about them. For our purposes here, the grammar implemented is:
```
 1. <expression> ::= <term> + <expression> | <term> - <expression> | <term>
 2. <term> ::= <factor> * <term> | <factor> / <term> | <factor>
 3. <factor> ::= ( <expression> ) | <operand>
 4. <operand> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

Some important notes about this grammar:
1. It has direct right recursion. This is not usually done for grammars of this nature. Recursion is usually implicit to allow for associativity(more on this later), but for the fun of it I decided to implement this grammar with these rules instead
2. PEMDAS is followed(mostly). Becase the '+' and '-' operations appear aboce the '*' and '/' operations, they have a lower precedence and will be executed after multiplication and division. However, some astute readers may notice that this grammar does not follow the "left-to-right" associativity rules from PEMDAS, due to the direct right recursion. This is true, and in fact expressions like `8 / 2 * 4` will evaluate to 1 because `2 *  4` will be evaluated and expanded first, as opposed to `8 / 2`. This is just a "feature" of the direct right recursion that we have here.
3. Grouping is allowed and is followed exactly. The parenthesis in rule 3 allow for the `<expression>` in them to be evaluated first. This will be obeyed by the grammar.
4. Operands may only be single digit. Because of how simple this grammar is, we cannot have any more than 1 digit in our operands. This does mean that a number like `11` would confuse the interpreter and cause an error.

## Sample derivation
For an illustration of how this grammar works, here is an example derivation of the expression `(8 + 1) * (9 - 3)`:
1. \<expression\> 
2. \<factor\> * \<term\>
3. \<factor\> * \<factor\>
4. ( \<expression\> ) * ( \<expression\> )
5. ( \<term\> + \<expression\> ) * ( \<term\> - \<expression\> )
6. ( \<term\> + \<term\> ) * ( \<term\> - \<term\> )
7. ( \<factor\> + \<factor\> ) * ( \<factor\> - \<factor\> )
8. ( \<operand\> + \<operand\> ) * ( \<operand\> - \<operand\> )
9. ( 8 + 1 ) * ( 9 - 3 )

It is important to note that derivations of a context free grammar are never deterministic. In other words, there is always more than one way to derive a given string from a context free grammar, and the way shown above is just one example.

## Programmatic implementation
The programmatic implementation for this context-free grammar can be found [here](https://github.com/jackr276/Arithmetic-Expression-Evaluator/blob/main/src/expression_evaluator.cpp). It is written in C++, which I chose mainly because I like the convenience of the `stringstream` module. The program itself is very well documented, so I will not go over it here entirely, but I will mention how it is very simple to implement context free grammars like this with a few simple translation rules:
1. Left hand side rules become function calls
2. Vertical bars `|` become if statements in the code
3. Not exemplified here, but optional repeat brackets `{}` become while loops
With these 3 rules, it is pretty easy to implement recursive descent on any grammar like this. I will let you look at the code to see its simplicity firsthand.

## Running this program
A simple [runner script](https://github.com/jackr276/Arithmetic-Expression-Evaluator/blob/main/run.sh) is provided for convenienve of compiling and running this program. To start, download the code from this repo and navigate to where it is stored on your machine. Then run the following commands:
```console
example@bash: ~/Arithmetic-Expression-Evaluator $ chmod +x run.sh
example@bash: ~/Arithmetic-Expression-Evaluator $ ./run.sh
Welcome to the arithmetic expression evaluator

=====================================================================================================
Expressions must obey the following BNF grammar
* <expression>  ::= <term> + <expression>   |   <term>  -  <expression>   | <term>
* <term>        ::= <factor> * <term> | <factor> / <term> | <factor>
* <factor>      ::= ( <expression> ) | <operand>
* <operand>     ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
=====================================================================================================

Enter the arithmetic expression to evaluate:
```
Upon entering an expression, the program will parse and interpret the expression and print out the answer and a representation of the parse tree, like this:
```console
Enter the arithmetic expression to evaluate: (8 + 1) * (9 - 3)

Expression result: (8 + 1) * (9 - 3) = 54

Expression tree: 

     
          
               
                    3
          -
               
                    9
     *
          
               
                    1
          +
               
                    8

example@bash: ~/Arithmetic-Expression-Evaluator $
```

