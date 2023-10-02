//Mark Abbe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cell Struct
typedef struct Cell {
    char value;
    struct Cell* next;
    struct Cell* previous;
} Cell;

// Instruction Struct
typedef struct {
    char writeVal;
    char moveDirection;
    int newState;
    int isSet; // if this instruction is set, then 1, otherwise 0
} Instruction;

/* createCell function which takes a char, and two cells which are
 * used as the previous and next cell */
Cell* createCell(char value, Cell* prev, Cell* next) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->value = value;
    cell->previous = prev;
    cell->next = next;
    return cell;
}

int main() {
    char filename[100];
    printf("Enter filename (make sure the file is in the cmake-build-debug folder): ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    // Initialize the variables and get their value from the input file
    char initialTape[100];
    int numStates, startState, endState;
    fscanf(file, "%s\n%d\n%d\n%d\n", initialTape, &numStates, &startState, &endState);
    printf("\n");
    printf("Initial tape contents: %s\n\n", initialTape);

    // Initialize the 2D array instructions, and set every byte of the array to 0 so
    Instruction instructions[numStates][128];
    memset(instructions, 0, sizeof(instructions));

    // While we are not at the end of the input file, get the current instruction, and append it to the instructions array
    while (!feof(file)) {
        int currentState;
        char readVal, writeVal, moveDirection;
        int newState;
        fscanf(file, "(%d,%c)->(%c,%c,%d)\n", &currentState, &readVal, &writeVal, &moveDirection, &newState);
        instructions[currentState][(int)readVal] = (Instruction){writeVal, moveDirection, newState, 1};
    }
    fclose(file);

    // Debug purposes: Print instructions
    printf("Instructions:\n");
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < 128; j++) {
            if (instructions[i][j].isSet) {
                printf("(%d,%c)->(%c,%c,%d)\n", i, (char)j, instructions[i][j].writeVal, instructions[i][j].moveDirection, instructions[i][j].newState);
            }
        }
    }

    printf("\n");

    // Create the initial tape
    Cell* tape = createCell('A', NULL, NULL);
    Cell* current = tape;
    for (int i = 0; initialTape[i] != '\0'; i++) {
        current->next = createCell(initialTape[i], current, NULL);
        current = current->next;
    }

    // Turing Machine Execution
    Cell* currentCell = tape;
    int currentState = startState;

    printf("Starting Turing machine...\n\n");
    while (currentState != endState) {
        printf("Current state: %d, Current value: %c\n", currentState, currentCell->value);
        Instruction instr = instructions[currentState][(int)currentCell->value];

        if (!instr.isSet) {
            if(currentCell->value == 'B'){
                currentState = endState; // Transition to the final state
                continue; // Skip other logic and move to the next instruction
            }
            printf("Error: No instruction found for (State %d, Value %c).\n", currentState, currentCell->value);
            return 1; // Exit with error code
        }


        printf("Fetched transition: (%d, %c) -> (%c, %c, %d)\n", currentState, currentCell->value, instr.writeVal, instr.moveDirection, instr.newState);
        printf("Write %c, Move %c, New State %d\n------------\n", instr.writeVal, instr.moveDirection, instr.newState);

        if (instr.writeVal == 'B' && currentCell->value != 'B') {
            currentCell->value = 'B';
            if (!currentCell->next) {
                currentCell->next = createCell('B', currentCell, NULL);
            }
            currentCell = currentCell->next;
            currentState = instr.newState;
            continue; // Skip other logic and move to the next instruction
        }

        currentCell->value = instr.writeVal;
        if (instr.moveDirection == 'R') {
            if (!currentCell->next) {
                currentCell->next = createCell('B', currentCell, NULL);
            }
            currentCell = currentCell->next;
        }
        else if (instr.moveDirection == 'L') {
            if (currentCell->previous) {
                currentCell = currentCell->previous;
            }
            else {
                printf("Error: Trying to move left at the beginning of the tape.\n");
                return 1;
            }
        }
        else {
            printf("Error: Invalid move direction.\n");
            return 1;
        }

        currentState = instr.newState;
    }

    // Print final tape contents
    printf("\n");
    printf("Final tape contents: ");
    currentCell = tape;
    while (currentCell) {
        printf("%c", currentCell->value);
        currentCell = currentCell->next;
    }
    printf("\n");

    // Clean up: deallocate memory
    while (tape) {
        Cell* temp = tape;
        tape = tape->next;
        free(temp);
    }

    return 0;
}