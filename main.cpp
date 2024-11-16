#include <iostream>
#include <memory>
#include <string>
#include <cctype>

// Токени для Flex
enum Token {
    TOK_NUM = 256,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULT,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN
};

// Лексичний аналізатор
int yylex() {
    int ch;
    while ((ch = getchar()) == ' ');  // Пропуск пробілів

    if (isdigit(ch)) {
        yylval = ch - '0';
        return TOK_NUM;
    }

    switch (ch) {
        case '+': return TOK_PLUS;
        case '-': return TOK_MINUS;
        case '*': return TOK_MULT;
        case '/': return TOK_DIV;
        case '(': return TOK_LPAREN;
        case ')': return TOK_RPAREN;
        case EOF: return 0;
        default: return ch;
    }
}

// Структури AST
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual int evaluate() const = 0;
};

struct NumberNode : ASTNode {
    int value;
    NumberNode(int v) : value(v) {}
    int evaluate() const override { return value; }
};

struct BinaryOpNode : ASTNode {
    std::unique_ptr<ASTNode> left, right;
    char op;
    BinaryOpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r, char o)
        : left(std::move(l)), right(std::move(r)), op(o) {}
    int evaluate() const override {
        int lval = left->evaluate();
        int rval = right->evaluate();
        switch (op) {
            case '+': return lval + rval;
            case '-': return lval - rval;
            case '*': return lval * rval;
            case '/': return rval != 0 ? lval / rval : 0;
            default: return 0;
        }
    }
};

// Прототипи для Bison
extern int yylex();
extern int yyparse();
extern std::unique_ptr<ASTNode> root;

// Змінна для AST
std::unique_ptr<ASTNode> root;

// Парсер (визначення граматики)
int yyparse() {
    int token = yylex();
    if (token == TOK_NUM) {
        root = std::make_unique<NumberNode>(yylval);
    } else if (token == TOK_LPAREN) {
        yyparse();
        int op = yylex();
        yyparse();
        if (yylex() == TOK_RPAREN) {
            root = std::make_unique<BinaryOpNode>(std::move(root), std::make_unique<NumberNode>(yylval), op);
        }
    }
    return 0;
}

// Головна функція
int main() {
    std::cout << "Введіть арифметичний вираз: ";
    if (yyparse() == 0 && root) {
        std::cout << "Результат: " << root->evaluate() << std::endl;
    } else {
        std::cerr << "Помилка розбору виразу" << std::endl;
    }
    return 0;
}
