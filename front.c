/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>   // For input/output functions
#include <ctype.h>   // For character classification functions
/* Global declarations */
/* Variables */
int charClass;          // Class of current character
char lexeme[100];  // Array to store the current lexeme
char nextChar;          // The current character read from input
int lexLen;             // Current length of the lexeme
int token;              // Stores the token type returned by lex()(not used directly)
int nextToken;          // Stores the current token code
FILE *in_fp, *fopen();  // File pointer for input file
/* Function declarations */
void addChar();         // Adds current character to the lexeme
void getChar();         // Gets the next character and sets its class
void getNonBlank();     // Skips whitespace
int lex();              // Main lexical analyzer function
/* Character classes */
#define LETTER 0        // Represents alphabetic character
#define DIGIT 1         // Represents numeric character
#define UNKNOWN 99     // Represents any other character
/* Token codes */
#define INT_LIT 10      // Integer literal
#define IDENT 11        // Identifier
#define ASSIGN_OP 20    // Assignment operator (=)
#define ADD_OP 21       // Addition operator (+)
#define SUB_OP 22       // Subtraction operator (-)
#define MULT_OP 23      // Multiplication operator (*)
#define DIV_OP 24       // Division operator (/)
#define LEFT_PAREN 25   // Left parenthesis (
#define RIGHT_PAREN 26  // Right parenthesis )
/******************************************************/
/* main driver */
main() {
    // Open the input file "front.in" for reading
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();  // Read the first character
        do {
            lex();  // Analyze and tokenize each lexeme
        } while (nextToken != EOF);  // Continue until end of file
    }
}
/*****************************************************/
/* lookup : function  for identifying singlecharacter operators and parentheses and assigning them the correct token type. */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;  // Unknown character
            break;
    }
    return nextToken;
}
/*****************************************************/
/* addChar : adds nextChar to the lexeme buffer */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;  // Add character and increment length
        lexeme[lexLen] = 0;           // Null terminate string
    } else
        printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar : reads next character from input and sets character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}
/*****************************************************/
/* skips whitespace characters */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}
/*****************************************************/
/* the lexical analyzer function */
int lex() {
    lexLen = 0;         // Reset lexeme length
    getNonBlank();      // Skip whitespace
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* Operators and parentheses */
        case UNKNOWN:
            lookup(nextChar);  // Look up the token type
            getChar();
            break;
        /* End of file */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    // Print the token and lexeme
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;}
