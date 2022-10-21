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
#include <time.h>
#include <libgen.h>
#include <sys/types.h>

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
    struct stat buf;
    printf("%s\n", argv[0]);
    if (argc < 2)
    {
        printf("usage:%s <index>\n", basename(argv[0]));
        printf("\t --index:0-99\n");
        return 0;
    }
    int index = atoi(argv[1]);
    const size_t region_size = sysconf(_SC_PAGE_SIZE);
    int fd = shm_open(memname, O_RDONLY, 0666);
    if (fd < 0)
    {
        printf("shm open failed, errno:%d, res:%s\n", errno, strerror(errno));
        return -1;
    }
    if (fstat(fd, &buf))
    {
        printf("fstat failed, errno:%d, res:%s\n", errno, strerror(errno));
        return -2;
    }
    SHA_INF *ptr = (SHA_INF *)mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("MMAP failed,%s\n", strerror(errno));
        return -3;
    }
    close(fd);
    printf("timestamp:%lu, data:%lf\n", ptr[index].timestamp, ptr[index].data);
    munmap((void *)ptr, buf.st_size);
    return 0;
}