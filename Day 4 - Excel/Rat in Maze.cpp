#include <stdio.h>
#include <stdlib.h>

struct QueueNode {
    int row;
    int col;
    struct QueueNode *next;
    struct QueueNode *parent;
};

typedef struct QueueNode Node;

int **create_2d_matrix(int n, int m) {
    int **arr = (int **)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        arr[i] = (int *)malloc(sizeof(int) * m);

    return arr;
}

void fillMatrix(int **visited, int n, int m, int val) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            visited[i][j] = val;
    }
}

void solve(int **maze,int n, int m, int start_row, int start_col, int end_row, int end_col, int **solution, int *index) {
    int current_row = start_row, current_col = start_col;

    int **visited = create_2d_matrix(n, m);
    fillMatrix(visited, n, m, 0);

    int path[100][2];
    *index += 1;
    path[*index][0] = start_row;
    path[*index][1] = start_col;
    visited[start_row][start_col] = 1;

    while (*index != -1) {

        if (current_row == end_row && current_col == end_col)
            break;

        if (current_row < n && current_col + 1 < m && maze[current_row][current_col + 1] && !visited[current_row][current_col + 1]) {
            current_col += 1;
            *index += 1;
            path[*index][0] = current_row;
            path[*index][1] = current_col;
            visited[current_row][current_col] = 1;
            continue;
        }
        if (current_row + 1 < n && current_col < m && maze[current_row + 1][current_col] && !visited[current_row + 1][current_col]) {
            current_row += 1;
            *index += 1;
            path[*index][0] = current_row;
            path[*index][1] = current_col;
            visited[current_row][current_col] = 1;
            continue;
        }
        if (current_row - 1 >= 0 && current_col < m && maze[current_row - 1][current_col] && !visited[current_row - 1][current_col]) {
            current_row -= 1;
            *index += 1;
            path[*index][0] = current_row;
            path[*index][1] = current_col;
            visited[current_row][current_col] = 1;
            continue;
        }
        if (current_row < n && current_col - 1 >= 0 && maze[current_row][current_col - 1] && !visited[current_row][current_col - 1]) {
            current_col -= 1;
            *index += 1;
            path[*index][0] = current_row;
            path[*index][1] = current_col;
            visited[current_row][current_col] = 1;
            continue;
        }

        *index -= 1;
        current_row = path[*index][0];
        current_col = path[*index][1];
    }

    if (*index != -1) {
        for (int i = 0; i < *index; i++) {
            solution[path[i][0]][path[i][1]] = 1;
        }
        solution[end_row][end_col] = 1;
    }
}

Node *get_new_node(int row, int col) {
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node -> row = row;
    new_node -> col = col;
    new_node -> next = NULL;
    new_node -> parent = NULL;

    return new_node;
}

void solve_best(int **maze, int n, int m,int start_row, int start_col, int end_row, int end_col, int **solution) {
    Node *front = NULL, *rear = NULL;

    front = get_new_node(start_row, start_col);
    rear = front;

    int **visited = create_2d_matrix(n, m);
    fillMatrix(visited, n, m, 0);
    visited[start_row][start_col] = 1;

    while (front != NULL) {
        int row = front -> row, col = front -> col;
        if (row == end_row && col == end_col)
            break;

        if (col + 1 < m && maze[row][col + 1] && !visited[row][col + 1]) {
            Node *new_node = get_new_node(row, col + 1);
            rear -> next = new_node;
            new_node -> parent = front;
            visited[row][col+1] = 1;
            rear = rear -> next;
        }

        if (row + 1 < n && maze[row + 1][col] && !visited[row + 1][col]) {
            Node *new_node = get_new_node(row + 1, col);
            rear -> next = new_node;
            new_node -> parent = front;
            visited[row + 1][col] = 1;
            rear = rear -> next;
        }

        if (col - 1 >= 0 && maze[row][col - 1] && !visited[row][col - 1]) {
            Node *new_node = get_new_node(row, col - 1);
            rear -> next = new_node;
            new_node -> parent = front;
            visited[row][col - 1] = 1;
            rear = rear -> next;
        }

        if (row - 1 >= 0 && maze[row - 1][col] && !visited[row - 1][col]) {
            Node *new_node = get_new_node(row - 1, col);
            rear -> next = new_node;
            new_node -> parent = front;
            visited[row - 1][col] = 1;
            rear = rear -> next;
        }

        front = front -> next;
    }

    while (front != NULL) {
        solution[front -> row][front -> col] = 1;
        front = front -> parent;
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int **maze = create_2d_matrix(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0;  j < m; j++)
            scanf("%d", &maze[i][j]);
    }

    printf("Enter start: ");
    int start_row, start_col;
    scanf("%d %d", &start_row, &start_col);

    printf("Enter end: ");
    int end_row, end_col;
    scanf("%d %d", &end_row, &end_col);

    int **solution = create_2d_matrix(n, m);
    fillMatrix(solution, n, m, 0);
//    int index = -1;
    solve_best(maze, n, m, start_row, start_col, end_row, end_col, solution);
//    if (index == -1)
//        printf("No path possible\n");
//    else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                printf("%d ", solution[i][j]);
            printf("\n");
        }
//    }
    return 0;
}