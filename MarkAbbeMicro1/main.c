#include <stdio.h>
#include <string.h>

#define MAX_SHELVES 10
#define MAX_SLOTS 10
#define MAX_NAME_LENGTH 50

struct Item {
    char name[MAX_NAME_LENGTH];
    float price;
};

// Function to trim leading and trailing whitespace and newlines from a string
void trim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
    int start = 0;
    while (str[start] == ' ' || str[start] == '\n' || str[start] == '\r') {
        start++;
    }
    if (start > 0) {
        for (int i = start; i <= len; i++) {
            str[i - start] = str[i];
        }
    }
}

int main() {
    int numShelves, numSlots;

    printf("Enter the number of shelves: ");
    scanf("%d", &numShelves);

    while (numShelves <= 0 || numShelves > MAX_SHELVES) {
        printf("Invalid number of shelves. Maximum allowed is %d.\n", MAX_SHELVES);
        printf("Enter the number of shelves: ");
        scanf("%d", &numShelves);
    }

    printf("Enter the number of slots per shelf: ");
    scanf("%d", &numSlots);

    while (numSlots <= 0 || numSlots > MAX_SLOTS) {
        printf("Invalid number of slots per shelf. Maximum allowed is %d.\n", MAX_SLOTS);
        printf("Enter the number of slots per shelf: ");
        scanf("%d", &numSlots);
    }

    // Consume the newline character in the input buffer
    getchar();

    struct Item shelf[MAX_SHELVES][MAX_SLOTS];

    for (int i = 0; i < numShelves; i++) {
        for (int j = 0; j < numSlots; j++) {
            shelf[i][j].name[0] = '\0';
        }
    }

    printf("Enter items in the format: <name>, <price>, <shelf>, <slot>\n");
    printf("Type 'done' when you are finished.\n");

    char input[100];
    while (1) {
        printf("Enter an item or 'done': ");
        fgets(input, sizeof(input), stdin);

        // Trim leading and trailing whitespace and newlines
        trim(input);

        // Check for 'done'
        if (strcmp(input, "done") == 0) {
            break;
        }

        char name[MAX_NAME_LENGTH];
        float price;
        int shelfIndex, slotIndex;

        if (sscanf(input, "%49[^,], %f, %d, %d", name, &price, &shelfIndex, &slotIndex) == 4) {
            if (shelfIndex >= 0 && shelfIndex < numShelves && slotIndex >= 0 && slotIndex < numSlots) {
                struct Item newItem;
                strcpy(newItem.name, name);
                newItem.price = price;
                shelf[shelfIndex][slotIndex] = newItem;
                printf("Item added to shelf %d, slot %d.\n", shelfIndex, slotIndex);
            } else {
                printf("Invalid shelf or slot number.\n");
            }
        } else {
            printf("Invalid input format. Please use: <name>, <price>, <shelf>, <slot>\n");
        }
    }

    while (1) {
        int shelfIndex, slotIndex;
        printf("Enter the shelf and slot for the item you want to retrieve (e.g., 1 2): ");
        if (scanf("%d %d", &shelfIndex, &slotIndex) != 2) {
            printf("Invalid input format. Please use: <shelf> <slot>\n");
            continue;
        }

        if (shelfIndex >= 0 && shelfIndex < numShelves && slotIndex >= 0 && slotIndex < numSlots) {
            struct Item item = shelf[shelfIndex][slotIndex];
            if (item.name[0] != '\0') {
                printf("Item at shelf %d, slot %d:\n", shelfIndex, slotIndex);
                printf("Name: %s\n", item.name);
                printf("Price: %.2f\n", item.price);
            } else {
                printf("This slot is empty.\n");
            }
        } else {
            printf("Invalid shelf or slot number.\n");
        }

        printf("Enter 'done' to exit or 'continue' to search for another item: ");
        char choice[10];
        scanf("%9s", choice);
        if (strcmp(choice, "done") == 0) {
            break;
        }
    }

    return 0;
}
