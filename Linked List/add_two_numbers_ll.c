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

Node *create_ll(int n) {
    Node *head = NULL, *ll_end = NULL;

    if (n == 0) {
        head = get_new_node(0);
        head -> next = NULL;
        return head;
    }

    while (n > 0) {
        Node *new_node = get_new_node(n % 10);
        if (head == NULL)
            head = new_node;
        else
            ll_end -> next = new_node;

        ll_end = new_node;
        n /= 10;
    }
    return reverse_ll(head);
}

void display_ll(Node *head) {
    while (head != NULL) {
        printf("%d ", head -> data);
        head = head -> next;
    }
    printf("\n");
}

int length_ll(Node *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head -> next;
    }
    return count;
}

Node *insert_begin(Node *head, int val) {
    Node *new_node = get_new_node(val);
    Node *temp = head;
    head = new_node;
    head -> next = temp;
    return head;
}

Node *add_util(Node *num1, Node *num2, int *carry) {
    if (num1 == NULL)
        return NULL;

    Node *result = (Node *)malloc(sizeof(Node));

    result -> next = add_util(num1 -> next, num2 -> next, carry);

    int sum = num1 -> data + num2 -> data + *carry;
    result -> data = sum % 10;
    *carry = sum / 10;

    return result;
}

Node *add_carry(Node *head, int *carry) {
    if (head == NULL)
        return NULL;
    Node *result = (Node *)malloc(sizeof(Node));

    result -> next = add_carry(head -> next, carry);

    int sum = head -> data + *carry;
    result -> data = sum % 10;
    *carry = sum / 10;

    return result;
}

Node *addTwoNumbers_ll(Node *num1, Node *num2) {

    if (num1 == NULL)
        return num2;

    if (num2 == NULL)
        return num1;

    int num1_len = length_ll(num1);
    int num2_len = length_ll(num2);

    int carry = 0;

    if (num1_len == num2_len) {
        Node *result = add_util(num1, num2, &carry);
        if (carry)
            return insert_begin(result, 1);
        return result;
    }

    Node *ptr = num1_len > num2_len ? num1 : num2;
    Node *temp = ptr, *parent;
    for (int i = 0; i < abs(num1_len - num2_len); i++) {
        parent = temp;
        temp = temp -> next;
    }

    Node *result;
    if (num1_len > num2_len)
        result = add_util(temp, num2, &carry);
    else
        result = add_util(num1, temp, &carry);

    parent -> next = NULL;
    ptr = add_carry(ptr, &carry);

    temp = ptr;
    while (temp -> next != NULL)
        temp = temp -> next;
    temp -> next = result;
    if (carry)
        return insert_begin(ptr, 1);
    return ptr;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    Node *num1 = create_ll(n);
    Node *num2 = create_ll(m);

    Node *result = addTwoNumbers_ll(num1, num2);

    display_ll(result);

    return 0;
}