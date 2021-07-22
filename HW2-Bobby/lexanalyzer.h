// Prevents a duplicate header file from being created when called
#ifndef HEADER_FILE
#define HEADER_FILE

// Constants
#define IDENT_CHARS_MAX_LENGTH 11
#define NUMS_DIGITS_MAX_LENGTH 5
#define NUM_SYMBOLS_MAX_LENGTH 15
#define RESERVED_WORDS_MAX_NUM 12

// Struct definitions
typedef enum
{
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    lbracesym, rbracesym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} tokenType;

typedef struct 
{
    tokenType type;
    char name[IDENT_CHARS_MAX_LENGTH];
    char numberVal[NUMS_DIGITS_MAX_LENGTH]; 
} token;


#endif