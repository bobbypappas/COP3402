//Bobby Pappas (bo666127) and Nathaniel Lyra (na107394)
//COP3402 Summer 2020

// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "header.h"

// Declare array of instruction structs of length MAX_CODE_LENGTH
instruction instructionSet[MAX_CODE_LENGTH];

// File for input and output
FILE *ifp;
FILE *ofp;

// Initial values for PM/0 CPU registers
int stack[MAX_DATA_STACK_HEIGHT];
int sp = MAX_DATA_STACK_HEIGHT;
int bp = MAX_DATA_STACK_HEIGHT - 1; // sp - 1
int pc = 0;
instruction ir;
int haltFlag = 1;
int saoFlag = 0;
int activationRecordFlag = 0;

// Function prototypes
int base(int l, int base);
void fetch();
void execute(int OP, int L, int M);
int readInFile();
void outputOPName(int op);
void outputAssemblyLanguage(int count);
void outputExecution(int count);
void outputAll();
void outputStack();

// Main
void main_vmachine(void)
{
    outputAll();
}

// Helper function to assist in finding a variable in a different Activation
// Record some L levels down
int base(int l, int base) // l stand for L in the instruction format
{ 
    int b1; //find base L levels down
    b1 = base; 

    while (l > 0)
    {
        b1 = stack[b1 - 1];   
        l--;
    }

    return b1;
}

// Fetch instruction from text segment, place it in IR, increment PC
void fetch()
{
    ir = instructionSet[pc];
    pc += 1;
}

// Instruction set architecture. (Executes instruction)
void execute(int OP, int L, int M)
{
    // Switch case for operations
    switch (OP)
    {
        // LIT
        case (1):
            sp -= 1;
            stack[sp] = M;
            break;
        // OPR
        case (2):
            switch(M)
            {
                // RET
                case (0):
                    sp = bp + 1;
                    pc = stack[sp - 4];
                    bp = stack[sp - 3];
                    break;
                // NEG
                case (1):
                    stack[sp] = stack[sp] * (-1);
                    break;
                // ADD
                case (2):
                    sp += 1;
                    stack[sp] += stack[sp - 1];
                    break;
                // SUB
                case (3):
                    sp += 1;
                    stack[sp] -= stack[sp - 1];
                    break;
                // MUL
                case (4):
                    sp += 1;
                    stack[sp] *= stack[sp - 1];
                    break;
                // DIV
                case (5):
                    sp += 1;
                    stack[sp] /= stack[sp - 1];
                    break;
                // ODD
                case (6):
                    stack[sp] %= 2;
                    break;
                // MOD
                case (7):
                    sp += 1;
                    stack[sp] %= stack[sp - 1];
                    break;
                // EQL
                case (8):
                    sp += 1;
                    stack[sp] = stack[sp] == stack[sp - 1];
                    break;
                // NEQ
                case (9):
                    sp += 1;
                    stack[sp] = stack[sp] != stack[sp - 1];
                    break;
                // LSS
                case (10):
                    sp += 1;
                    stack[sp] = stack[sp] < stack[sp - 1];
                    break;
                // LEQ
                case (11):
                    sp += 1;
                    stack[sp] = stack[sp] <= stack[sp - 1];
                    break;
                // GTR
                case (12):
                    sp += 1;
                    stack[sp] = stack[sp] > stack[sp - 1];
                    break;
                // GEQ
                case (13):
                    sp += 1;
                    stack[sp] = stack[sp] >= stack[sp - 1];
                    break;
            }
            break;
        // LOD
        case (3):
            sp -= 1;
            stack[sp] = stack[base(L, bp) - M];
            break;
        // STO
        case (4):
            stack[base(L, bp) - M] = stack[sp];
            sp += 1;
            break;
        // CAL
        case (5):
            // Activation Record Flag
            activationRecordFlag = 1;
            // Space to return value
            stack[sp - 1] = 0;
            // Static link (SL)
            stack[sp - 2] = base(L, bp);
            // Dynamic link (DL)
            stack[sp - 3] = bp;
            // Return address (RA)
            stack[sp - 4] = pc;
            bp = sp - 1;
            pc = M;
            break;
        // INC
        case (6):
            sp -= M;
            break;
        // JMP
        case (7):
            pc = M;
            break;
        // JPC
        case (8):
            if (stack[sp] == 0)
                pc = M;
            sp += 1;
            break;
        // SIO 1
        case (9):
            printf("%d", stack[sp]);
            sp += 1;
            break;
        // SIO 2
        case (10):
            sp -= 1;
            printf("Please enter value.\n");
            scanf("%d", &stack[sp]);
            break;
        // SIO 3
        case (11):
            saoFlag = 1;
            haltFlag = 0;
            break;
    }
}

