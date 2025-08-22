#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX 100000

int stack1[MAX], stack2[MAX];
int top1 = -1, top2 = -1;

void push1(int x) { stack1[++top1] = x; }
void push2(int x) { stack2[++top2] = x; }

int pop1() { return stack1[top1--]; }
int pop2() { return stack2[top2--]; }

int isEmpty1() { return top1 == -1; }
int isEmpty2() { return top2 == -1; }

void transfer() {
    while (!isEmpty1()) {
        push2(pop1());
    }
}

int main() {

    int q;
    scanf("%d", &q);

    while (q--) {
        int type, x;
        scanf("%d", &type);

        if (type == 1) {   // enqueue
            scanf("%d", &x);
            push1(x);
        } 
        else if (type == 2) {  // dequeue
            if (isEmpty2()) transfer();
            if (!isEmpty2()) pop2();
        } 
        else if (type == 3) {  // print front
            if (isEmpty2()) transfer();
            if (!isEmpty2()) printf("%d\n", stack2[top2]);
        }
    }
    return 0;
}
