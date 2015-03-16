#include <cassert>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <iostream>

static const off_t k_chunk_size = 4096;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Error: Usage:\n\t " << argv[0] << " [filename]\n";
        return 1;
    }
    const char* filename = argv[1];

    int fd = open(filename, O_RDWR);

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Error: unable to stat file " << filename << "\n";
        return 2;
    }

    const size_t len = sb.st_size;
    const off_t midpoint = static_cast<off_t>(len / 2U);
    off_t offset = 0U;

    std::array<uint8_t, k_chunk_size> buf1;
    std::array<uint8_t, k_chunk_size> buf2;

    while (offset < midpoint)
    {
        // Read either a full chunk or the break the remainder into halves.
        size_t read_len = (offset + k_chunk_size < midpoint) ? k_chunk_size : midpoint - offset;
        auto back_offset = len - offset - read_len;

        size_t res1 = pread(fd, buf1.data(), read_len, offset);
        size_t res2 = pread(fd, buf2.data(), read_len, back_offset);

        if (res1 != read_len || res2 != read_len) {
            std::cerr << "Error: file read error, offset = " << offset << "\n";
            return 3;
        }

        // Reverse each buffer in-place.
        std::reverse(buf1.begin(), buf1.begin() + read_len);
        std::reverse(buf2.begin(), buf2.begin() + read_len);

        res1 = pwrite(fd, buf2.data(), read_len, offset);
        res2 = pwrite(fd, buf1.data(), read_len, back_offset);

        if (res1 != read_len || res2 != read_len) {
            std::cerr << "Error: file write error, offset = " << offset << "\n";
            return 4;
        }

        offset += k_chunk_size;  // offset not correct after last iteration.
    }

    auto r = close(fd);
    assert(r == 0);

    return 0;
}
