#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <array>
#include <iostream>

static const long k_chunk_size = 4096;

/*
 * File Read/Write helpers.
 */
void ReadBytes(FILE* fp, long offset, uint8_t* v, size_t count)
{
    if (fseek(fp, offset, SEEK_SET) != 0) {
        std::cerr << "Seek error.\n";
        exit(1);
    }

    if (fread(v, sizeof(*v), count, fp) != count) {
        std::cerr << "Read error at offset " << offset << ".\n";
        exit(1);
    }
}

void WriteBytes(FILE* fp, long offset, uint8_t* v, size_t count)
{
    if (fseek(fp, offset, SEEK_SET) != 0) {
        std::cerr << "Seek error.\n";
        exit(1);
    }

    if (fwrite(v, sizeof(*v), count, fp) != count) {
        std::cerr << "Write error at offset " << offset << ".\n";
        exit(1);
    }
}


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage:\n\t" << argv[0] << " [filename]\n";
        return 1;
    }

    const char* filename = argv[1];

    FILE* fp = fopen(filename, "r+");

    fseek(fp, 0, SEEK_END);
    const long len = ftell(fp);
    const long midpoint = len / 2;
    long offset = 0;

    std::array<uint8_t, k_chunk_size> buf1;
    std::array<uint8_t, k_chunk_size> buf2;

    while (offset < midpoint)
    {
        // Read either a full chunk or the break the remainder into halves.
        long read_len = (offset + k_chunk_size < midpoint) ? k_chunk_size : midpoint - offset;
        auto back_offset = len - offset - read_len;

        ReadBytes(fp, offset, buf1.data(), read_len);
        ReadBytes(fp, back_offset, buf2.data(), read_len);

        // Reverse each buffer in-place.
        std::reverse(buf1.begin(), buf1.begin() + read_len);
        std::reverse(buf2.begin(), buf2.begin() + read_len);

        // Write back out.
        WriteBytes(fp, offset, buf2.data(), read_len);
        WriteBytes(fp, back_offset, buf1.data(), read_len);

        offset += k_chunk_size;  // offset not correct after last iteration.
    }

    fclose(fp);

    return 0;
}
