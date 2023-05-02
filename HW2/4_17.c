#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL 10000
int circle_points = 0, square_points = 0;   /* this data is shared by the thread(s) */
void *monteC();                  /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid;       /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */

    if (argc != 1)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread */
    pthread_create(&tid, &attr, monteC, NULL);

    /* now wait for the thread to exit */
    pthread_join(tid, NULL);
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
    srand(time(NULL));
  
    // Total Random numbers generated = possible x
    // values * possible y values
    for (int i = 0; i < (INTERVAL * INTERVAL); i++)
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

    pthread_exit(0);
}