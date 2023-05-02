#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;

#define INTERVAL 1000
int circle_points = 0, square_points = 0; /* this data is shared by the thread(s) */
void *monteC();                           /* the thread */


int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    /* create the mutex lock */
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid1, tid2, tid3, tid4, tid5;       /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid1, &attr, monteC, NULL);
    pthread_create(&tid2, &attr, monteC, NULL);
    pthread_create(&tid3, &attr, monteC, NULL);
    pthread_create(&tid4, &attr, monteC, NULL);
    pthread_create(&tid5, &attr, monteC, NULL);
    /* now wait for the thread to exit */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

    double pi;
    // estimated pi after this iteration
    pi = (double)(4 * circle_points) / square_points;
    // Final Estimated Value
    printf("Estimation of Pi = %g", pi);
    printf("\n");

    return 0;
}

void *monteC()
{
    double rand_x, rand_y, origin_dist;

    // Initializing rand()
    srand(time(NULL));//

    /* acquire the mutex lock */
    pthread_mutex_lock(&mutex);

    // Total Random numbers generated = possible x
    // values * possible y values
    for (int i = 0; i < INTERVAL ; i++)
    {
        // Randomly generated x and y values
        rand_x = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
        rand_y = (double)(rand() % (INTERVAL + 1)) / INTERVAL;

        // Distance between (x, y) from the origin
        origin_dist = rand_x * rand_x + rand_y * rand_y;

        // Checking if (x, y) lies inside the define
        // circle with R=1
        if (origin_dist <= 1)
            circle_points++;

        // Total number of points generated
        square_points++;
    }

    /* release the mutex lock */
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}