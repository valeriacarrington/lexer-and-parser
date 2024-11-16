// ast.h

#ifndef AST_H
#define AST_H

#include <memory>
#include <iostream>
#include <stdexcept>

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual int evaluate() const = 0;
};

struct NumberNode : ASTNode {
    int value;
    explicit NumberNode(int v) : value(v) {}
    int evaluate() const override { return value; }
};

struct BinaryOpNode : ASTNode {
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    char op;

    BinaryOpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r, char o)
        : left(std::move(l)), right(std::move(r)), op(o) {}

    int evaluate() const override {
        switch (op) {
            case '+': return left->evaluate() + right->evaluate();
            case '-': return left->evaluate() - right->evaluate();
            case '*': return left->evaluate() * right->evaluate();
            case '/':
                if (right->evaluate() == 0) throw std::runtime_error("Ділення на нуль");
                return left->evaluate() / right->evaluate();
            default: throw std::runtime_error("Невідома операція");
        }
    }
};

#endif // AST_H
