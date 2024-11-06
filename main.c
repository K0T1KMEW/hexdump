#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"
#include "func.h"

int main(int argc, char* argv[]) {
    char* namefile = NULL;
    long int offset = 0;
    long int size = -1;
    int group_size = 1;
    int count = 16;

    int value;
    while ((value = getopt(argc, argv, "i:o:l:g:n:")) != -1) {
        switch (value) {
        case 'i':
            namefile = optarg;
            break;
        case 'o':
            offset = atoi(optarg);
            break;
        case 'l':
            size = atoi(optarg);
            break;
        case 'g':
            if (strcmp(optarg, "byte") == 0) {
                group_size = 1;
            }
            else if (strcmp(optarg, "word") == 0) {
                group_size = 2;
            }
            else if (strcmp(optarg, "dword") == 0) {
                group_size = 4;
            }
            break;
        case 'n':
            count = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s -i file_name [-o offset] [-l size] [-g byte|word|dword] [-n count]\n", argv[0]);
            return 1;
        }
    }

    hexdump(namefile, offset, size, group_size, count);

    return 0;
}
