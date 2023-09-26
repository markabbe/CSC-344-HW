#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 1024
#define NUM_STATES 10

enum Direction {
    LEFT = -1,
    STAY = 0,
    RIGHT = 1,
};

struct TMInstruction {
    int newState;
    char writeSymbol;
    enum Direction moveDirection;
};

struct TuringMachine {
    char tape[TAPE_SIZE];
    int head;
    int currentState;
    struct TMInstruction instructions[NUM_STATES][256];
};

void initializeTuringMachine(struct TuringMachine* tm) {
    memset(tm->tape, 'B', sizeof(tm->tape));
    tm->head = 0;
    tm->currentState = 0;
}

void loadInstructions(struct TuringMachine* tm, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[256];
    int lineCount = 0; // Added for debugging
    while (fgets(line, sizeof(line), file) != NULL) {
        int fromState, toState;
        char readSymbol, writeSymbol, move;

        // Debugging: Print the line being processed
        printf("Line %d: %s", ++lineCount, line);

        // Check if the line is a comment (starting with #) and skip it
        if (line[0] == '#') {
            continue;
        }

        // Attempt to parse the instruction line
        if (sscanf(line, "(%d,%c)->(%c,%c,%d)", &fromState, &readSymbol, &writeSymbol, &move, &toState) == 5) {
            // Ensure valid state and symbol values
            if (fromState >= 0 && fromState < NUM_STATES && (int)readSymbol >= 0 && (int)readSymbol < 256) {
                tm->instructions[fromState][(int)readSymbol].newState = toState;
                tm->instructions[fromState][(int)readSymbol].writeSymbol = writeSymbol;

                // Map the character direction to the enum Direction
                if (move == 'L') {
                    tm->instructions[fromState][(int)readSymbol].moveDirection = LEFT;
                } else if (move == 'R') {
                    tm->instructions[fromState][(int)readSymbol].moveDirection = RIGHT;
                } else if (move == 'S') {
                    tm->instructions[fromState][(int)readSymbol].moveDirection = STAY;
                } else {
                    fprintf(stderr, "Invalid move direction in line %d: %c\n", lineCount, move);
                    exit(1);
                }
            }
        }
    }

    fclose(file);
}

void runTuringMachine(struct TuringMachine* tm) {
    while (1) {
        int state = tm->currentState;
        char symbol = tm->tape[tm->head];
        struct TMInstruction instruction = tm->instructions[state][(int)symbol];
        if (instruction.newState == -1) {
            printf("No more instructions for state %d and symbol %c. Halting.\n", state, symbol);
            break;
        }

        tm->tape[tm->head] = instruction.writeSymbol;
        tm->currentState = instruction.newState;
        tm->head += instruction.moveDirection;

        // Ensure tape bounds are not violated
        if (tm->head < 0) {
            perror("Tape out of bounds (left)");
            exit(1);
        } else if (tm->head >= TAPE_SIZE) {
            perror("Tape out of bounds (right)");
            exit(1);
        }

        // Debugging: Print the current state of the tape and head position
        printf("State: %d, Symbol: %c, New State: %d, Write: %c, Move: %d\n", state, symbol, instruction.newState, instruction.writeSymbol, instruction.moveDirection);
        printf("Tape: %s\n", tm->tape);
    }
}

int main() {
    struct TuringMachine tm;
    initializeTuringMachine(&tm);

    char input[256];
    char filename[256];
    printf("Enter the filename (make sure it's located in the project's root directory): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input filename\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    snprintf(filename, sizeof(filename), "../%s", input);

    // Print the filename before attempting to open it
    printf("Opening file: %s\n", filename);

    loadInstructions(&tm, filename);
    runTuringMachine(&tm);

    printf("Final tape contents: %s\n", tm.tape);

    return 0;
}
