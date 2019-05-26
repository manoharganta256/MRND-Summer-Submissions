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

Node *reverse_ll(Node *head) {
    Node *current = head;
    Node *prev = NULL, *next = NULL;

    while (current != NULL) {
        next = current->next;

        current->next = prev;

        prev = current;
        current = next;
    }
    return prev;
}

Node *reverse_in_parts(Node *root, int k) {
    if (root == NULL || k <= 1)
        return root;

    Node *result = NULL, *result_end = NULL, *temp = NULL, *temp_end = NULL;
    int count = 0, less_than = 1;

    while (root != NULL) {
        if (count == k) {
            less_than = 0;
            temp_end -> next = NULL;
            if (result == NULL)
                result = reverse_ll(temp);
            else
                result_end -> next = reverse_ll(temp);

            result_end = temp;
            temp = NULL;
            count = 0;
        }
        else {
            count++;
            if (temp == NULL) {
                temp = root;
                temp_end = temp;
            }
            else {
                temp_end -> next = root;
                temp_end = root;
            }
            root = root -> next;
        }
    }

    if (count == k) {
        if (result == NULL)
            return reverse_ll(temp);
        result_end -> next = reverse_ll(temp);
    }
    else
        result_end -> next = temp;

    if (less_than) {
        temp_end -> next = NULL;
        return temp;
    }

    return result;
}

int main() {
    Node *root = NULL, *temp = NULL;
    int val, k;
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

    printf("Enter k: ");
    scanf("%d", &k);

    root = reverse_in_parts(root, k);

    while (root != NULL) {
        printf("%d ", root -> data);
        root = root -> next;
    }

    return 0;
}