#include <assert.h>

#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Error. Usage:\n\t%s [filename]\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];

    int fd = open(filename, O_RDWR);
    assert(fd != -1);

    // Obtain file size.
    struct stat sb;
    int res = fstat(fd, &sb);
    assert(res != -1);
    int len = sb.st_size;

    // Mmap.
    void* addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(addr != MAP_FAILED);

    char* begin_ptr = (char*)addr;
    char* end_ptr = begin_ptr + len;

    // Reverse loop.
    for (long i = 0; i < len / 2; ++i) {
        char a = *(begin_ptr + i);
        char b = *(end_ptr - i - 1);

        *(begin_ptr + i) = b;
        *(end_ptr - i - 1) = a;
    }

    // Cleanup.
    munmap(addr, len);
    close(fd);
    return 0;
}
