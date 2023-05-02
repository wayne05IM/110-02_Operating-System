#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// collatzConj with print sequence
void collatzConj(int k)
{
    // test if k is valid
    if(k <= 0)
    {
        printf("Number is invalid!");
        return;
    }

    //print the sequence
    printf("%d", k);
    while(k != 1)
    {
        if(k % 2 == 0)
            k = k / 2;
        else
            k = 3 * k + 1;
            
      	printf(",%d", k);
    };
    return;
};

int main()
{
    pid_t pid;

    pid = fork(); /* fork another process */

    if (pid < 0)
    {  /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
        // exit(1);
    }
    else if (pid == 0)
    {  /* child process */
        //execlp("/bin/ls", "ls", NULL);

        //input number
        int n = 0;
        scanf("%d", &n);

        //conduct collatz conjecture
        collatzConj(n);
    }
    else
    {  /* parent process */

        // parent will wait for the child to complete
        wait(NULL);
        printf("\nChild Complete\n");
    }
    return 0; // exit(0);
}