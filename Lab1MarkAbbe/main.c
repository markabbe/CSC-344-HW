#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLANK 'B'
#define START_SYMBOL 'A'
#define MAX_NUM_STATES 10
#define ASCII_SIZE 128

struct TapeCell {
    char value;
    struct TapeCell* prev;
    struct TapeCell* next;
};

struct TMInstruction {
    char writeVal;
    char moveDirection;
    int newState;
};

// Function to allocate a new tape cell
struct TapeCell* createTapeCell(char value) {
    struct TapeCell* cell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
    if (cell == NULL) {
        fprintf(stderr, "Error allocating memory for tape cell\n");
        exit(1);
    }
    cell->value = value;
    cell->prev = NULL;
    cell->next = NULL;
    return cell;
}

// Function to free the tape cells
void freeTape(struct TapeCell* tape) {
    while (tape != NULL) {
        struct TapeCell* temp = tape;
        tape = tape->next;
        free(temp);
    }
}

int main() {
    char input[256];
    char filename[256];
    printf("Enter the filename (make sure it's located in the project's root directory): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    snprintf(filename, sizeof(filename), "../%s", input);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    char initialTape[256];
    int numStates, startState, endState;

    if (fgets(initialTape, sizeof(initialTape), file) == NULL) {
        fprintf(stderr, "Error reading initial tape content\n");
        fclose(file);
        return 1;
    }

    if (fscanf(file, "%d", &numStates) != 1) {
        fprintf(stderr, "Error reading number of states\n");
        fclose(file);
        return 1;
    }

    if (fscanf(file, "%d", &startState) != 1) {
        fprintf(stderr, "Error reading start state\n");
        fclose(file);
        return 1;
    }

    if (fscanf(file, "%d", &endState) != 1) {
        fprintf(stderr, "Error reading end state\n");
        fclose(file);
        return 1;
    }

    fgetc(file); // Consume the newline character

    if (numStates < 1 || numStates > MAX_NUM_STATES) {
        fprintf(stderr, "Invalid number of states\n");
        fclose(file);
        return 1;
    }

    struct TMInstruction instructions[MAX_NUM_STATES][ASCII_SIZE];
    int currentState, newState;
    char readVal, writeVal, moveDirection;

    for (int i = 0; i < MAX_NUM_STATES; i++) {
        for (int j = 0; j < ASCII_SIZE; j++) {
            instructions[i][j].writeVal = BLANK;
            instructions[i][j].moveDirection = 'S';
            instructions[i][j].newState = -1;
        }
    }

    char instructionLine[256];
    while (fgets(instructionLine, sizeof(instructionLine), file) != NULL) {
        if (instructionLine[0] == '(') {
            sscanf(instructionLine, "(%d,%c)->(%c,%c,%d)", &currentState, &readVal, &writeVal, &moveDirection, &newState);

            if (currentState < 0 || currentState >= numStates ||
                readVal < 0 || readVal >= ASCII_SIZE ||
                writeVal < 0 || writeVal >= ASCII_SIZE ||
                (moveDirection != 'L' && moveDirection != 'R' && moveDirection != 'S') ||
                newState < 0 || newState >= numStates) {
                fprintf(stderr, "Invalid input in the instruction table\n");
                fclose(file);
                return 1;
            }

            instructions[currentState][(int)readVal].writeVal = writeVal;
            instructions[currentState][(int)readVal].moveDirection = moveDirection;
            instructions[currentState][(int)readVal].newState = newState;
        }
    }

    fclose(file);

    // Initialize the tape with the initial tape content
    struct TapeCell* tape = createTapeCell(START_SYMBOL);
    struct TapeCell* head = tape;

    // Skip the first character (START_SYMBOL 'A')
    for (int i = 1; i < strlen(initialTape); i++) {
        if (initialTape[i] != '\n') {
            head->value = initialTape[i];
            if (initialTape[i + 1] != '\0') {
                struct TapeCell* newCell = createTapeCell(BLANK);
                head->next = newCell;
                newCell->prev = head;
                head = newCell;
            }
        }
    }

    currentState = startState;
    while (currentState != endState) {
        char currentSymbol = head->value;
        struct TMInstruction instruction = instructions[currentState][(int)currentSymbol];
        head->value = instruction.writeVal;

        if (instruction.moveDirection == 'R') {
            if (head->next == NULL) {
                struct TapeCell* newCell = createTapeCell(BLANK);
                head->next = newCell;
                newCell->prev = head;
            }
            head = head->next;
        }
        else if (instruction.moveDirection == 'L') {
            if (head->prev == NULL) {
                struct TapeCell* newCell = createTapeCell(BLANK);
                newCell->next = head;
                head->prev = newCell;
                tape = newCell;
            }
            head = head->prev;
        }
        else if (instruction.moveDirection == 'S') {
            // Do nothing for 'S' (stay in place)
        }
        else {
            fprintf(stderr, "Invalid tape movement direction\n");
            fclose(file);
            freeTape(tape);
            return 1;
        }

        currentState = instruction.newState; // Updated the current state inside the loop
    }

    // Print the final tape contents, skipping the initial 'A' if present
    printf("Initial Tape Content: %s", initialTape);
    printf("Final tape contents: ");
    if (tape->value == START_SYMBOL) {
        tape = tape->next;
    }
    struct TapeCell* currentCell = tape;
    while (currentCell != NULL) {
        printf("%c", currentCell->value);
        currentCell = currentCell->next;
    }
    printf("\n");

    freeTape(tape); // Free allocated memory

    return 0;
}
