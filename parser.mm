%{
#include <iostream>
#include <memory>
#include "ast.h"

void yyerror(const char *s);
int yylex();
std::unique_ptr<ASTNode> root;
%}

%token NUMBER
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN

%left PLUS MINUS
%left MULT DIV

%%

expr:
      expr PLUS expr     { $$ = std::make_unique<BinaryOpNode>(std::move($1), std::move($3), '+'); }
    | expr MINUS expr    { $$ = std::make_unique<BinaryOpNode>(std::move($1), std::move($3), '-'); }
    | expr MULT expr     { $$ = std::make_unique<BinaryOpNode>(std::move($1), std::move($3), '*'); }
    | expr DIV expr      { $$ = std::make_unique<BinaryOpNode>(std::move($1), std::move($3), '/'); }
    | LPAREN expr RPAREN { $$ = std::move($2); }
    | NUMBER             { $$ = std::make_unique<NumberNode>($1); }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "Помилка: " << s << std::endl;
}
