#pragma once

void print_hex_byte(unsigned char byte);

void print_hex_word(unsigned short word);

void print_hex_dword(unsigned int dword);

void print_ascii_representation(unsigned char* buffer, int length);

void hexdump(char* namefile, long int offset, long int size, int group_size, int count);