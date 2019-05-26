#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
    struct ListNode *random;
};

typedef struct ListNode Node;

Node *get_new_node(int val) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node -> data = val;
    new_node -> next = NULL;
    new_node -> random = NULL;
    return new_node;
}

void display_ll(Node *head) {
    while (head != NULL) {
        printf("Data : %d ", head -> data);
        if (head -> random)
            printf("Random: %d", head -> random -> data);
        printf("\n");
        head = head -> next;
    }
    printf("\n");
}

Node *clone_ll(Node *root) {
    Node *temp, *ptr = root;

    while (ptr != NULL) {
        temp = ptr -> next;
        Node *new_node = get_new_node(ptr -> data);
        ptr -> next = new_node;
        new_node -> next = temp;
        ptr = temp;
    }

    ptr = root;
    while (ptr != NULL) {
        if (ptr -> random)
            ptr -> next -> random = ptr -> random -> next;
        ptr = ptr -> next -> next;
    }

    Node *result = NULL, *result_end = NULL;

    ptr = root;
    while (ptr != NULL) {
        if (result == NULL) {
            result = ptr -> next;
            result_end = result;
        }
        else {
            result_end -> next = ptr -> next;
            result_end = result_end -> next;
        }

        ptr -> next = ptr -> next -> next;
        ptr = ptr -> next;
    }

    if (result)
        result_end -> next = NULL;

    return result;
}

int main() {
    Node *root = NULL, *temp = NULL;
    Node *arr[7];
    for (int i = 1; i < 7; i++) {
        arr[i] = (Node *) malloc(sizeof(Node));

        Node *new_node = get_new_node(i);
        arr[i] = new_node;

        if (root == NULL) {
            root = new_node;
            temp = root;
        } else {
            temp->next = new_node;
            temp = new_node;
        }
    }

    arr[1] -> random = arr[3];
    arr[2] -> random = arr[5];
    arr[4] -> random = arr[5];
    arr[6] -> random = arr[6];

    Node *result = clone_ll(root);

   printf("----root----\n");
   display_ll(root);
   printf("----result----\n");
   display_ll(result);

    for (int i = 1; i < 7; i++)
        free(arr[i]);

    return 0;
}