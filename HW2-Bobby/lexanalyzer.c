//Bobby Pappas bo666127 and Nathaniel Lyra na107394
// COP 3402 Summer 2020

// Include necessary C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexanalyzer.h"

// Global variables
char *reservedWords[] = {"const", "var", "procedure", "call", 
    "if", "then", "else", "while", "do", "read", "write", "odd"
};

char reservedSymbols[] = {'+', '-', '*', '/', '(', ')', '=',
    ',', '.', '<', '>', ';', ':', '}', '{'
};

char charArr[IDENT_CHARS_MAX_LENGTH];
token tokens[5000];
int tokenIdx = 0;
int arrIdx = 0;

FILE *ifp;
FILE *ofp;

// Function prototypes
void outputSourceProgram();
void outputLexTable();
void outputAll();
void outputLexList();
void inputProcess();
int isReservedWord(char*);
int isReservedSymbol(char);
tokenType reservedIdentifiersSwitch(char*);
tokenType reservedSymbolSwitch(char*);

// Main
int main(void)
{
    // Open .txt files and check if they're not null
    ifp = fopen("lex_input_1.txt", "r");
    ofp = fopen("lex_output_1.txt", "w");

    if ((ifp == NULL) || (ofp == NULL))
    {
        perror("Error ");
        return -1;
    }

    // Call functions, close files to prevent leakages/errors, and return
    inputProcess();
    outputAll();

    fclose(ifp);
    fclose(ofp);

    return (0);
}

// Primary output function
void outputAll()
{
    outputSourceProgram();
    outputLexTable();
    outputLexList();
}

