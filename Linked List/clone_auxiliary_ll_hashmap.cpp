#include <stdio.h>
#include <stdlib.h>
#include <map>

struct ListNode {
    int data;
    struct ListNode *next;
    struct ListNode *random;
};

typedef struct ListNode Node;

Node *get_new_node(int val) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node -> data = val;
    new_node -> next = nullptr;
    new_node -> random = nullptr;
    return new_node;
}

void display_ll(Node *head) {
    while (head != nullptr) {
        printf("Data : %d ", head -> data);
        if (head -> random)
            printf("Random: %d", head -> random -> data);
        printf("\n");
        head = head -> next;
    }
    printf("\n");
}

Node *clone_ll_hashmap(Node *head) {
    std::map<Node *, Node *> hash_map;
    Node *result = nullptr, *ptr = head, *result_end = nullptr;

    while (ptr != nullptr) {
        Node *new_node = get_new_node(ptr -> data);

        hash_map.insert({ptr, new_node});

        if (result == nullptr)
            result = new_node;
        else
            result_end -> next = new_node;

        result_end = new_node;
        ptr = ptr -> next;
    }

    if (result != nullptr)
        result_end -> next = nullptr;

    for (auto &val: hash_map) {
        if (val.first -> random) {
            std::map<Node *, Node *>::iterator it;
            it = hash_map.find(val.first -> random);

            val.second -> random = it -> second;
        }
    }

    return result;
}

int main() {
    Node *root = nullptr, *temp = nullptr;
    Node *arr[7];
    for (int i = 1; i < 7; i++) {
        arr[i] = (Node *) malloc(sizeof(Node));

        Node *new_node = get_new_node(i);
        arr[i] = new_node;

        if (root == nullptr) {
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

    Node *result = clone_ll_hashmap(root);

    printf("----root----\n");
    display_ll(root);
    printf("----result----\n");
    display_ll(result);

    for (int i = 1; i < 7; i++)
        free(arr[i]);

    return 0;
}