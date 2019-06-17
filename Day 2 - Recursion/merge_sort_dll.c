#include <stdio.h>
#include <stdlib.h>

struct DoubleListNode {
    int data;
    struct DoubleListNode *next;
    struct DoubleListNode *prev;
};

typedef struct DoubleListNode Node;

Node *get_new_node(int val) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node -> data = val;
    new_node -> next = NULL;
    new_node -> prev = NULL;
    return new_node;
}

Node *create_dll(int *arr, int len) {
    Node *head = NULL, *head_end = NULL;

    for (int i = 0; i < len; i++) {
        Node *new_node = get_new_node(arr[i]);

        if (head == NULL)
            head = new_node;
        else {
            head_end -> next = new_node;
            new_node -> prev = head_end;
        }

        head_end = new_node;
    }

    return head;
}

Node *merge_dll(Node *head1, Node *head2) {
    if (head1 == NULL)
        return head2;
    if (head2 == NULL)
        return head1;

    Node *result = NULL, *result_end = NULL;

    while (head1 && head2) {
        if (head1 -> data < head2 -> data) {
            if (result == NULL) {
                result = head1;
                result -> prev = NULL;
            }
            else {
                result_end -> next = head1;
                head1 -> prev = result;
            }

            result_end = head1;
            head1 = head1 -> next;
        }
        else {
            if (result == NULL) {
                result = head2;
                result -> prev = NULL;
            }
            else {
                result_end -> next = head2;
                head2 -> prev = result;
            }

            result_end = head2;
            head2 = head2 -> next;
        }
    }

    while (head1) {
        result_end -> next = head1;
        head1 -> prev = result_end;
        head1 = head1 -> next;
        result_end = result_end -> next;
    }

    while (head2) {
        result_end -> next = head2;
        head2 -> prev = result_end;
        head2 = head2 -> next;
        result_end = result_end -> next;
    }

    return result;
}

Node *merge_sort_dll(Node *head) {
    if (head == NULL || head -> next == NULL)
        return head;

    Node *fast_ptr = head -> next, *slow_ptr = head;

    while(fast_ptr && fast_ptr -> next) {
        fast_ptr = fast_ptr -> next -> next;
        slow_ptr = slow_ptr -> next;
    }

    Node *head2 = slow_ptr -> next;
    slow_ptr -> next = NULL;
    head2 -> prev = NULL;

    head = merge_sort_dll(head);
    head2 = merge_sort_dll(head2);

    return merge_dll(head, head2);
}

int main() {

    int arr[] = {5, 3, 4, 1, 2};
    int len = sizeof(arr) / sizeof(arr[0]);

    Node *head = create_dll(arr, len);

    head = merge_sort_dll(head);

    while (head != NULL) {
        printf("%d ", head -> data);
        head = head -> next;
    }

    return 0;
}