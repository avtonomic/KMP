#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#ifndef FUNCTION_H
#define FUNCTION_H

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void list_dir (const char * dir_name, const int f, char *pattern);
int file_read(char * path, char * pattern);
int check(char *str);
int kmp_matcher(char text[], char pattern[]);
void picalculator(char pattern[],int pi[],int patlen);

#endif