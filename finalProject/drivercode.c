// Bobby Pappas bo666127 and Nathaniel Lyra na107394
// COP 3402 Summer '20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"

FILE *ifp;
FILE *ofp;

void printOutVMachine();
void printOutLexList();
void printOutLexSymbolicRep();
void printOutParser();
char* returnName(tokenType kind);

// Driver code main
int main (int argc, char *argv[])
{
    bool printLexeme = false, printAssembly = false, printVMachine = false;
    char **input = argv;

    // Goes through user argument when he/she compiles code and set flag variables accordingly.
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(input[i], "-l") == 0)
        {
            printLexeme = true;
        }
        if (strcmp(input[i], "-a") == 0)
        {
            printAssembly = true;
        }
        if (strcmp(input[i], "-v") == 0)
        {  
            printVMachine = true;
        }
    }

    // Run different source code files
    main_lex();
    printf("\n\n");
    execute_p();
    main_vmachine();

    // Open the output text file to write for all chosen programs.
    ofp = fopen("all_output.txt", "w");

    // Read from lexeme output and write to all_output.
    if (printLexeme)
    {
        // First, print out the token list (internal representation) to ofp.
        printOutLexList();

        // Next, print out the symbolic representation to ofp.
        printOutLexSymbolicRep();

        // Check for number of errors.
        if (numErrs == 0)
        {
            fprintf(ofp, "No errors, program is syntactically correct.\n\n");
        }
        else
        {
            fprintf(ofp, "\n\n");
        }
    }

    // Read from assembly code output and write to all_output (ofp).
    if (printAssembly)
    {
        // Prints to ofp.
        printOutParser();
    }

    // Read from virtual machine output and write to all_output.
    if (printVMachine)
    {
        // Prints to ofp.
        printOutVMachine();
    }

    fclose(ofp);
    return 0;
}

// Helper function that prints out virtual machine output to ofp.
void printOutVMachine()
{
    ifp = fopen("pmvm_output.txt", "r");
    char c;

    while (c = fgetc(ifp) != EOF)
    {
        fprintf(ofp, "%c", c);
    }

    fprintf(ofp, "\n");
    fclose(ifp);
}

// Helper function that prints out lexical list to ofp.
void printOutLexList()
{
    ifp = fopen("lex_output_1.txt", "r");
    char c;

    while (c = fgetc(ifp) != EOF)
    {
        fprintf(ofp, "%c", c);
    }
    
    fprintf(ofp, "\n");
    fclose(ifp);
}

// Helper function that prints out lexical symbolic representation to ofp.
void printOutLexSymbolicRep()
{
    ifp = fopen("lex_output_1.txt", "r");
    int i = 0;
    char c;

    while (c = fgetc(ifp) != EOF)
    {
        if (tokens[i].type == 0)
        {
            fprintf(ofp, "\n");
            break;
        }

        // Print type
        fprintf(ofp, "%s ", returnName(tokens[i].type));

        // Print name
        if (tokens[i].type == identsym)
        {
            fprintf(ofp, "%s ", tokens[i].name);
        }

        // Print value
        if (tokens[i].type == numbersym)
        {
            fprintf(ofp, "%s ", tokens[i].numberVal);
        }

        // Increment counter
        i++;
    }

    fprintf(ofp, "\n\n");
    fclose(ifp);
}

// Helper function that prints out parser output to ofp.
void printOutParser()
{
    ifp = fopen("parser_output.txt", "r");

    char c;

    while (c = fgetc(ifp) != EOF)
    {
        fprintf(ofp, "%c", c);
    }

    fprintf(ofp, "\n");
    fclose(ifp);
}

// Helper function for returning string representation of token type to ofp.
char* returnName(tokenType kind)
{
    switch (kind)
    {
        case (nulsym):
            return "nulsym";
        case (identsym):
            return "identsym";
        case (numbersym):
            return "numbersym";
        case (plussym):
            return "plussym";
        case (minussym):
            return "minussym";
        case (multsym):
            return "multsym";
        case (slashsym):
            return "slashsym";
        case (oddsym):
            return "oddsym";
        case (eqlsym):
            return "eqlsym";
        case (neqsym):
            return "neqsym";
        case (lessym):
            return "lessym";
        case (leqsym):
            return "leqsym";
        case (gtrsym):
            return "gtrsym";
        case (geqsym):
            return "geqsym";
        case (lparentsym):
            return "lparentsym";
        case (rparentsym):
            return "rparentsym";
        case (commasym):
            return "commasym";
        case (semicolonsym):
            return "semicolonsym";
        case (periodsym):
            return "periodsym";
        case (becomessym):
            return "becomessym";
        case (beginsym):
            return "beginsym";
        case (endsym):
            return "endsym";
        case (ifsym):
            return "ifsym";
        case (thensym):
            return "thensym";
        case (whilesym):
            return "whilesym";
        case (dosym):
            return "dosym";
        case (callsym):
            return "callsym";
        case (constsym):
            return "constsym";
        case (varsym):
            return "varsym";
        case (procsym):
            return "procsym";
        case (writesym):
            return "writesym";
        case (readsym):
            return "readsym";
        case (elsesym):
            return "elsesym";
        default:
            return NULL;
    }
}