#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000000
// Stack to save states of string
typedef struct {
    char** states;
    int top;
} Stack;

void push(Stack* stack, const char* str) {
    stack->states[++stack->top] = strdup(str);
}

void pop(Stack* stack) {
    if (stack->top >= 0) {
        free(stack->states[stack->top]);
        stack->top--;
    }
}

char* peek(Stack* stack) {
    if (stack->top >= 0) {
        return stack->states[stack->top];
    }
    return NULL;
}

int main() {
    int Q;
    scanf("%d", &Q);

    char* current = calloc(1, sizeof(char)); // current string (initially empty)
    Stack stack;
    stack.top = -1;
    stack.states = malloc((Q+1) * sizeof(char*));
    
    push(&stack, current); // push empty string

    for (int i = 0; i < Q; i++) {
        int op;
        scanf("%d", &op);

        if (op == 1) {
            // Append string
            char buf[100001];
            scanf("%s", buf);
            int old_len = strlen(current);
            int add_len = strlen(buf);

            char* new_str = malloc(old_len + add_len + 1);
            strcpy(new_str, current);
            strcat(new_str, buf);

            // Save new state and free old state
            free(current);
            current = new_str;
            push(&stack, current);

        } else if (op == 2) {
            // Delete last k chars.
            int k;
            scanf("%d", &k);
            int old_len = strlen(current);
            int new_len = old_len - k;
            current[new_len] = '\0';

            // Save new state
            push(&stack, current);

        } else if (op == 3) {
            // Print k-th char (1-based)
            int k;
            scanf("%d", &k);
            printf("%c\n", current[k-1]);

        } else if (op == 4) {
            // Undo last append/delete
            pop(&stack);
            // Replace current with top of stack (previous state)
            char* prev = peek(&stack);
            free(current);
            current = strdup(prev);
        }
    }

    // Free all allocated states
    while (stack.top >= 0) {
        free(stack.states[stack.top--]);
    }
    free(stack.states);
    free(current);

    return 0;
}
