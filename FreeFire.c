#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BAG_ITEMS 10

// ====== Tipos e globais ======
typedef enum {
    SORT_BY_NAME = 1,
    SORT_BY_TYPE = 2,
    SORT_BY_PRIORITY = 3
} SortCriteria;

typedef struct {
    char name[30];
    char type[20];
    int amount;
    int priority; // 1 (baixa) .. 5 (alta)
} Bag;

int bagItemCounter = 0;
bool isSortedByName = false; // fica true somente após ordenar por nome e enquanto não houver mutações

// ====== Protótipos ======
void showMenu(Bag *bag);
void listBagItems(Bag *bag);
void addItem(Bag *bag);
void removeItem(Bag *bag);

void sortMenu(Bag *bag);
long insertionSort(Bag *bag, int n, SortCriteria crit); // retorna número de comparações
int binarySearchByName(Bag *bag, int n, const char *target, long *comparisons);

void binarySearchMenu(Bag *bag);

void clearEntryBuffer(void);

// ====== Main ======
int main(void)
{
    Bag bag[MAX_BAG_ITEMS];

    printf("==============================\n");
    printf("  SURVIVOR BAG - ISLAND CODE  \n");
    printf("=========== MESTRE ===========\n");

    while (1) {
        printf("\nItens: %d/%d | Ordenado por nome: %s\n",
               bagItemCounter, MAX_BAG_ITEMS, isSortedByName ? "SIM" : "NAO");
        showMenu(bag);
    }
}

// ====== Util ======
void clearEntryBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}

// ====== Menu Principal ======
void showMenu(Bag *bag)
{
    int opt;

    printf("\n1. Adicionar item\n");
    printf("2. Remover item\n");
    printf("3. Listar itens\n");
    printf("4. Ordenar (Insertion Sort)\n");
    printf("5. Buscar por nome (Busca Binaria)\n");
    printf("0. Sair\n");
    printf("------------------------------\n");
    printf("Escolha: ");

    if (scanf("%d", &opt) != 1) { clearEntryBuffer(); return; }
    clearEntryBuffer();

    switch (opt) {
        case 1: addItem(bag); break;
        case 2: removeItem(bag); break;
        case 3: listBagItems(bag); break;
        case 4: sortMenu(bag); break;
        case 5: binarySearchMenu(bag); break;
        case 0: printf("\nSaindo...\n"); exit(0);
        default: printf("\nOpcao invalida!\n");
    }
}

