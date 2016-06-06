#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h"

#include "token.h"
#include "scanner.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    FILE *filePtr;
    /*---------Begin process cmd-line args and Redirection--------*/

    char filepath[100] = "good1.lan";


    if ( (filePtr = fopen(filepath, "r")) == NULL ) {
        printf("Cannot open input file %s.\n", argv[1]);
        printf("Syntax: testScanner [file] (.lan is implicit)\n");
        printf("test case 1");
        exit(1);
    }

    /*---------End process cmd-line args and Redirection----------*/

    /*---------Begin check if file empty-----------*/

    fseek(filePtr, 0, SEEK_END);
    if (ftell(filePtr) == 0) {
        printf("File is empty.\n");
        exit(1);
    } else {
        rewind(filePtr);
    }

    /*---------/End check if file empty-------------*/

    /*---------Begin check for invalid characters and max length-------------*/

    char c;
    int numLine = 1;

    int charCount = 0;
    char tempStr[MAX]; // ! Remember: C doesn't do array out-of-bound checking!
    int i;

    int isValid = 1; // true
    while ((c = fgetc(filePtr)) != EOF) {
        if (c == '\n')
            numLine++;

        // Exclude comment line starting with '//'
        if (c == '/') {
            if (fgetc(filePtr) == '/') {
                while ((c = fgetc(filePtr)) != '\n') {
                }
                numLine++;
            }
        }

        if (isalnum(c)) {
            tempStr[charCount] = c; // array out-of-bound checking not happen here
            charCount++;
            if (charCount > MAX) {
                printf("Word '%s' on line number %d is too long. \n", tempStr, numLine);
                exit(1);
            }
        } else if (isspace(c) || isExAcceptableChar(c)) {
            charCount = 0;
        } else {
            printf("Invalid character '%c' at line %d. \n", c, numLine);
            isValid = 0; // false
        }

    }

    if (isValid == 0) {
        printf("Something wrong with the input file. \n");
        exit(1);
    }

    /*---------/End check for invalid characters and max length-------------*/


    // At this time, the file is good. Now let scanner.c do the work

    displaySourceFile(filePtr);


    /*---------Begin Scanner-------------*/

    printf("%10s \t Line number \t %s\n\n", "Token instance", "Token type");
    numToken = 0; // extern var
    do {
        numToken++;
        tokens[numToken - 1] = scanner(filePtr);

        printToken(tokens[numToken - 1]);

    } while (tokens[numToken - 1].tokenType != EOFtk);

    /*---------/End Scanner-------------*/


    /*---------Begin Parser-------------*/

    parser(filePtr);

    /*---------/End Parser--------------*/


    fclose(filePtr);
    return 0;
}

