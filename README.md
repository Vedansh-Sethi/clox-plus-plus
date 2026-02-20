# Clox++

This is the C++ implementation of the JLox compiler made in "Crafting Interpreters" by Robert Nystrom

## Context Free Grammar (CFG) -

- Scanner scans the source code and groups the individual characters into meaningful tokens, but it is not enough to implement things like operator precedence
- Context free grammar has patterns which can contain themselves, which gives features like scope, operator precedence, make the language much stronger than with regular grammar
- CFG is an example of 'formal grammar', a formal grammar contains a collection of atomic pieces called "alphabet", which are arranged to give "strings" which are "in" the grammar.

| Element | Lexical Grammar | Syntactic Grammar |
| :--- | :--- | :--- |
| Alphabet | Characters | Tokens |
| String | Lexemes or token | Expressions |
| Implementation | Scanner | Parser |

### Rules for Grammar -

- We create rules that the grammar follows, which then creates the valid strings, the rules are called **productions**, and the strings created are called **derivations**
- Each production has a head and a body, head is the name of the production and body is a list of symbols, which can be terminal or non-terminal 

### Grammar for Lox -

- `expression` -> `literal` | `unary` | `binary` | `grouping` ;
- `literal` -> STRING | NUMBER | "nil" | "true" | "false" ;
- `grouping` -> "(" `expression` ")" ;
- `unary` -> ("-" | "!") `expression` ;
- `binary` -> `expression` `operator` `expression` ;
- `operator` -> "==" | "!=" | "<" | "<=" | ">" | ">=" | "+" | "-" | "*" | "/" ;

### Drawback of Current Grammar -

- The current grammar terminates into same class `expression`, this creates a barrier in implementing precedence and associativity
- This can be solved by defining separate rules for each level of precedence
- Each rule in grammar should contain either same precedence level or higher level expressions

### Updated Grammar with Precedence -

- `expression` -> `Comma Separated Expressions`
- `Comma Separated Expressions` -> `ternary` (("?") `ternary`)*;
-  `ternary` -> `equality` ( ("?") `ternary` (":") `ternary` );
- `equality` -> `comparison` (("!=" | "==") `comparison`)*;
- `comparison` -> `term` ((">" | ">=" | "<" | "<=") `term`)*;
- `term` -> `factor` (("+" | "-") `factor`) *;
- `factor` ->  `unary` (( "*" | "/") `unary`) *;
- `unary` -> (( "!" | "-" ) `unary`) | `primary`;
- `primary` -> NUMBER | STRING | "true" | "false" | "nil" | "(" `expression` ")";

### Basic Rules for statements -
- `program` -> `stmt`* EOF;
- `stmt` -> `exprStmt` | `printStmt`;
- `exprStmt` -> `expression` ";" ;
- `printStmt` -> "print" `expression` ";";