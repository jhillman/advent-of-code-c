#include <stdio.h>
#include <stdlib.h>

struct CharStack {
    char *data;
    int capacity;
    int size;
};

void push(struct CharStack *stack, char value) {
    if (stack->capacity == 0) {
        stack->capacity = 5;
        stack->data = (char *)malloc(stack->capacity * sizeof(char));
    } else if (stack->size == stack->capacity) {
        stack->capacity += 5;
        stack->data = (char *)realloc(stack->data, stack->capacity * sizeof(char));
    }

    stack->data[stack->size++] = value;
}

char pop(struct CharStack *stack) {
    if (stack->size > 0) {
        return stack->data[--stack->size];
    }

    return '*';
}

void clear(struct CharStack *stack) {
    while(stack->size) {
        pop(stack);
    }    
}

char closer(char opener) {
    switch (opener) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
    }

    return '*';
}
