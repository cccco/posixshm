#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define SHM_SIZE 1048576

int main(void)
{
    int shmfd = shm_open("/test.shm", O_RDWR | O_CREAT, 0666);
    if (shmfd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shmfd, SHM_SIZE) == -1) {
        perror("ftruncate");
        shm_unlink("/test.shm");
        return 1;
    }

    void *res = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (res == (void *)-1) {
        perror("mmap");
        shm_unlink("/test.shm");
        return 1;
    }

    memset(res, 13, sizeof(char) * SHM_SIZE);

    pause();

    munmap(res, SHM_SIZE);

    shm_unlink("/test.shm");

    return 0;
}

