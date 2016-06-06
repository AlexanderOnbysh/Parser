//
// Created by Alexander Onbysh on 6/4/16.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H
#include "scanner.h"
#include "parser.h"
#include "token.h"

void parser(FILE *);

// Represent non-terminal token nodes
typedef enum {
    programNode, blockNode, varNode, mvarsNode, exprNode, xNode,
    tNode, yNode, fNode, rNode, statsNode, mStatNode, statNode,
    inNode, outNode, ifNode, loopNode, assignNode, roNode
} NodeType;

/*------- TREE -------*/
struct nodeTag {
    NodeType nodeType;
    Token *tokenPtr; // linked-list of tokens of this node
    struct nodeTag *child1; // usually only up to 3 children needed
    struct nodeTag *child2;
    struct nodeTag *child3;
    struct nodeTag *child4; // but for <if> and <loop>, 4 children needed
};
typedef struct nodeTag Node;



void insertToken(Node *node, Token tk);
Token *getTokenPtr(Token tk);
Node * ro();
Node * var();
Node *getNode(NodeType nodeType);
Node * program();
void parser(FILE *filePtr);
void printParseTree(Node *root, int level);
#endif //PARSER_PARSER_H
