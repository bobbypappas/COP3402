// Prevents a duplicate header file from being created when called
// #ifndef HEADER_FILE
// #define HEADER_FILE

// Constants
#define IDENT_CHARS_MAX_LENGTH 11
#define NUMS_DIGITS_MAX_LENGTH 5
#define NUM_SYMBOLS_MAX_LENGTH 13
#define RESERVED_WORDS_MAX_NUM 12
#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

// Struct definitions
typedef enum
{
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} tokenType;

typedef struct 
{
    tokenType type;
    char name[IDENT_CHARS_MAX_LENGTH];
    char numberVal[NUMS_DIGITS_MAX_LENGTH]; 
} token;

typedef struct 
{
    int kind; // const = 1, var = 2, proc = 3
    char name[10]; // name up to 11 chars
    int val; // number ASCII values
    int level; // L level
    int addr; // M address
    int mark; // to indicate unavailable or delete d
} symbol;

typedef struct instruction {
    int op;
    int l;
    int m;
} instruction;

// Extended variables
extern int numErrs;
extern int codeCount;
extern token tokens[5000];
extern instruction instructionSet[MAX_CODE_LENGTH];

// Function prototypes
void execute_p(); // Parser
void main_lex(); // Lexical Analyzer
void main_vmachine(); // Virtual Machine




// #endif