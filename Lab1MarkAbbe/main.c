//Mark Abbe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define TAPE_CELL_BLANK 'B'
#define TAPE_CELL_BEGIN 'A'
#define TAPE_CELL_SIZE 100

struct Cell {
    char value;
    struct Cell* prev;
    struct Cell* next;
};

struct Instruction {
    int currentState;
    char readVal;
    char writeVal;
    char moveDirection;
    int newState;
};

struct TuringMachine {
    struct Cell* tape;
    int numStates;
    int startState;
    int endState;
    struct Instruction* instructions;
};

// Function to initialize the Turing machine
void initializeTuringMachine(struct TuringMachine* tm, const char* filename) {
    // Debug: Print the filename
    printf("%s\n\n", filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char initialTape[TAPE_CELL_SIZE];
    fscanf(file, "%s", initialTape);

    // Read number of states, start state, and end state
    fscanf(file, "%d", &tm->numStates);
    fscanf(file, "%d", &tm->startState);
    fscanf(file, "%d", &tm->endState);

    // Read and parse instructions
    int numInstructions;
    fscanf(file, "%d", &numInstructions);
    tm->instructions = (struct Instruction*)malloc(numInstructions * sizeof(struct Instruction));
    for (int i = 0; i < numInstructions; i++) {
        char buf[100];
        fscanf(file, "%s", buf);
        sscanf(buf, "(%d,%c)->(%c,%c,%d)",
               &tm->instructions[i].currentState,
               &tm->instructions[i].readVal,
               &tm->instructions[i].writeVal,
               &tm->instructions[i].moveDirection,
               &tm->instructions[i].newState);
    }

    fclose(file);

    tm->tape = (struct Cell*)malloc(sizeof(struct Cell));
    tm->tape->value = TAPE_CELL_BEGIN;
    tm->tape->prev = NULL;
    tm->tape->next = NULL;

    // Add input to the tape
    struct Cell* current = tm->tape;
    for (int i = 0; initialTape[i] != '\0'; i++) {
        struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
        newCell->value = initialTape[i];
        newCell->prev = current;
        newCell->next = NULL;
        current->next = newCell;
        current = newCell;
    }
}

// Function to execute one step of the Turing machine
void executeStep(struct TuringMachine* tm) {
    char currentValue = tm->tape->value;
    int i;

    for (i = 0; i < tm->numStates; i++) {
        if (tm->instructions[i].currentState == tm->startState && tm->instructions[i].readVal == currentValue) {
            // Execute the instruction
            tm->tape->value = tm->instructions[i].writeVal;

            if (tm->instructions[i].moveDirection == 'L') {
                if (tm->tape->prev == NULL) {
                    // Create new cell on the left
                    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
                    newCell->value = TAPE_CELL_BLANK;
                    newCell->prev = NULL;
                    newCell->next = tm->tape;
                    tm->tape->prev = newCell;
                    tm->tape = newCell;
                }
                else {
                    tm->tape = tm->tape->prev;
                }
            }
            else if (tm->instructions[i].moveDirection == 'R') {
                if (tm->tape->next == NULL) {
                    // Create new cell on the right
                    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
                    newCell->value = TAPE_CELL_BLANK;
                    newCell->prev = tm->tape;
                    newCell->next = NULL;
                    tm->tape->next = newCell;
                }
                else {
                    tm->tape = tm->tape->next;
                }
            }

            tm->startState = tm->instructions[i].newState;
            return; // Exit the function after executing one instruction
        }
    }

    // If no instruction is found then halt
    printf("Halt.\n");
    exit(0);
}

int main() {
    struct TuringMachine tm;
    char input[256];
    char filename[256];

    printf("Enter the filename (make sure the file is located in the root directory of the project): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (getcwd(filename, sizeof(filename)) == NULL) {
        perror("getcwd");
        return 1;
    }

    // Construct the full file path by appending ../ to the user-provided filename
    strncat(filename, "/../", sizeof(filename) - strlen(filename) - 1);
    strncat(filename, input, sizeof(filename) - strlen(filename) - 1);

    initializeTuringMachine(&tm, filename);

    struct Cell* head = tm.tape;

    // Print initial tape contents
    printf("Initial tape contents: ");
    struct Cell* current = tm.tape;
    while (current != NULL) {
        printf("%c", current->value);
        current = current->next;
    }
    printf("\n");

    while (1) {
        // Print current state and tape
        printf("State %d\n", tm.startState);
        current = tm.tape;
        while (current != NULL) {
            if (current == head) {
                printf("^");
            }
            else {
                printf(" ");
            }
            printf("%c", current->value);
            current = current->next;
        }
        printf("\n");

        // Execute a step
        executeStep(&tm);

        // Check if reached the end state
        if (tm.startState == tm.endState) {
            break;
        }
    }

    // Not sure why this doesnt work?
    printf("\nFinal tape contents: ");
    current = tm.tape;
    while (current != NULL && current->value != TAPE_CELL_BLANK) {
        printf("%c", current->value);
        current = current->next;
    }
    printf("\n");

    // Free the memory used by the tape and instructions
    current = tm.tape;
    while (current != NULL) {
        struct Cell* temp = current;
        current = current->next;
        free(temp);
    }
    free(tm.instructions);

    return 0;
}