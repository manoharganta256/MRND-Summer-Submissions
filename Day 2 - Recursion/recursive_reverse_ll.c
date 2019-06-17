#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

typedef struct ListNode Node;

Node *get_new_node(int val) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node -> data = val;
    new_node -> next = NULL;
    return new_node;
}

Node *reverse_recursive_ll(Node *head) {
    if (head == NULL || head -> next == NULL)
        return head;

    Node *ptr = reverse_recursive_ll(head -> next);

    head -> next -> next = head;
    head -> next = NULL;
    return ptr;
}

int main() {
    Node *root = NULL, *temp = NULL;
    int val;
    while (1) {
        scanf("%d", &val);
        if (val == -1)
            break;
        Node *new_node = get_new_node(val);

        if (root == NULL) {
            root = new_node;
            temp = root;
        }
        else {
            temp -> next = new_node;
            temp = new_node;
        }
    }

    root = reverse_recursive_ll(root);

    while (root != NULL) {
        printf("%d ", root -> data);
        root = root -> next;
    }
}
