#include <stdio.h>
#include <string.h>


#define MAX_PATH 1024
#define BUFFER_SIZE 4096


void show_help() {
    printf("FileMaster - File Operations, Text Processing & Backup Tool\n");
    printf("Usage: filemaster <command> [options]\n");
    printf("Commands:\n");
    printf("  fileops:\n");
    printf("    -copy <src> <dest>      Copy file\n");
    printf("    -info <file>            Show file information\n");
    printf("    -search <dir> <pattern> Search files in directory\n");
    printf("  textproc:\n");
    printf("    -extract <file> <pattern> Extract lines matching pattern\n");
    printf("    -replace <file> <old> <new> Replace text in file\n");
    printf("    -count <file>           Count lines, words, characters\n");
    printf("  backup:\n");
    printf("    -backup <src> <dest>    Create backup\n");
    printf("    -restore <backup> <dest> Restore from backup\n");
}


void copy_file(const char *src, const char *dest) {
    char expanded_src[MAX_PATH];
    char expanded_dest[MAX_PATH];
    
    strcpy(expanded_src, src);
    strcpy(expanded_dest, dest);
    
    FILE *source = fopen(expanded_src, "rb");
    if (!source) {
        printf("Error: Cannot open source file: %s\n", expanded_src);
        return;
    }
    
    FILE *destination = fopen(expanded_dest, "wb");
    if (!destination) {
        printf("Error: Cannot open destination file: %s\n", expanded_dest);
        fclose(source);
        return;
    }
    
    char buffer[BUFFER_SIZE];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }
    
    fclose(source);
    fclose(destination);
    printf("Copied: %s -> %s\n", expanded_src, expanded_dest);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    if (strcmp(argv[1], "-copy") == 0 && argc == 4) {
        copy_file(argv[2], argv[3]);
    } else {
        show_help();
        return 1;
    }

    return 0;
}