// Reads in input file data and returns the number of new lines that the file contains
int readInFile()
{
    // Count variable
    int i = 0;
    
    // Open file read only
    ifp = fopen("parser_output.txt", "r");

    // Null check if file failed to open
    if (ifp == NULL)
    {
        printf("Error opening file.\n");
        fclose(ifp);
    }

    // Increment count and scan in data
    while (fscanf(ifp, "%d %d %d", &instructionSet[i].op, &instructionSet[i].l, &instructionSet[i].m) != EOF)
        i += 1;

    // Close file to prevent leaks
    fclose(ifp);

    // Returns number of lines in input txt file
    return i;
}

// Calls readInFile() to take in instructions, executes them, and prints out current state
void outputAll()
{
    // Declare count variable (number of instruction lines) and read it in
    int count;
    count = readInFile();

    // Open file to write
    ofp = fopen("pmvm_output.txt", "w");

    // Call function to print interpreted assembly language output
    outputAssemblyLanguage(count);

    // Call function to print execution of the program in the virtual machine
    outputExecution(count);

    // Closes file
    fclose(ofp);
}

// Outputs text for assembly language results
void outputAssemblyLanguage(int count)
{
    // Prints different sections
    fprintf(ofp, "Line\tOP\t L\t M\n");

    // Iterates through each line 
    int i;
    for (int i = 0; i < count; i++)
    {
        fprintf(ofp, "%2d\t\t", i);
        outputOPName(instructionSet[i].op);
        fprintf(ofp, "\t%2d", instructionSet[i].l);
        fprintf(ofp, "\t%2d\n", instructionSet[i].m);
    }
}

// Outputs text for execution of program in virtual machine
void outputExecution(int count)
{
    int prevPC;

    // Prints headers and initial values
    fprintf(ofp, "\n\t\t\t\t\t\t\t\tpc\tbp\tsp\t\tstack\n");
    fprintf(ofp, "Initial values\t%d\t\t%d\t%d\t\t", pc, bp, sp);
    outputStack();

    // While halt is not triggered..
    while (haltFlag != 0)
    {
        // Store value of pc before it changes
        prevPC = pc;

        // Calls fetch and execute functions
        fetch();
        execute(ir.op, ir.l, ir.m);

        // For each new row, it prints previous and current values
        fprintf(ofp, "%2d ", prevPC);
        outputOPName(ir.op);
        fprintf(ofp, " %d %d\t\t\t", ir.l, ir.m);
        fprintf(ofp, "%2d\t%2d\t%2d\t\t", pc, bp, sp);
        if (saoFlag == 0)
          outputStack();
    }

}

void outputStack()
{
    // Prints current state of stack
    for (int i = 999; i >= sp; i--)
    {
        // Checks for new activation record
        if ((activationRecordFlag == 1) && (i == 993))
        {
            fprintf(ofp, " | ");
        }
        
        fprintf(ofp, "%d ", stack[i]);
    }
    
    fprintf(ofp, "\n");
}

// Prints actual name of OP
void outputOPName(int op)
{
    switch(op)
    {
        // LIT
        case (1):
            fprintf(ofp, "lit");
            break;
        // OPR
        case (2):
            fprintf(ofp, "opr");
            break;
        // LOD
        case (3):
            fprintf(ofp, "lod");
            break;
        // STO
        case (4):
            fprintf(ofp, "sto");
            break;
        // CAL
        case (5):
            fprintf(ofp, "cal");
            break;
        // INC
        case (6):
            fprintf(ofp, "inc");
            break;
        // JMP
        case (7):
            fprintf(ofp, "jmp");
            break;
        // JPC
        case (8):
            fprintf(ofp, "jpc");
            break;
        // SIO 01
        case (9):
            fprintf(ofp, "sio");
            break;
        // SIO 02
        case (10):
            fprintf(ofp, "sio");
            break;
        // SIO 03
        case (11):
            fprintf(ofp, "sio");
            break;
    }
}