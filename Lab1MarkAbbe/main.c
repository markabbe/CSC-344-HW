#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct TapeCell{
    char value;
    struct TapeCell* prev;
    struct TapeCell* next;
};

struct TMInstruction{
    int currentState;
    char readVal;
    char writeVal;
    char moveDirection;
    int newState;
};

#define MAX_NUM_STATES 10
#define MAX_LINE_LENGTH 256

//*********************************************************************************************************************

int main()
{
    FILE *file = fopen("../input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    char initialTape[MAX_LINE_LENGTH];
    int numStates, startState, endState;

    if (fgets(initialTape, sizeof(initialTape), file) == NULL)
    {
        perror("Error reading initial tape content");
        fclose(file);
        return 1;
    }

    if (fscanf(file, "%d\n%d\n%d\n", &numStates, &startState, &endState) != 3)
    {
        perror("Error reading machine parameters");
        fclose(file);
        return 1;
    }

    struct TMInstruction instructions[MAX_NUM_STATES][128];
    int currentState, newState;
    char readVal, writeVal, moveDirection;
    while (fscanf(file, "(%d,%c)->(%c,%c,%d)\n", &currentState, &readVal, &writeVal, &moveDirection, &newState) == 5)
    {
        instructions[currentState][(int)readVal] = (struct TMInstruction){currentState, readVal, writeVal, moveDirection, newState};
    }

    fclose(file);

    // Print the values you read for testing purposes
    printf("Initial Tape Content: %s\n", initialTape);
    printf("Number of States: %d\n", numStates);
    printf("Start State: %d\n", startState);
    printf("End State: %d\n", endState);

    struct TapeCell* tape = (struct TapeCell*)malloc(sizeof(struct TapeCell));
    if (tape == NULL)
    {
        perror("Error allocating memory for tape");
        return 1;
    }
    tape->value = 'A';
    tape->prev = NULL;
    tape->next = NULL;

    struct TapeCell* head = tape;

    while (currentState != endState)
    {
        char currentSymbol = head->value;

        struct TMInstruction instruction = instructions[currentState][(int)currentSymbol];

        head->value = instruction.writeVal;

        if (instruction.moveDirection == 'R')
        {
            if (head->next == NULL)
            {
                struct TapeCell* newCell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
                if (newCell == NULL)
                {
                    perror("Error allocating memory for tape cell");
                    return 1;
                }
                newCell->value = 'B';
                newCell->prev = head;
                newCell->next = NULL;
                head->next = newCell;
            }
            head = head->next;
        }
        else if (instruction.moveDirection == 'L')
        {
            if (head->prev == NULL)
            {
                struct TapeCell* newCell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
                if (newCell == NULL)
                {
                    perror("Error allocating memory for tape cell");
                    return 1;
                }
                newCell->value = 'B';
                newCell->prev = NULL;
                newCell->next = head;
                head->prev = newCell;
                tape = newCell;
            }
            head = head->prev;
        }

        currentState = instruction.newState;
    }

    printf("Final tape contents: ");
    struct TapeCell* currentCell = tape;
    while (currentCell != NULL)
    {
        printf("%c", currentCell->value);
        currentCell = currentCell->next;
    }
    printf("\n");

    while (tape != NULL)
    {
        struct TapeCell* temp = tape;
        tape = tape->next;
        free(temp);
    }

    return 0;
}
