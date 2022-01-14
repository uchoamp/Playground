#include <stdio.h>

#define V_SIZE 10


void qsort(int v[], int left, int right);
void swap(int v[], int i, int j);
void printArray(int v[], size_t s);

int main(void)
{
    int v[V_SIZE] = {3, 3, 5, 65, 23, -1, 6, 8, 11, 9};

    qsort(v, 0, 9);

    printArray(v, V_SIZE);

    return 0;
}

void qsort(int v[], int left, int right)
{
    int i, last;
    void swap(int v[], int i, int j);

    if(left >= right)
        return;

    swap(v, left, (left+right)/2);
    last = left;
    for(i = left+1; i <= right; i++)
        if(v[i] < v[left])
            swap(v, ++last, i);
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}


void swap(int v[], int i, int j)
{
    int temp;
    printArray(v, V_SIZE);
    
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;

}


void printArray(int v[], size_t s)
{
    printf("{");
    for(int i = 0; i < s-1; i++)
        printf(" %d,", v[i]);
    printf(" %d }\n", v[s-1]);

}

