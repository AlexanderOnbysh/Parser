#ifndef SCANNER_H
#define SCANNER_H

int isExAcceptableChar(char);

int isDelimiter(char);
int isOtherOperators(char);
int isStartRelationalOperator(char);
int isKeyword(char *);

Token scanner(FILE *);

void printToken(Token);

TokenType getTokenTypeOfKeyword(char *);
TokenType getTokenTypeOfDelimiter(char);
TokenType getTokenTypeOfOtherOperator(char);

void displaySourceFile(FILE *);


#endif


