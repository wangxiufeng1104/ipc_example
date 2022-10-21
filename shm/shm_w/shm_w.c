#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <errno.h>
#include <unistd.h>
#include "shareinfo.h"
#include <sys/time.h>
#include <unistd.h>

const char *memname = "shm-file0001";

__uint64_t get_time_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000000) + (tv.tv_usec));
}

int main(int argc, char **argv)
{
    int r;
    SHA_INF s;
    double count = 0;
    int fd = -1;
    printf("%s\n", argv[0]);

    while ((fd = shm_open(memname, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0666)) < 0)
    {
        if (errno == EEXIST)
        {
            shm_unlink(memname);
        }
        else
        {
            printf("shm open failed,fd:%d, errno:%d, res:%s\n", fd, errno, strerror(errno));
            return -1;
        }
    }
    r = ftruncate(fd, sizeof(SHA_INF) * 100);
    if (r != 0)
    {
        printf("ftruncate failed,%s\n", strerror(errno));
        return -2;
    }

    SHA_INF *ptr = (SHA_INF *)mmap(0, sizeof(SHA_INF) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("MMAP failed,%s\n", strerror(errno));
        return -3;
    }
    close(fd);

    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            count += 0.3;
            ptr[i].timestamp = get_time_us();
            ptr[i].data = count;
            sleep(1);
        }
    }
    return 0;
}