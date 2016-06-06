#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include <ctype.h>
#include "scanner.h"
#include "parser.h"

Token tk = {"N/A", NAtk, 0};
FILE *fP = NULL;

Node * getNode(NodeType);
void printParseTree(Node *, int);

Node * program();
void parser(FILE *filePtr) {
    lineNum = 1; // reset line number
    fP = filePtr;
    rewind(fP);

    tk = scanner(fP);
    Node *root = NULL;

    root = program();

    if (tk.tokenType == EOFtk)
        printf("Parse OK! \n");
    else {
        exit(1);
    }

    printf("\n*-----Parse Tree-----*\n");
    printParseTree(root, 0);
    return;
}

void insertToken(Node *, Token);
Node * var();
Node * block();
Node * program() {
    Node *node = getNode(programNode);

    //node->child1 = var();
    if (tk.tokenType == VOIDtk || tk.tokenType == INTtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
    } else {
        printf("ERROR: expect INTtk or VOIDtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }

    node->child1 = var();
    if (tk.tokenType == MAINtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
    } else {
        printf("ERROR: expect MAINtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }

    if (tk.tokenType == LEFTPAtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        //    return node;
    } else {
        printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }

    if (tk.tokenType == RIGHTPAtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        //    return node;
    } else {
        printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
    node->child2 = block();
}

Node * mvars();
Node * var() {
    Node *node = getNode(varNode);
    if (tk.tokenType == INTtk || tk.tokenType == FLOATtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        if (tk.tokenType == IDtk) {
            insertToken(node, tk);
            //node->token = tk;
            tk = scanner(fP);
        } else {
            printf("ERROR: expect IDtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
        node->child1 = mvars();
        if (tk.tokenType == SEMICOLONtk) {
            //insertToken(node, tk);
            tk = scanner(fP);
            return node;
        } else {
            printf("ERROR: expect SEMICOLONtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
    }
    else {
        return node; // predict <var> --> empty
    }
}

Node * mvars() {
    Node * node = getNode(mvarsNode);
    if (tk.tokenType == COMMAtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        if (tk.tokenType == IDtk) {
            insertToken(node, tk);
            //node->token = tk;
            tk = scanner(fP);
        } else {
            printf("ERROR: expect IDtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
        node->child1 = mvars();
        return node;
    } else {
        return node; // predict <mvars> --> empty
    }
}

Node * stats();
Node * block() {
    Node *node = getNode(blockNode);
    if (tk.tokenType == LEFTBRACEtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        node->child1 = var();
        node->child2 = stats();
        if (tk.tokenType == RIGHTBRACEtk) {
            //insertToken(node, tk);
            tk = scanner(fP);
            return node;
        } else {
            printf("ERROR: expect RIGHTBRACEtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
    } else {
        printf("ERROR: expect LEFTBRACEtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}

Node * stat();
Node * mStat();
Node * stats() {
    Node *node = getNode(statsNode);
    node->child1 = stat();
    node->child2 = mStat();
    return node;
}

Node * in();
Node * out();
Node * loop();
Node * ifTk();
Node * assign();

Node * stat() {
    Node *node = getNode(statNode);
    if (tk.tokenType == READtk) {
        node->child1 = in();
        return node;
    } else if (tk.tokenType == PRINTtk) {
        node->child1 = out();
        return node;
    } else if (tk.tokenType == STARTtk) {
        node->child1 = block();
        return node;
    } else if (tk.tokenType == IFtk) {
        node->child1 = ifTk();
        return node;
    } else if (tk.tokenType == FORtk) {
        node->child1 = loop();
        return node;
    } else if (tk.tokenType == IDtk) {
        node->child1 = assign();
        return node;
    } else {
        printf("ERROR: expect either READtk, PRINTtk, STARTtk, Iftk, FORtk, or IDtk. ");
        printf("But received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}

Node * mStat() {
    Node *node = getNode(mStatNode);
    if (tk.tokenType == READtk || tk.tokenType == PRINTtk || tk.tokenType == STARTtk
        || tk.tokenType == IFtk || tk.tokenType == FORtk || tk.tokenType == IDtk) {
        node->child1 = stat();
        node->child2 = mStat();
        return node;
    } else {
        return node; // predict <mStat> --> empty
    }
}

Node * in() {
    Node *node = getNode(inNode);
    if (tk.tokenType == READtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        if (tk.tokenType == IDtk) {
            insertToken(node, tk);
            //node->token = tk;
            tk = scanner(fP);
            if (tk.tokenType == DOTtk) {
                //insertToken(node, tk);
                tk = scanner(fP);
                return node;
            } else {
                printf("ERROR: expect DOTtk, but received %s on line #%d \n", tk.str, tk.lineNum);
                exit(1);
            }
        } else {
            printf("ERROR: expect IDtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
    } else {
        printf("ERROR: expect READtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}

Node * expr();
Node * out() {
    Node *node = getNode(outNode);
    if (tk.tokenType == PRINTtk)
    {
        //insertToken(node, tk);
        tk = scanner(fP);

        if (tk.tokenType == LEFTPAtk) {
            tk = scanner(fP);
            node->child1 = expr();
        }
        else
        {
            printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
        if (tk.tokenType == RIGHTPAtk)
        {
            //insertToken(node, tk);
            tk = scanner(fP);
            //return node;
        } else
        {
            printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
        if (tk.tokenType == SEMICOLONtk)
        {
            //insertToken(node, tk);
            tk = scanner(fP);
            // return node;
        } else
        {
            printf("ERROR: expect SEMICOLONtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
    }
    else
        {
            printf("ERROR: expect PRINTtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
}

Node * t();
Node * expr() {
    Node *node = getNode(exprNode);
    node->child1 = t();
    if (tk.tokenType == MULtk) {
        tk = scanner(fP);
        node->child2 = expr();
        return node;
    } else if (tk.tokenType == DIVtk) {
        tk = scanner(fP);
        node->child2 = expr();
        return node;
    } else {
        return node; // predict empty after <T>
    }
}

Node * f();
Node * t() {
    Node *node = getNode(tNode);
    node->child1 = f();
    if (tk.tokenType == ADDtk) {
        tk = scanner(fP);
        node->child2 = t();
        return node;
    } else if (tk.tokenType == SUBTRACTtk) {
        tk = scanner(fP);
        node->child2 = t();
        return node;
    } else {
        return node; // predict empty after <F>
    }
}

Node * r();
Node * f() {
    Node *node = getNode(fNode);
    if (tk.tokenType == SUBTRACTtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        node->child1 = f();
        return node;
    } else {
        node->child1 = r();
        return node;
    }
}

Node * r() {
    Node *node = getNode(rNode);
    if (tk.tokenType == LEFTPAtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        node->child1 = expr();
        if (tk.tokenType == RIGHTPAtk) {
            insertToken(node, tk);
            tk = scanner(fP);
            return node;
        } else {
            printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
            exit(1);
        }
    } else if (tk.tokenType == IDtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == NUMBERtk) {
        insertToken(node, tk);
        //node->token = tk;
        tk = scanner(fP);
        return node;
    } else {
        printf("ERROR: expect either LEFTPAtk, or IDtk, or NUMBERtk. ");
        printf("But received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}

/*------ for re-written grammar ------*/
void y() {
    if (tk.tokenType == ADDtk) {
        tk = scanner(fP);
        t();
        return;
    } else if (tk.tokenType == SUBTRACTtk) {
        tk = scanner(fP);
        t();
        return;
    } else {
        return; // predict <Y> --> empty
    }
}

void x() {
    if (tk.tokenType == MULtk) {
        tk = scanner(fP);
        expr();
        return;
    } else if (tk.tokenType == DIVtk) {
        tk = scanner(fP);
        expr();
        return;
    } else {
        return; // predict <X> --> empty
    }
}
/*------ /for re-written grammar ------*/

Node * ro();
Node * ifTk() {
    Node *node = getNode(ifNode);
    if (tk.tokenType == IFtk) {
        //insertToken(node, tk);
        tk = scanner(fP);
        if (tk.tokenType == LEFTPAtk) {
            tk = scanner(fP);
            node->child1 = expr();
            node->child2 = ro();
            node->child3 = expr();
            if (tk.tokenType == RIGHTPAtk) {
                tk = scanner(fP);
                node->child4 = block();
                return node;
            } else {
                printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n",
                       tk.str, tk.lineNum);
                exit(1);
            }
        } else {
            printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n",
                   tk.str, tk.lineNum);
            exit(1);
        }
    } else {
        printf("ERROR: expect IFtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}

Node * ro() {
    Node *node = getNode(roNode);
    if (tk.tokenType == LESSEQtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == GREATEREQtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == EQUALtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == GREATERtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == LESStk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else if (tk.tokenType == DIFFtk) {
        insertToken(node, tk);
        tk = scanner(fP);
        return node;
    } else {
        printf("ERROR: expect relational operator, but received %s on line #%d \n",
               tk.str, tk.lineNum);
        exit(1);
    }
}

Node * assign() {
    Node *node = getNode(assignNode);
    if (tk.tokenType == IDtk) {
        insertToken(node, tk);
        //node->token = tk;
        tk = scanner(fP);
        if (tk.tokenType == ASSIGNtk) {
            //insertToken(node, tk);
            tk = scanner(fP);
            expr();
            if (tk.tokenType == SEMICOLONtk) {
                insertToken(node, tk);
                tk = scanner(fP);
                return node;
            } else {
                printf("ERROR: expect SEMICOLONtk, but received %s on line #%d \n",
                       tk.str, tk.lineNum);
                exit(1);
            }
        } else {
            printf("ERROR: expect ASSIGNtk, but received %s on line #%d \n",
                   tk.str, tk.lineNum);
            exit(1);
        }
    } else {
        printf("ERROR: expect IDtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}


Node * loop() {
    Node *node = getNode(loopNode);
    if (tk.tokenType == FORtk)
    {
        //insertToken(node, tk);
        tk = scanner(fP);
        if (tk.tokenType == LEFTPAtk) {
            tk = scanner(fP);

            node->child1 = assign();
            tk = scanner(fP);
            node->child1 = ro();
            node->child2 = expr();
        } else
        {
            printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n",
                   tk.str, tk.lineNum);
            exit(1);
        }

        if (tk.tokenType == SEMICOLONtk)
        {
            tk = scanner(fP);
            node->child1 = expr();

        }
        else
        {
            printf("ERROR: expect SEMICOLONtk, but received %s on line #%d \n",
                   tk.str, tk.lineNum);
            exit(1);

        }
        if (tk.tokenType == RIGHTPAtk) {
            tk = scanner(fP);
            node->child1 = block();
        }
        else
        {
            printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n",
                   tk.str, tk.lineNum);
            exit(1);
        }


    } else {
        printf("ERROR: expect FORtk, but received %s on line #%d \n", tk.str, tk.lineNum);
        exit(1);
    }
}


/*---------------TREE---------------*/

// Hard-code to map with enum NodeType declared in parser.h
char *nodeTypeStrArr[] = {
        "<program>", "<block>", "<var>", "<mvars>", "<expr>", "<x>", "<t>", "<y>", "<f>", "<r>",
        "<stats>", "<mStat>", "<stat>", "<in>", "<out>", "<if>", "<loop>", "<assign>", "<ro>"
};

// Hard-coded to map with enum TokenType declared in token.h
char *tokenStrArr[] = {
        "IDtk",

        "STARTtk", "FINISHtk", "THENtk", "IFtk", "FORtk", "VARtk", "INTtk", "FLOATtk", "MAINtk",
        "READtk", "PRINTtk", "VOIDtk", "RETURNtk", "DUMMYtk", "PROGRAMtk",

        "NUMBERtk",

        "EQUALtk", "GREATERtk", "LESStk", "DIFFtk", "GREATEREQtk", "LESSEQtk",

        "ASSIGNtk", "COLONtk", "ADDtk", "SUBTRACTtk", "MULtk", "DIVtk", "REMAINDERtk",

        "DOTtk", "LEFTPAtk","RIGHTPAtk", "COMMAtk", "LEFTBRACEtk", "RIGHTBRACEtk",
        "SEMICOLONtk", "LEFTBRACKETtk", "RIGHTBRACKETtk",

        "NAtk", "EOFtk"
};

void printParseTree(Node *root, int level) {
    if (root == NULL) return;
    printf("%*s" "%d %s ", level * 4, "", level, nodeTypeStrArr[root->nodeType]);
    // printf("%*s" "%s ", level * 4, "", nodeTypeStrArr[root->nodeType]);
/*
	if (root->token.tokenType != NAtk && root->token.tokenType != EOFtk) {
		// printf(" Token on line #%d is %s", root->token.lineNum, root->token.str);
		printf(" [Token %s on line #%d]", root->token.str, root->token.lineNum);
	}
*/

    Token *tmp = root->tokenPtr;
    int isTokenFound = 0; // false
    if (tmp != NULL) {
        isTokenFound = 1;
        printf("{Token(s) found: ");
    }

    while (tmp != NULL) {
        int isLastToken = 0; // false
        printf("%s (%s, #%d)", tmp->str, tokenStrArr[tmp->tokenType], tmp->lineNum);
        tmp = tmp->next;
        if (tmp == NULL)
            isLastToken = 1;
        if (! isLastToken) {
            printf(", and ");
        }
    }

    if (isTokenFound) {
        printf("}");
    }

    printf("\n");

    printParseTree(root->child1, level + 1);
    printParseTree(root->child2, level + 1);
    printParseTree(root->child3, level + 1);
    printParseTree(root->child4, level + 1);
}

// Mark the new node by its type
Node *getNode(NodeType nodeType) {
    Node *node;
    node = (Node *) malloc(sizeof(Node));

    node->nodeType = nodeType;

    node->tokenPtr = NULL;
    node->child1 = node->child2 = node->child3 = node->child4 = NULL;

    return node;
}

Token *getTokenPtr(Token tk) {
    Token *tokenPtr = (Token *) malloc(sizeof(Token));
    strcpy(tokenPtr->str, tk.str);
    tokenPtr->lineNum = tk.lineNum;
    tokenPtr->tokenType = tk.tokenType;
    return tokenPtr;
}

// Insert new token at the end of the linked-list of tokens
void insertToken(Node *node, Token tk) {
    Token *new = getTokenPtr(tk);
    if (node->tokenPtr == NULL) {
        node->tokenPtr = new;
    } else {
        Token *tmp = node->tokenPtr;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
}

/*---------------/TREE---------------*/

