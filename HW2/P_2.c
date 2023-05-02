#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int end;
} parameters;

#define NUMBERS 10
int unsorted[10] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};   /* this data is shared by the thread(s) */
int sorted[10] = {};
void *sortingAlg();  //insertionSort                     /* the thread */
void *merge();       //merge

int main()
{
    parameters *data = (parameters *) malloc(sizeof(parameters));
    parameters *data2 = (parameters *) malloc(sizeof(parameters));

    pthread_t tids1, tids2, tidm;       /* the thread identifier */
    pthread_attr_t attr;                /* set of attributes for the thread */
    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    data->start = 0;
    data->end = NUMBERS / 2 - 1;
    pthread_create(&tids1, &attr, sortingAlg, (void *)data);
    
    data2->start = NUMBERS / 2;
    data2->end = NUMBERS - 1;
    pthread_create(&tids2, &attr, sortingAlg, (void *)data2);
    
    pthread_join(tids1, NULL);
    pthread_join(tids2, NULL);

    pthread_create(&tidm, &attr, merge, (void *)(NUMBERS / 2));
    pthread_join(tidm, NULL);

    // print sorted list
    printf("Sorted List:[");
    for(int i = 0; i < NUMBERS - 1; i++)
    {
        printf("%d,", sorted[i]);
    }
    printf("%d]\n", sorted[NUMBERS - 1]);

    return 0;
}

void *sortingAlg(parameters* data)      //insertionSort
{
    int key, j;
    for (int i = data->start + 1; i < data->end + 1; i++)
    {
        key = unsorted[i];
        int j = i - 1;
 
        while (j >= data->start && unsorted[j] > key)
        {
            unsorted[j + 1] = unsorted[j];
            j = j - 1;
        }
        unsorted[j + 1] = key;
    }

    pthread_exit(0);
}


void *merge(int half)     //merge
{
    int start1 = 0, end1 = half - 1;
    int start2 = half, end2 = NUMBERS - 1;
    int counter = 0;

    while((start1 != (end1 + 1)) & (start2 != (end2 + 1)))
    {
        if(unsorted[start1] < unsorted[start2])
        {
            sorted[counter] = unsorted[start1];
            start1 += 1;
            counter += 1;
        }
        else if(unsorted[start1] > unsorted[start2])
        {
            sorted[counter] = unsorted[start2];
            start2 += 1;
            counter += 1; 
        }
        else
        {
            sorted[counter] = unsorted[start1];
            start1 += 1;
            counter += 1;
            sorted[counter] = unsorted[start2];
            counter += 1;
            start2 += 1;
        }
    }

    if(start1 == (end1 + 1))
    {
        while(start2 != (end2 + 1))
        {
            sorted[counter] = unsorted[start2];
            start2 += 1;
            counter += 1;
        }
    }

    if(start2 == (end2 + 1))
    {
        while(start1 != (end1 + 1))
        {
            sorted[counter] = unsorted[start1];
            start1 += 1;
            counter += 1;
        }
    }

    pthread_exit(0);
}