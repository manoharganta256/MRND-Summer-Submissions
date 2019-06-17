#include <stdio.h>
#include <stdlib.h>
#define LEAFSIZE 10

struct TrieNode {
    struct TrieNode *children[LEAFSIZE];
    int word_end;
};

typedef struct TrieNode Node;

Node *get_new_node() {
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node -> word_end = 0;
    for (int i = 0; i < LEAFSIZE; i++)
        new_node -> children[i] = NULL;

    return new_node;
}

void permutations_with_replacement(int *arr, int *prem, int n, int index) {
    if (index == n) {
        for (int i = 0; i < index; i++)
            printf("%d ", prem[i]);
        printf("\n");
    }
    else {
        for (int i = 0; i < n; i++) {
            prem[index] = arr[i];
            permutations_with_replacement(arr, prem, n, index + 1);
        }
    }
}

void permutations(int *arr, int n, int index) {
    if (index == n) {
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }
    else {
        for (int i = index ; i < n; i++) {
            int temp = arr[index];
            arr[index] = arr[i];
            arr[i] = temp;

            permutations(arr, n, index + 1);

            temp = arr[index];
            arr[index] = arr[i];
            arr[i] = temp;
        }
    }
}

void insert_number(Node *root, const int *arr, int n) {
    Node *ptr = root;

    for (int i = 0; i < n; i++) {
        if (!ptr -> children[arr[i]])
            ptr -> children[arr[i]] = get_new_node();

        ptr = ptr -> children[arr[i]];
    }

    ptr -> word_end = 1;
}

void print_trie(Node *root, int *buffer, int index) {
    Node *ptr = root;

    if (ptr -> word_end) {
        for (int i = 0; i < index; i++)
            printf("%d ", buffer[i]);
        printf("\n");
    }
    else {
        for (int i = 0; i < LEAFSIZE; i++) {
            if (ptr->children[i]) {
                buffer[index] = i;
                print_trie(ptr->children[i], buffer, index + 1);
            }
        }
    }
}

void permutations_unique_util(int *arr, int n, int index, Node *root) {
    if (index == n) {
        insert_number(root, arr, n);
    }
    else {
        for (int i = index ; i < n; i++) {
            int temp = arr[index];
            arr[index] = arr[i];
            arr[i] = temp;

            permutations_unique_util(arr, n, index + 1, root);

            temp = arr[index];
            arr[index] = arr[i];
            arr[i] = temp;
        }
    }
}

void permutations_unique(int *arr, int n, int index) {
    Node *root = get_new_node();

    permutations_unique_util(arr, n, index, root);

    int *buffer = (int *)malloc(sizeof(int) * n);

    print_trie(root, buffer, 0);
    free(buffer);
}

int main() {
    int n;
    scanf("%d", &n);

    int *arr = (int *)malloc(sizeof(int) * n);
    int *perm = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

//    printf("---- permutation ----\n");
//    permutations(arr, n, 0);

//    printf("---- permutation with replacement ----\n");
//    permutations_with_replacement(arr, perm,  n, 0);

    printf("---- unique permutations ----\n");
    permutations_unique(arr, n, 0);

    return 0;
}