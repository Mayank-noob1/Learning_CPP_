# Learning_CPP
This repo contains the work I am doing while learning C++.

## File description:
### 1. Calc.cpp:
   This file contains a Calculator in C++. It tokenize the input and perform operation on token based on the grammar mentioned. You can add more function definition, constants at the appropriate phase of evaluating. Precedence is also mentioned in grammar. For now it supports constants : e, pi and operators : +, -, *, /, !, ^, sqrt, log, sin, cos, tan, cot, cosec, sec.
 
   #### Grammar:
    BLOCK := EXPR ;
    EXPR := TERM + TERM | TERM - TERM | TERM 
    TERM := PREPRIMARY / PREPRIMARY | PREPRIMARY * PREPRIMARY  | PREPRIMARY % PREPRIMARY | PREPRIMARY
    PREPRIMARY = PRIMARY ^ PREPRIMARY | PRIMARY !
    PRIMARY := (EXPR) | FUN PRIMARY | NUM | -PRIMARY | CONSTANT | +PRIMARY
    NUM := DIGIT NUM | DIGIT
    FUN := SIN | COS | TAN | COT | COSEC | SEC | SQRT | LOG
    DIGIT := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    CONSTANT := E | PI
   #### How to run:
   Just run the file as you run any .cpp file on your favourite code editor. When you run the file it will prompt "Enter:" with newline, then you can write the expression you want to be evaluated with ";" character in end as end of expression token.
  #### Pre-requisite:
  1) How to write and interpret a grammar.
  2) How to tokenize a stream.