// ====== Listagem ======
void listBagItems(Bag *bag)
{
    printf("\n----------- ITENS (%d/%d) -----------\n", bagItemCounter, MAX_BAG_ITEMS);
    printf("%-18s | %-12s | %-7s | %-9s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < bagItemCounter; i++) {
        printf("%-18s | %-12s | %-7d | %-9d\n",
               bag[i].name, bag[i].type, bag[i].amount, bag[i].priority);
    }
    printf("-----------------------------------------------\n");
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ====== Adicao ======
void addItem(Bag *bag)
{
    if (bagItemCounter >= MAX_BAG_ITEMS) {
        printf("\nA mochila esta cheia! Remova algo antes.\n");
        return;
    }

    Bag newItem;

    printf("\nNome do item: ");
    if (!fgets(newItem.name, sizeof(newItem.name), stdin)) return;
    newItem.name[strcspn(newItem.name, "\n")] = '\0';

    printf("Tipo do item: ");
    if (!fgets(newItem.type, sizeof(newItem.type), stdin)) return;
    newItem.type[strcspn(newItem.type, "\n")] = '\0';

    printf("Quantidade: ");
    if (scanf("%d", &newItem.amount) != 1) { clearEntryBuffer(); printf("Quantidade invalida.\n"); return; }
    clearEntryBuffer();

    printf("Prioridade (1=baixa .. 5=alta): ");
    if (scanf("%d", &newItem.priority) != 1) { clearEntryBuffer(); printf("Prioridade invalida.\n"); return; }
    clearEntryBuffer();

    if (newItem.priority < 1) newItem.priority = 1;
    if (newItem.priority > 5) newItem.priority = 5;

    bag[bagItemCounter++] = newItem;
    isSortedByName = false; // lista perdeu garantia de ordenacao por nome
    printf("\nItem adicionado!\n");
}

// ====== Remocao ======
void removeItem(Bag *bag)
{
    if (bagItemCounter == 0) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    char nameToRemove[30];
    printf("\nNome do item a remover: ");
    if (!fgets(nameToRemove, sizeof(nameToRemove), stdin)) return;
    nameToRemove[strcspn(nameToRemove, "\n")] = '\0';

    int found = -1;
    for (int i = 0; i < bagItemCounter; i++) {
        if (strcmp(bag[i].name, nameToRemove) == 0) { found = i; break; }
    }

    if (found == -1) {
        printf("\nItem nao encontrado!\n");
        return;
    }

    for (int i = found; i < bagItemCounter - 1; i++) {
        bag[i] = bag[i + 1];
    }
    bagItemCounter--;
    isSortedByName = false; // apos remocao, nao garantimos mais ordenacao por nome
    printf("\nItem removido!\n");
}

// ====== Menu de Ordenacao ======
void sortMenu(Bag *bag)
{
    if (bagItemCounter <= 1) {
        printf("\nNada para ordenar.\n");
        return;
    }

    int opt;
    printf("\nOrdenar por:\n");
    printf("1. Nome\n");
    printf("2. Tipo\n");
    printf("3. Prioridade (crescente 1..5)\n");
    printf("Escolha: ");
    if (scanf("%d", &opt) != 1) { clearEntryBuffer(); return; }
    clearEntryBuffer();

    if (opt < 1 || opt > 3) {
        printf("Criterio invalido.\n");
        return;
    }

    SortCriteria crit = (SortCriteria)opt;
    long comparisons = insertionSort(bag, bagItemCounter, crit);
    isSortedByName = (crit == SORT_BY_NAME);

    const char *critStr = (crit == SORT_BY_NAME) ? "nome" :
                          (crit == SORT_BY_TYPE) ? "tipo" : "prioridade";
    printf("\nItens ordenados por %s. Comparacoes realizadas: %ld\n", critStr, comparisons);
}

// ====== Insertion Sort com contador ======
static int cmpForCriteria(const Bag *a, const Bag *b, SortCriteria crit, long *comparisons)
{
    (*comparisons)++;
    switch (crit) {
        case SORT_BY_NAME:     return strcmp(a->name, b->name);
        case SORT_BY_TYPE:     return strcmp(a->type, b->type);
        case SORT_BY_PRIORITY: return (a->priority - b->priority);
        default:               return 0;
    }
}

long insertionSort(Bag *bag, int n, SortCriteria crit)
{
    long comparisons = 0;
    for (int i = 1; i < n; i++) {
        Bag key = bag[i];
        int j = i - 1;

        // Move elementos MAIORES que key uma posicao a frente (ordem crescente)
        while (j >= 0 && cmpForCriteria(&bag[j], &key, crit, &comparisons) > 0) {
            bag[j + 1] = bag[j];
            j--;
        }
        bag[j + 1] = key;
    }
    return comparisons;
}

// ====== Busca Binaria por Nome ======
int binarySearchByName(Bag *bag, int n, const char *target, long *comparisons)
{
    int left = 0, right = n - 1;
    *comparisons = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        (*comparisons)++;
        int c = strcmp(bag[mid].name, target);

        if (c == 0) return mid;
        else if (c < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void binarySearchMenu(Bag *bag)
{
    if (!isSortedByName) {
        printf("\nA lista precisa estar ORDENADA POR NOME para a busca binaria.\n");
        printf("Use a opcao 'Ordenar' -> 'Nome' antes de buscar.\n");
        return;
    }

    if (bagItemCounter == 0) {
        printf("\nMochila vazia.\n");
        return;
    }

    char query[30];
    printf("\nNome do item para buscar (binaria): ");
    if (!fgets(query, sizeof(query), stdin)) return;
    query[strcspn(query, "\n")] = '\0';

    long comps = 0;
    int idx = binarySearchByName(bag, bagItemCounter, query, &comps);

    if (idx >= 0) {
        printf("\nItem encontrado!\n");
        printf("Nome: %s\nTipo: %s\nQuantidade: %d\nPrioridade: %d\n",
               bag[idx].name, bag[idx].type, bag[idx].amount, bag[idx].priority);
        printf("Comparacoes (busca binaria): %ld\n", comps);
    } else {
        printf("\nItem NAO encontrado. Comparacoes (busca binaria): %ld\n", comps);
    }
}