// Processes input
void inputProcess()
{   
    // Do while loop to go through input .txt file
    do
    {
        char c = fgetc(ifp);

        if (tokenIdx == 0 && !isalpha(c))
        {
            printf("Error 1 Variable doesn't start with letter.\n");
            exit(0);
        }
        // If the end of file has reached, break out of loop
        if (c == '.')
        {
            tokens[tokenIdx].type = reservedSymbolSwitch(&c);
            tokens[tokenIdx].name[0] = c;
            tokenIdx++;
            return;
        }

        // Reset character array
        memset(charArr, '\0', sizeof(charArr)); // or \0 maybe ?
        arrIdx = 0;

        // If it's an invisible character
        if (c == ' ' || c == '\n' || c == '\t')
            continue;

        // If it's a reserved symbol..
        if (isReservedSymbol(c))
        {
            // Add it to our array and increment it
            charArr[arrIdx] = c;
            arrIdx++;

            // Get the next character to check for double symbols
            c = fgetc(ifp);

            if (c == '.')
            {
                if (charArr[0] == '}' || charArr[0] == '{')
                {
                    tokens[tokenIdx].type = reservedSymbolSwitch(charArr);
                    tokens[tokenIdx].name[0] = charArr[0];
                    tokenIdx++;
                }

                tokens[tokenIdx].type = reservedSymbolSwitch(&c);
                tokens[tokenIdx].name[0] = c;
                tokenIdx++;
                return;
            }

            // If it is a double symbol, add to array, increment, and tokenize
            if (isReservedSymbol(c) && (c != ' ') && (c != '\t') && (c != '\n'))
            {
                if (c == ';')
                {
                    // Tokenize symbol held in charArr
                    tokens[tokenIdx].type = reservedSymbolSwitch(charArr);
                    tokens[tokenIdx].name[0] = charArr[0];
                    tokenIdx++;

                    //Tokenize character held in c
                    tokens[tokenIdx].type = reservedSymbolSwitch(&c);
                    tokens[tokenIdx].name[0] = c;
                    tokenIdx++;
                    continue;
                }
                charArr[arrIdx] = c;
                arrIdx++;

                tokens[tokenIdx].type = reservedSymbolSwitch(charArr);
                strcpy(tokens[tokenIdx].name, charArr);
                tokenIdx++;
                continue;
            }
            else if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
            {
                tokens[tokenIdx].type = reservedSymbolSwitch(charArr);
                strcpy(tokens[tokenIdx].name, charArr);
                tokenIdx++;
                continue;
            }
            else
            {
                printf("Error: whoops\n");
                exit(0);
            }
        }

        // If it's an alphabetical letter
        if (isalpha(c))
        {
            charArr[arrIdx] = c;
            arrIdx++;

            if (arrIdx > 11)
            {
                printf("Error 3 Name too long.\n");
                exit(0);
            }

            while (c != EOF)
            {
                // Get the next character
                c = fgetc(ifp);

                if (c == '.')
                {
                    tokens[tokenIdx].type = reservedSymbolSwitch(&c);
                    tokens[tokenIdx].name[0] = c;
                    tokenIdx++;
                    return;
                }


                // If the next character is an alaphabetical letter
                if (isalpha(c))
                {
                    // Add it to our character array and increase index
                    charArr[arrIdx] = c; 
                    arrIdx++;

                    if (arrIdx > 11)
                    {
                        printf("Error 3 Name too long.\n");
                        exit(0);
                    }

                    // Check if it's a reserved word
                    if (isReservedWord(charArr))
                    {
                        strcpy(tokens[tokenIdx].name, charArr); 
                        tokens[tokenIdx].type = reservedIdentifiersSwitch(charArr);
                        tokenIdx++;
                        break;
                    }
                    // If it's not a reserved word
                    else
                    {
                        continue;
                    }
                }
                // If the next character is not an alphabetical letter, and not a whitespace, then is must be a variable name
                else if (!isalpha(c) && (c != ' ') && (c != '\n') && (c != '\t'))
                {
                    // Add it to our array of tokens and increase index
                    tokens[tokenIdx].type = identsym;
                    strcpy(tokens[tokenIdx].name, charArr); 
                    tokenIdx++;

                    // The current character stored then must be a symbol, let's check and add it to our tokens array
                    if (isReservedSymbol(c))
                    {
                        strcpy(tokens[tokenIdx].name, &c); // dereferenced this to make it work properly -bobby
                        tokens[tokenIdx].type = reservedSymbolSwitch(&c); // dereferenced this to make it work properly -bobby
                        tokenIdx++;
                        break;
                    }
                    // If current character is not another letter, and it's not a symbol, well then fuck it. Throw an error.
                    else
                    {
                        printf("Error: Invalid character\n");
                        exit(0);
                    }
                }
                // Unless character after variable name is a white space
                else if (c == ' ')
                {
                    tokens[tokenIdx].type = identsym;
                    strcpy(tokens[tokenIdx].name, charArr); 
                    tokenIdx++;
                    break;
                }

                break;
            }
        }
        // If it's a numerical digit
        else if (isdigit(c))
        {
            // Add it to our character array and increment index
            charArr[arrIdx] = c;
            arrIdx++;
            
            // While loop until end of file
            while (c != EOF)
            {
                // Get next character
                c = fgetc(ifp);

                if (c == '.')
                {
                    tokens[tokenIdx].type = reservedSymbolSwitch(&c);
                    tokens[tokenIdx].name[0] = c;
                    tokenIdx++;
                    return;

                }

                // If it's not a digit
                if (!isdigit(c) && (c != ' ') && (c != '\t') && (c != '\n'))
                {
                    // Tokenize the prior digit and increment index
                    tokens[tokenIdx].type = numbersym;
                    strcpy(tokens[tokenIdx].numberVal, charArr);
                    tokenIdx++;

                    // And check if it's a reserved symbol. If so, tokenize it and break out of digit loop.
                    if (isReservedSymbol(c))
                    {
                        tokens[tokenIdx].type = reservedSymbolSwitch(&c);
                        strcpy(tokens[tokenIdx].name, &c);
                        tokenIdx++;
                        break;
                    }
                    // Else invalid character
                    else
                    {
                        printf("Error: Invalid character\n");
                        exit(0);
                    }
                }
                // If it is a digit, add it to our character array and continue to next iteration.
                else if (isdigit(c))
                {
                    if(arrIdx > 5)
                    {
                        printf("Error 2 Number too long.\n");
                        exit(0);
                    }
                    charArr[arrIdx] = c;
                    arrIdx++;
                    continue;
                }  

                break; 
            }
        }

        // Handling invalid symbol
        else
        {
            printf("Error 4 Invalid Symbols.\n");
            exit(0);
        }
        
    } while (1);
    

}

// Outputs source program (first section of output)
void outputSourceProgram()
{
    rewind(ifp);

    char text = fgetc(ifp);

    // Print header
    fprintf(ofp, "Source Program:\n");

    // Print input file text (source program)
    while (text != EOF)
    {
        fprintf(ofp, "%c", text);
        text = fgetc(ifp);
    }

    fprintf(ofp, "\n\n");
}

