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


