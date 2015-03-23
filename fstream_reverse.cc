#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>

static const size_t k_chunk_size = 4096U;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Error: Usage:\n\t " << argv[0] << " [filename]\n";
        return 1;
    }
    const char* filename = argv[1];

    std::fstream file;
    file.open(filename, std::ios::out | std::ios::in | std::ios::binary);

    file.seekg(0, file.end);
    size_t len = file.tellg();

    const size_t midpoint = static_cast<off_t>(len / 2U);
    size_t offset = 0U;

    std::array<char, k_chunk_size> buf1;
    std::array<char, k_chunk_size> buf2;

    while (offset < midpoint)
    {
        // Read either a full chunk or the break the remainder into halves.
        size_t read_len = (offset + k_chunk_size < midpoint) ? k_chunk_size : midpoint - offset;
        auto back_offset = len - offset - read_len;

        file.seekg(offset);
        file.read(buf1.data(), read_len);
        file.seekg(back_offset);
        file.read(buf2.data(), read_len);

        // Reverse each buffer in-place.
        std::reverse(buf1.begin(), buf1.begin() + read_len);
        std::reverse(buf2.begin(), buf2.begin() + read_len);

        file.seekg(offset);
        file.write(buf2.data(), read_len);
        file.seekg(back_offset);
        file.write(buf1.data(), read_len);

        offset += k_chunk_size;  // offset not correct after last iteration.
    }

    file.close();

    return 0;
}