// Outputs lexeme table (second section of output)
void outputLexTable()
{
    // Print headers
    fprintf(ofp, "Lexeme Table:\n");
    fprintf(ofp, "lexeme\t\ttoken type\n");

    // Print actual output here
    for (int i = 0; i < tokenIdx; i++)
    {
        if (tokens[i].type == numbersym)
        {
            fprintf(ofp, "%s\t%12d\n", tokens[i].numberVal, tokens[i].type);
        }
        else
        {
            fprintf(ofp, "%s\t%12d\n", tokens[i].name, tokens[i].type);
        }
    }
}

// Outputs lexeme list (third section of output)
void outputLexList()
{
    // Print header
    fprintf(ofp, "Lexeme List:\n");

    // Print actual output here
    for (int i = 0; i < tokenIdx; i++)
    {
        if (tokens[i].type == identsym)
            fprintf(ofp, "%d\t%s\t", tokens[i].type, tokens[i].name);
        else
            fprintf(ofp, "%d\t", tokens[i].type); 
    }
}

// Checks to see if the word being passed in is one of the reserved words
int isReservedWord(char *word)
{
    // Check through array of reserved words to see if they match with
    // any of them
    for (int i = 0; i < RESERVED_WORDS_MAX_NUM; i++)
    {
        // If it is a reserved word, return 1
        if (strcmp(word, reservedWords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// Checks to see if the word being passed in is one of the reserved symbols
int isReservedSymbol(char c)
{
    // Loop through symbols array and see if our character being passed in
    // matches any of them
    for (int i = 0; i < NUM_SYMBOLS_MAX_LENGTH; i++)
    {
        // If it is a reserved symbol return 1
        if (c == reservedSymbols[i])
        {
            return 1;

        }
    }

    return 0;
}

// Switch to respective token type value depending on parameter comparison for reserved identitifier words
tokenType reservedIdentifiersSwitch(char *word)
{
    if (strcmp(word, "null") == 0)
        return nulsym;
    else if (strcmp(word, "const") == 0)
        return constsym;
    else if (strcmp(word, "var") == 0)
        return varsym;
    else if (strcmp(word, "procedure") == 0)
        return procsym;
    else if (strcmp(word, "call") == 0)
        return callsym;
    else if (strcmp(word, "if") == 0)
        return ifsym;
    else if (strcmp(word, "then") == 0)
        return thensym;
    else if (strcmp(word, "else") == 0)
        return elsesym;
    else if (strcmp(word, "while") == 0)
        return whilesym;
    else if (strcmp(word, "do") == 0)
        return dosym;
    else if (strcmp(word, "read") == 0)
        return readsym;
    else if (strcmp(word, "write") == 0)
        return writesym;
    else if (strcmp(word, "odd") == 0)
        return oddsym;
    
    return -1;
}

// Switch to respective token type value depending on parameter comparison for symbols
tokenType reservedSymbolSwitch(char *word)
{
    if (strcmp(word, "+") == 0)
        return plussym;
    else if (strcmp(word, "-") == 0)
        return minussym;
    else if (strcmp(word, "*") == 0)
        return multsym;
    else if (strcmp(word, "/") == 0)
        return slashsym;
    else if (strcmp(word, "(") == 0)
        return lparentsym;
    else if (strcmp(word, ")") == 0)
        return rparentsym;
    else if (strcmp(word, "=") == 0)
        return eqlsym;
    else if (strcmp(word, "<>") == 0)
        return neqsym;
    else if (strcmp(word, ",") == 0)
        return commasym;
    else if (strcmp(word, ".") == 0)
        return periodsym;
    else if (strcmp(word, "<") == 0)
        return lessym;
    else if (strcmp(word, "<=") == 0)
        return leqsym;
    else if (strcmp(word, ">") == 0)
        return gtrsym;
    else if (strcmp(word, ">=") == 0)
        return geqsym;
    else if (strcmp(word, ";") == 0)
        return semicolonsym;
    else if (strcmp(word, ":=") == 0)
        return becomessym;
    else if (strcmp(word, "{") == 0)
        return lbracesym;
    else if (strcmp(word, "}") == 0)
        return rbracesym;

    return -1;
}

