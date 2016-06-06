#ifndef TOKEN_H
#define TOKEN_H

#define MAX 50 // max length of each word string, not including '\0'
#define LIMIT 200 // max number of word strings in an input file

extern int lineNum;

typedef enum {
    // Identifier: begin with a letter, and continue with any number
    // of letters. No ID is longer than MAX
            IDtk,

    // Keywords (start finish then if repeat var int float do
    // read print void return dummy program)
            STARTtk, FINISHtk, THENtk, IFtk, FORtk, VARtk, INTtk, FLOATtk,
    MAINtk, READtk, PRINTtk, VOIDtk, RETURNtk, DUMMYtk, PROGRAMtk,

    // Number: sequence of decimal digits, no sign, no longer than MAX digits
            NUMBERtk,

    // Relational Operators (==  <  >  =!=    =>  =<)
            EQUALtk, GREATERtk, LESStk, DIFFtk, GREATEREQtk, LESSEQtk,

    // Other operators (= :  +  -  *  / %)
            ASSIGNtk, COLONtk, ADDtk, SUBTRACTtk, MULtk, DIVtk, REMAINDERtk,

    // Delimiters (. (  ) , { } ; [ ])
            DOTtk, LEFTPAtk, RIGHTPAtk, COMMAtk, LEFTBRACEtk, RIGHTBRACEtk,
    SEMICOLONtk, LEFTBRACKETtk, RIGHTBRACKETtk,

    NAtk, // N/A token
    EOFtk

} TokenType;


struct tokenTag {
    char str[MAX];
    TokenType tokenType;
    int lineNum;

    struct tokenTag *next; // linked-list, used for parse tree
};
typedef struct tokenTag Token;

extern int numToken;
extern Token tokens[500]; // list of all tokens (array of numToken)


#endif
