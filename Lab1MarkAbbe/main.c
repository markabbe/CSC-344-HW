#include <stdio.h>
#include <string.h>

struct Cell {
    char value;
    struct Cell* next;
    struct Cell* prev;
};

struct TuringMachine {
    struct Cell* tape; // Pointer to the tape
    struct Cell* head; // Pointer to the current cell
    int state;         // Current state
    int num_states;    // Total number of states
    int start_state;   // Start state
    int end_state;     // End state
    struct Instruction** instruction_table; // 2D array for instructions
};

struct Instruction {
    char read_val;
    char write_val;
    char move_direction; // 'L' for left, 'R' for right
    int new_state;
};

int main() {

    return 0;
}
