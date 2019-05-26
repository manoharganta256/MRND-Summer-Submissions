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

Node *detect_loop_ll(Node *head) {
    if (!head || !head -> next)
        return NULL;

    Node *fast_ptr = head -> next -> next;
    Node *slow_ptr = head -> next;

    while (fast_ptr && fast_ptr -> next) {
        if (fast_ptr == slow_ptr)
            break;

        fast_ptr = fast_ptr -> next -> next;
        slow_ptr = slow_ptr -> next;
    }

    if (fast_ptr != slow_ptr)
        return NULL;

    fast_ptr = head;
    while (fast_ptr != slow_ptr) {
        fast_ptr = fast_ptr -> next;
        slow_ptr = slow_ptr -> next;
    }

    return fast_ptr;
}

int main() {
    Node *root = NULL, *temp = NULL;
    int val;
    while (1) {
        scanf("%d", &val);
        if (val == -1)
            break;
        Node *new_node = get_new_node(val);

        if (root == NULL)
            root = new_node;
        else
            temp -> next = new_node;

        temp = new_node;
    }

    temp -> next = root -> next -> next; //loop

    Node *result = detect_loop_ll(root);

    if (result == NULL)
        printf("No loop");
    else
        printf("Loop at Node: %d", result -> data);

    return 0;
}