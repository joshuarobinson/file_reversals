#include <stdio.h>
#include <stdlib.h>

char ReadByte(FILE* fp, long offset)
{
    char temp;
    if (fseek(fp, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Seek error.\n");
        exit(1);
    }

    if (fread(&temp, 1, 1, fp) != 1U) {
        fprintf(stderr, "Read error at offset %d.\n", (int)offset);
        exit(1);
    }
    return temp;
}

void WriteByte(FILE* fp, long offset, char v)
{
    if (fseek(fp, offset, SEEK_SET) != 0) {
        fprintf(stderr, "Seek error.\n");
        exit(1);
    }
    if (fwrite(&v, sizeof(v), 1, fp) != 1U) {
        fprintf(stderr, "Write error at offset %d).\n", (int)offset);
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Error. Usage:\n\t%s [filename]\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    FILE* fp = fopen(filename, "r+");

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);

    for (long i = 0; i < len / 2; ++i) {
        char a, b;
        a = ReadByte(fp, i);
        b = ReadByte(fp, len - i - 1);

        WriteByte(fp, i, b);
        WriteByte(fp, len - i - 1, a);
    }

    fclose(fp);

    return 0;
}
