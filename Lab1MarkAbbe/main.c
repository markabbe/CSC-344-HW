#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_CELL_BLANK 'B'
#define TAPE_CELL_BEGIN 'A'
#define TAPE_CELL_SIZE 100

// Define a struct for representing a tape cell
struct Cell {
    char value;
    struct Cell* prev;
    struct Cell* next;
};

// Define a struct for representing an instruction
struct Instruction {
    int currentState;
    char readVal;
    char writeVal;
    char moveDirection; // 'L' for left, 'R' for right
    int newState;
};

// Function to initialize the tape with input
struct Cell* initializeTape(char* input) {
    // Create a tape with 'A' as the first cell
    struct Cell* tape = (struct Cell*)malloc(sizeof(struct Cell));
    tape->value = TAPE_CELL_BEGIN;
    tape->prev = NULL;
    tape->next = NULL;

    // Add input to the tape
    struct Cell* current = tape;
    for (int i = 0; input[i] != '\0'; i++) {
        struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
        newCell->value = input[i];
        newCell->prev = current;
        newCell->next = NULL;
        current->next = newCell;
        current = newCell;
    }

    return tape;
}

// Function to execute a step based on the current instruction
void executeStep(struct Cell** tape, struct Instruction* instructions, int* currentState, int numInstructions, struct Cell** head) {
    char currentValue = (*head)->value;
    int i;

    for (i = 0; i < numInstructions; i++) {
        if (instructions[i].currentState == *currentState && instructions[i].readVal == currentValue) {
            // Execute the instruction
            (*head)->value = instructions[i].writeVal;

            if (instructions[i].moveDirection == 'L') {
                if ((*head)->prev == NULL) {
                    // Create a new cell on the left if necessary
                    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
                    newCell->value = TAPE_CELL_BLANK;
                    newCell->prev = NULL;
                    newCell->next = *head;
                    (*head)->prev = newCell;
                    *head = newCell;
                } else {
                    *head = (*head)->prev;
                }
            } else if (instructions[i].moveDirection == 'R') {
                if ((*head)->next == NULL) {
                    // Create a new cell on the right if necessary
                    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
                    newCell->value = TAPE_CELL_BLANK;
                    newCell->prev = *head;
                    newCell->next = NULL;
                    (*head)->next = newCell;
                } else {
                    *head = (*head)->next;
                }
            }

            *currentState = instructions[i].newState;
            break;
        }
    }

    // If no instruction is found, halt
    if (i == numInstructions) {
        printf("Halt.\n");
        exit(0);
    }
}

int main() {
    // Define the input tape and instructions here
    char initialTape[] = "111B1111";

    struct Instruction instructions[] = {
            {0, 'A', 'A', 'R', 1},
            {1, '1', '1', 'R', 1},
            {1, 'B', '1', 'R', 2},
            {2, '1', '1', 'R', 2},
            {2, 'B', 'B', 'L', 3},
            {3, '1', 'B', 'L', 4},
            {4, '1', '1', 'L', 4},
            {4, 'A', 'A', 'R', 5},
    };

    int numInstructions = sizeof(instructions) / sizeof(instructions[0]);
    int numStates = 6; // Total number of states
    int startState = 0;
    int endState = 5;

    // Initialize the tape
    struct Cell* tape = initializeTape(initialTape);

    // Initialize the read/write head to the leftmost cell
    struct Cell* head = tape;

    // Start executing the Turing Machine
    int currentState = startState;
    while (currentState != endState) {
        // Print the current state and tape
        printf("State %d\n", currentState);
        struct Cell* current = tape;
        while (current != NULL) {
            if (current == head) {
                printf("^");
            } else {
                printf(" ");
            }
            printf("%c", current->value);
            current = current->next;
        }
        printf("\n");

        // Execute a step
        executeStep(&tape, instructions, &currentState, numInstructions, &head);
    }

    // Print the final tape contents
    printf("\nFinal tape contents: ");
    struct Cell* current = tape;
    while (current != NULL) {
        printf("%c", current->value);
        current = current->next;
    }
    printf("\n");

    // Free the memory used by the tape
    current = tape;
    while (current != NULL) {
        struct Cell* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
