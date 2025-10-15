#include <stdio.h>    
#include <string.h>   
#include <sys/stat.h> 
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PATH 1024
#define BUFFER_SIZE 4096
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

void show_help() {
    printf("FileMaster - File Operations, Text Processing & Backup Tool\n");
    printf("Usage: filemaster <command> [options]\n");
    printf("Commands:\n");
    printf("  help:\n");
    printf("    -man                          Shows the manual\n");
    printf("  fileops:\n");
    printf("    -copy, -cp <src> <dest>       Copy file\n");
    printf("    -info, -i <file>              Show file information\n");
    printf("    -search, -s <dir> <pattern>   Search files in directory\n");
    printf("  textproc:\n");
    printf("    -extract, -x <file> <pattern> Extract lines matching pattern\n");
    printf("    -replace, -r <file> <old> <new> Replace text in file\n");
    printf("    -count, -c <file>             Count lines, words, characters\n");
    printf("  backup:\n");
    printf("    -backup, -b <src> <dest>      Create backup\n");
    printf("    -restore, -rs <backup> <dest> Restore from backup\n");
}

#include "file_functions.c"

void copy_file(const char *src, const char *dest);
void file_info(const char *filename);

#include "text_functions.c"

void extract_pattern(const char *filename, const char *pattern);
void replace_text(const char *filename, const char *old_text, const char *new_text);
void count_stats(const char *filename);

// Backup Functions
#include "backup_functions.c"
void create_backup(const char *src, const char *dest);
void restore_backup(const char *backup_file, const char *dest);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }
    if (strcmp(argv[1], "-man") == 0  && argc == 2) {
        show_help();
    } else if ((strcmp(argv[1], "-copy") == 0 || strcmp(argv[1], "-cp") == 0) && argc == 4) {
        copy_file(argv[2], argv[3]);
    } else if ((strcmp(argv[1], "-info") == 0 || strcmp(argv[1], "-i") == 0) && argc == 3) {
        file_info(argv[2]);
    } else if ((strcmp(argv[1], "-search") == 0 || strcmp(argv[1], "-s") == 0) && argc == 4) {
        search_files(argv[2], argv[3]);
    } else if ((strcmp(argv[1], "-extract") == 0 || strcmp(argv[1], "-x") == 0) && argc == 4) {
        extract_pattern(argv[2], argv[3]);
    } else if ((strcmp(argv[1], "-replace") == 0 || strcmp(argv[1], "-r") == 0) && argc == 5) {
        replace_text(argv[2], argv[3], argv[4]);
    } else if ((strcmp(argv[1], "-count") == 0 || strcmp(argv[1], "-c") == 0) && argc == 3) {
        count_stats(argv[2]);
    } else if ((strcmp(argv[1], "-backup") == 0 || strcmp(argv[1], "-b") == 0) && argc == 4) {
        create_backup(argv[2], argv[3]);
    } else if ((strcmp(argv[1], "-restore") == 0 || strcmp(argv[1], "-rs") == 0) && argc == 4) {
        restore_backup(argv[2], argv[3]);
    } else {
        if (argv[1][0] == '-') { 
            fprintf(stderr, "%sError: Unknown command or invalid flag: %s %s\n\n", 
                    COLOR_RED, argv[1], COLOR_RESET);
        }
        show_help();
        return 1;
    }

    return 0;
}
