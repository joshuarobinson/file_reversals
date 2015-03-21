#include <assert.h>

#include <fcntl.h>

#include <stdlib.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Error. Usage:\n\t" << argv[0] << " [filename]\n";
        return 1;
    }
    const char* filename = argv[1];

    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        std::cerr << "Error. Unable to open file " << filename << "\n";
        return 1;
    }

    // Obtain file size.
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Error. Unable to stat filesize.\n";
        return 1;
    }

    int len = sb.st_size;

    // Mmap.
    void* addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(addr != MAP_FAILED);

    char* begin_ptr = (char*)addr;
    char* end_ptr = begin_ptr + len;

    std::reverse(begin_ptr, end_ptr);

    // Cleanup.
    munmap(addr, len);
    close(fd);
    return 0;
}
