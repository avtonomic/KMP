#include "function.h"
#include "strings.h"

void list_dir (const char * dir_name, int f, char *pattern) // Функция рекурсивного прохода по директории 
{
    DIR *dir;
    dir = opendir (dir_name); // Открываем директорию 

    if (dir == NULL) { // Если в директории ничего нет, то выдается соответствующее сообщение
        printf(GRN "The directory is empty.\n" RESET);
        exit(0);
    }
    while (1) {
        struct dirent *entry;
        char path_full[PATH_MAX];
        int path_length;
        char path[PATH_MAX];
        const char *d_name;
        entry = readdir(dir); // Считываем все, что находится внутри директории 
        
        if (entry == NULL) break; // Если файлов нет, то цикл прерывается

        d_name = entry->d_name;
        if (d_name == NULL) break; 
        if (sspn(d_name, '.')) {
            snprintf(path_full, PATH_MAX, "%s/%s", dir_name, d_name); // Формируем строку путь, для открытия файла 
            if(scmp(path_full + slen(path_full) - 4, ".txt")) { // Проверяем является ли файл текстовым 
                if(!check(path_full)) { // Проверяем скрыта ли папка 
                    file_read(path_full, pattern); 
                }
            }
        }
        
        if (entry->d_type & DT_DIR) {
            
            if (strcmp(d_name, "..") != 0 && strcmp(d_name, ".") != 0) { // Отбрасываем лишнее 
                path_length = snprintf(path, PATH_MAX, "%s/%s", dir_name, d_name); // Формируем строку для следующего чтения 
                if (path_length >= PATH_MAX) exit(1);
                if(f) { // Рекурсия
                    list_dir (path, f, pattern);
                }
            }
        }
    }
    if (dir != NULL) closedir(dir);
}

int file_read(char *path, char *pattern)
{
    int k = 0, q = 0;
    printf("\n");
    
    while(k < slen(path) + 7) {
        printf("-");
        k++;
    }
    
    printf("\nOpen - " BLU "%s \n", path);
    printf("\n" RESET);

    int patternLength = slen(pattern);
    pattern[patternLength] = '\0';
    FILE *input;
    input = fopen(path, "r"); // Открываем файл 
    
    if(input == NULL) return 0; // Если не удалось, то возвращаем значение 0
    
    int textLength = 0;
    
    while (!feof(input)) { // Счетчик символов в файле 
        fgetc(input);
        textLength++;
    }
    
    if (patternLength > textLength) {  // Сравниваем текст и паттерн 
        printf("Pattern is bigger than text.\n");
        return -1;
    }
    
    char *text = (char*)calloc(textLength, sizeof(char));  // Выделяем память под текст 
    rewind(input); // Сбрасываем каретку 
    int i = 0;
    
    while (!feof(input)) {  // Переписываем текст в память 
        fscanf(input, "%c", &text[i]);
        i++;
    }
    
    fclose(input);  // Закрываем файл 

    int lastSym = kmp_matcher(text, pattern);  // Начинаем поиск подстроки в строке 
    
    if(lastSym == -1) { // Если подстрока не найдена, то выдается соответствующее сообщение
        printf("Not found pattern.\n\n"); 
        return 0;
    }
    
    while (q < textLength) { // Вывод строки и выделение найденной подстроки
        if(q == lastSym) {
            while (q < lastSym + patternLength) {
                printf(GRN "%c" RESET, text[q]);
                q++;   
            }
        }
        printf("%c", text[q]);
        q++;
    }

    printf("\n");
    
    return 0;
}

int check(char *str) 
{
    while(*(str + 1)) {
        if(*str == '/' && *(str + 1) == '.') return 1;
        str++;
    }
    return 0;
}

void picalculator(char pattern[], int pi[], int patlen) // Префикс-функция
{
    int i, j;
    pi[0] = 0;  // pi[Первый символ в шаблоне]=0
    j = 0;
    i = 1;
    while(i < patlen) {
        if(pattern[i] == pattern[j]) {
            pi[i] = j + 1;
            i++;
            j++;
                        
        } else if(j > 0) {
            j = pi[j - 1];
        } else {
            pi[i] = 0;
            i++;
        }
    }
}



int kmp_matcher(char text[], char pattern[])
{
    int i, j, F[100];
    int patlen = strlen(pattern);
    int textlen = strlen(text);
    picalculator(pattern, F, patlen);
    i = 0;
    j = 0;
    while(i < textlen) {
        if(text[i] == pattern[j]) {
                       
            if(j == patlen - 1)
                return i - j; // В (i - j) позиции происходит совпадение
            else {
                i++;
                j++;
            }
        } else if(j > 0) {
            j = F[j - 1];
        } else {
            i++;
        }
    }
    return -1; // Не найдено совпадений
}