#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BAG_ITEMS 10

struct Bag
{
    char name[30];
    char type[20];
    int amount;
};

int bagItemCounter = 0;

// === Prototype funcions ===
void addItem(struct Bag *bag);
void removeItem(struct Bag *bag);
void listBagItems(struct Bag *bag);
void showMenu(struct Bag *bag);

void clearEntryBuffer();

int main()
{
    struct Bag bag[MAX_BAG_ITEMS];

    printf("==============================\n");
    printf("  SURVIVOR BAG - ISLAND CODE  \n");
    printf("==============================\n");
    
    do
    {
        printf("Items amount: %d/%d\n", bagItemCounter, MAX_BAG_ITEMS);
        showMenu(bag);
    } while (1);
}

void clearEntryBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void showMenu(struct Bag *bag)
{
    int menuOption;

    printf("\n1. Add Item\n");
    printf("2. Remove Item\n");
    printf("3. Show Bag Items\n");
    printf("0. Exit\n");
    printf("------------------------------\n");
    printf("Choose an option: ");
    scanf("%d", &menuOption);
    clearEntryBuffer();

    switch (menuOption)
    {
        case 1:
            addItem(bag);
            break;

        case 2:
            removeItem(bag);
            break;

        case 3:
            listBagItems(bag);
            break;

        case 0:
            printf("\nExiting...\n");
            exit(0);
            break;

        default:
            printf("\nInvalid option!\n");
            break;
    }
}

void listBagItems(struct Bag *bag)
{
    printf("\n------ BAG ITEMS (%d/%d) -------\n", bagItemCounter, MAX_BAG_ITEMS);
    printf("--------------------------------------------\n");
    printf("NAME            | TYPE          | AMOUNT    \n");
    printf("--------------------------------------------\n");
    for (int index = 0; index < bagItemCounter; index++)
    {
        printf("%s          | %s            | %d    \n", bag[index].name, bag[index].type, bag[index].amount);
    }
    printf("--------------------------------------------\n");

    printf("\nPress any key to continue...");
    getchar();

    return;
}

void addItem(struct Bag *bag)
{
    if (bagItemCounter >= MAX_BAG_ITEMS)
    {
        printf("\nThe bag is full! Remove something first.\n");
        return;
    }

    struct Bag newItem;

    printf("\nEnter item name: ");
    fgets(newItem.name, sizeof(newItem.name), stdin);
    newItem.name[strcspn(newItem.name, "\n")] = '\0'; // remove \n

    printf("Enter item type: ");
    fgets(newItem.type, sizeof(newItem.type), stdin);
    newItem.type[strcspn(newItem.type, "\n")] = '\0';

    printf("Enter item amount: ");
    scanf("%d", &newItem.amount);
    clearEntryBuffer();

    bag[bagItemCounter] = newItem;
    bagItemCounter++;

    printf("\nItem added successfully!\n");
}

void removeItem(struct Bag *bag)
{
    if (bagItemCounter == 0)
    {
        printf("\nThe bag is empty!\n");
        return;
    }

    char nameToRemove[30];
    printf("\nEnter the item name to remove: ");
    fgets(nameToRemove, sizeof(nameToRemove), stdin);
    nameToRemove[strcspn(nameToRemove, "\n")] = '\0';

    int found = -1;

    for (int i = 0; i < bagItemCounter; i++)
    {
        if (strcmp(bag[i].name, nameToRemove) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\nItem not found!\n");
        return;
    }

    for (int i = found; i < bagItemCounter - 1; i++)
    {
        bag[i] = bag[i + 1];
    }

    bagItemCounter--;
    printf("\nItem removed successfully!\n");
}
