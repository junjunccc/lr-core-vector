/* vector.c —— 你要实现的地方 */
#include "vector.h"
#include <stdlib.h>

int vector_init(vector *v, size_t capacity) {
    if (capacity == 0) {
        v->data = v->end = v->cap = NULL;
        return 0;
    }
    if (capacity > SIZE_MAX / sizeof(int)) {
        v->data = v->end = v->cap = NULL;
        return -1;
    }
    int *p = malloc(capacity * sizeof(int));
    if (p == NULL) {
        v->data = v->end = v->cap = NULL;
        return -1;
    }
    v->data = v->end = p;
    v->cap = p + capacity;
    return 0;
}

void vector_destroy(vector *v) {
    free(v->data);
    v->data = v->end = v->cap = NULL;
}

size_t size(const vector *v) {
    if (v->data == NULL) {
        return 0;
    }
    return (size_t)(v->end - v->data);
}

size_t capacity(const vector *v) {
    if (v->data == NULL) {
        return 0;
    }
    return (size_t)(v->cap - v->data);
}

int empty(const vector *v) {
    return size(v) == 0;
}

int get(const vector *v, size_t index, int *out) {
    if (index >= size(v)) {
        return -1;
    }
    *out = v->data[index];
    return 0;
}

int set(vector *v, size_t index, int value) {
    if (index >= size(v)) {
        return -1;
    }
    v->data[index] = value;
    return 0;
}

int front(const vector *v, int *out) {
    if (empty(v)) {
        return -1;
    }
    return get(v, 0, out);
}

int back(const vector *v, int *out) {
    if (empty(v)) {
        return -1;
    }
    return get(v, size(v) - 1, out);
}

int push_back(vector *v, int value) {
    if (v->end == v->cap) {
        size_t old_cap = capacity(v);
        size_t new_cap = old_cap == 0 ? 1 : old_cap * 2;
        if (new_cap > SIZE_MAX / sizeof(int)) {
            return -1;
        }
        if (reserve(v, new_cap) != 0) {
            return -1;
        }
    }
    *v->end = value;
    v->end++;
    return 0;
}

int pop_back(vector *v, int *out) {
    if (back(v, out) != 0) {
        return -1;
    }
    v->end--;
    return 0;
}

int reserve(vector *v, size_t new_cap) {
    if (new_cap <= capacity(v)) {
        return 0;
    }
    if (new_cap > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    size_t len = size(v);
    int *p = realloc(v->data, new_cap * sizeof(int));
    if (p == NULL) {
        return -1;
    }
    v->data = p;
    v->end = p + len;
    v->cap = p + new_cap;
    return 0;
}

int shrink_to_fit(vector *v) {
    size_t len = size(v);
    if (len == 0) {
        vector_destroy(v);
        return 0;
    }
    if (len == capacity(v)) {
        return 0;
    }
    int *p = realloc(v->data, len * sizeof(int));
    if (p == NULL) {
        return -1;
    }
    v->data = p;
    v->end = v->cap = p + len;
    return 0;
}

void clear(vector *v) {
    v->end = v->data;
}
