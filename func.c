#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_hex_byte(unsigned char byte) {
    const char* hex = "0123456789ABCDEF";
    putchar(hex[byte >> 4]);
    putchar(hex[byte & 0x0F]); //махонький
}

void print_hex_word(unsigned short word) {
    print_hex_byte((word >> 8) );
    print_hex_byte(word );
}

void print_hex_dword(unsigned int dword) {
    print_hex_byte((dword >> 24) ); //старший бро
    print_hex_byte((dword >> 16) ); //средний бро
    print_hex_byte((dword >> 8) );  //младший бро
    print_hex_byte(dword );         //самый махонький
}

void print_ascii_representation(unsigned char* buffer, int length) {
    for (int i = 0; i < length; ++i) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            putchar(buffer[i]);
        }
        else {
            putchar('.');
        }
    }
}

void hexdump(char* namefile, long int offset, long int size, int group_size, int count) {
    if (namefile == NULL) {
        printf("Invalid name file.\n"); exit(0);
    }
    FILE* file = fopen(namefile, "rb");
    if (!file) {
        printf("Invalid name file.\n"); 
        exit(0);
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, offset, SEEK_SET);

    if (offset >= file_size) {
        printf("Offset is beyond the end of the file.\n");
        fclose(file);
        exit(0);
    }
    if (size == -1 || size > file_size - offset) {
        size = file_size - offset;
    }

    unsigned char* buffer = malloc(group_size * count * sizeof(unsigned char));
    if (!buffer) {
        printf("Failed to allocate memory.\n");
        fclose(file);
        exit(0);
    }

    int read = 0; //колво за одну итерацию
    int total_bytes = 0; //общее колво
    int bytes_to_read = group_size * count; //колво для чтения за одну итерацию
    while (total_bytes < size) {
        if (size - total_bytes < bytes_to_read) {
            bytes_to_read = size - total_bytes; //если осталось меньше, то уменьшаем
        }

        read = fread(buffer, 1, bytes_to_read, file);
        if (read <= 0) {
            break;
        }
        
        printf("%08lX  ", (unsigned long)(offset + total_bytes));
        for (int i = 0; i < read; i += group_size) {
            if (group_size == 1) {
                print_hex_byte(buffer[i]);
                printf(" ");
            }
            else if (group_size == 2) {
                if (i + 2 <= read) {
                    print_hex_word(*(unsigned short*)(buffer + i));
                }
                else {
                    print_hex_byte(buffer[i]);
                }
                printf(" ");
            }
            else if (group_size == 4) {
                if (i + 4 <= read) {
                    print_hex_dword(*(unsigned int*)(buffer + i));
                }
                else if (i + 3 <= read) {
                    print_hex_word(*(unsigned short*)(buffer + i));
                    print_hex_byte(buffer[i]);
                }
                else if (i + 2 <= read) {
                    print_hex_word(*(unsigned short*)(buffer + i));
                }
                else {
                    print_hex_byte(buffer[i]);
                }
                printf(" ");
            }
        }
        if (group_size == 1) {
            int ofs = (group_size * count) - read;
            for (int i = 0; i < ofs * 2 + ofs; ++i) {
                printf(" ");
            }
            printf("|");
            print_ascii_representation(buffer, read);
        }
        printf("\n");

        total_bytes += read; //обновляем колво прочитанных байт
    }

    free(buffer);
    fclose(file);
}