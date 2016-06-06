#ifndef SYMDEF_H
#define SYMDEF_H

char *keywords[15] = {"begin", "end", "then", "if", "else", "for", "var",
                      "int", "float", "main",
                      "read", "printf", "void", "return", "program"};

char *relationalOperators[] = {"==", "<", ">", "!=", ">=", "<="};

// char *otherOperators[] = {":", "+", "-", "*", "/", "%"};
char otherOperators[7] = {'=', ':', '+', '-', '*', '/', '%'};

// char *delimiters[9] = {".", "(", ")", ",", "{", "}", ";", "[", "]"};
char delimiters[9] = {'.', '(', ')', ',', '{', '}', ';', '[', ']'};

char word[MAX];
int wi = 0; // index of word string

char numStr[MAX];
int ni = 0;

#endif


