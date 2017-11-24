#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>

typedef struct heap_ {
    int *arr;
    int size;
    int max;
} HEAP;

void printarr(char c, int *arr, int size)
{
    int i;

    printf("%c [", c);
    for (i = 0; i < size; ++i)
        printf("%d, ", arr[i]);
    printf("]\n");
}

void swap(int *a, int *b)
{
    static int tmp;

    tmp = *a;
    *a  = *b;
    *b  = tmp;
}

void heap(HEAP *h, int max)
{
    h->arr = malloc(max * sizeof(int));
    h->max = max;
    h->size = 0;
}

inline int _parent(int i)
{
    return (i - 1) / 2;
}

void _heapify(HEAP *h, int i)
{
    int left  = (2 * i) + 1;
    int right = (2 * i) + 2;
    int smallest;

    if (left < h->size && h->arr[left] < h->arr[i])
        smallest = left;
    else
        smallest = i;

    if (right < h->size && h->arr[right] < h->arr[smallest])
        smallest = right;

    if (smallest != i) {
        swap(&h->arr[i], &h->arr[smallest]);
        _heapify(h, smallest);
    }
}

void build_heap(HEAP *h, int *arr, int size)
{
    int i;

    if (h->arr != NULL)
        free(h->arr);

    heap(h, size);
    h->size = size;
    memcpy(h->arr, arr, size * sizeof(int));

    printarr('h', h->arr, size);
    for (i = (h->size / 2) - 1; i >= 0; --i) {
        _heapify(h, i);
        printarr('h', h->arr, size);
    }
}

void insert_k(HEAP *h, int val)
{
    int i = 0;

    if (h->size == h->max) {
        printf("Heap is full\n");
        return;
    }

    i = h->size;
    h->size++;
    h->arr[i] = val;

    while (i > 0 && h->arr[_parent(i)] > h->arr[i]) {
        swap(&h->arr[i], &h->arr[_parent(i)]);
        i = _parent(i);
    }

    printarr('h', h->arr, h->size);
}

void decrease_k(HEAP *h, int i, int nval)
{
    if (i < 0 || i > h->size || nval > h->arr[i])
        return;

    h->arr[i] = nval;

    while (i > 0 && h->arr[_parent(i)] > h->arr[i]) {
        swap(&h->arr[i], &h->arr[_parent(i)]);
        i = _parent(i);
    }

    printarr('h', h->arr, h->size);
}

int extract_min(HEAP *h)
{
    int root = INT_MAX;

    if (h->size <= 0)
        return root;

    root = h->arr[0];
    h->arr[0] = h->arr[h->size - 1];
    h->size--;
    _heapify(h, 0);

    return root;
}

void delete_k(HEAP *h, int i)
{
    if (i < 0 || i > h->size)
        return;

    decrease_k(h, i, INT_MIN);
    extract_min(h);
    printarr('a', h->arr, h->size);
}

int get_min(HEAP *h)
{
    int root = INT_MAX;

    if (h->size <= 0)
        return root;

    return h->arr[0];
}

int main()
{
    int i = 0;
    int v = 0;
    int arr[] = {23, 12, 44, 124, 1, -1, 0, 231, 23, 11, 22, 11, 1212, -123, 1, 100101, 5, 6, 30, 41, 35};
    int size = sizeof(arr) / sizeof(int);

    HEAP h1 = {NULL, 0, 0};
    HEAP h2 = {NULL, 0, 0};

    printf("\nHEAP 1\n");
    build_heap(&h1, arr, size);


    printf("\nHEAP 2\n");
    heap(&h2, 1000);
    for (i = 0; i < size; ++i)
        insert_k(&h2, arr[i]);

    printf("Increase\n");
    decrease_k(&h2, 2, 3456787);
    decrease_k(&h2, 14, 3456788);
    decrease_k(&h2, 0, 2);

    printf("Delete\n");
    delete_k(&h2, 3);

    printf("Extract min\n");
    while (h2.size > 0) {
        v = extract_min(&h2);
        printf("extracted %d\n", v);
    }

    return 0;
}
