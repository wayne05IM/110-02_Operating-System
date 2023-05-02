#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// global file descriptions
const int SIZE = 4096;
const char *name = "OS";

// collatzConj with momory sharing
void collatzConj(int k)
{
    // establish shared memory
    int shm_fd; // shared memory file descriptor
    void *ptr;  // pointer to shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // test if k is valid
    if (k <= 0)
    {
        //printf("Number is invalid!");
        char *message_0 = "Number is invalid!";
        sprintf(ptr, "%s", message_0);
        ptr += strlen(message_0);
        return;
    }

    // input the sequence into shared momory
    while (k != 1)
    {
        char *message_1;
        sprintf(message_1, "%d,", k);
        sprintf(ptr, "%s", message_1);
        ptr += strlen(message_1);

        if (k % 2 == 0)
            k = k / 2;
        else
            k = 3 * k + 1;
    };

    char *message_2 = "1";
    sprintf(ptr, "%s", message_2);
    ptr += strlen(message_2);

    return;
};

int main()
{
    pid_t pid;
    pid = fork(); /* fork another process */

    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
        // exit(1);
    }
    else if (pid == 0)
    {/* child process */

        // input number
        int n = 0;
        scanf("%d", &n);

        // conduct collatz conjecture
        collatzConj(n);
    }
    else
    { /* parent process */

        // parent will wait for the child to complete
        wait(NULL);
        printf("Child Complete\n");

        // print shared memory
        int shm_fd;
        void *ptr;

        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

        printf("Sequence in shared momory: \n");
        printf("%s\n", (char *)ptr);

        shm_unlink(name);
    }
    return 0; // exit(0);
}