#include <stdio.h>    
#include <string.h>   
#include <sys/stat.h> 
#include <time.h>
#include <dirent.h>

#define MAX_PATH 1024
#define BUFFER_SIZE 4096
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

void show_help() {
    printf("FileMaster - File Operations, Text Processing & Backup Tool\n");
    printf("Usage: filemaster <command> [options]\n");
    printf("Commands:\n");
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

void copy_file(const char *src, const char *dest) {
    char expanded_src[MAX_PATH];
    char expanded_dest[MAX_PATH];
    
    strncpy(expanded_src, src, MAX_PATH - 1);
    expanded_src[MAX_PATH - 1] = '\0';
    strncpy(expanded_dest, dest, MAX_PATH - 1);
    expanded_dest[MAX_PATH - 1] = '\0';
    
    FILE *source = fopen(expanded_src, "rb");
    if (!source) {
        perror("Error opening source file");
        return;
    }
    
    FILE *destination = fopen(expanded_dest, "wb");
    if (!destination) {
        perror("Error opening destination file");
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


void file_info(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Error getting file info");
        return;
    }
    
    printf("File: %s\n", filename);
    printf("Size: %lld bytes\n", (long long)st.st_size);
    
    time_t creation_time;
    const char *creation_label = "Created:";
   
#ifdef st_birthtime
    creation_time = st.st_birthtime;
#else
    creation_time = st.st_ctime;
    creation_label = "Changed:";
#endif
    
    printf("%s %s", creation_label, ctime(&creation_time));
    printf("Modified: %s", ctime(&st.st_mtime));
    printf("Accessed: %s", ctime(&st.st_atime));
    printf("Permissions: %o\n", st.st_mode & 0777);
}

void search_files(const char *dirpath, const char *pattern) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        printf("Error: Cannot open directory\n");
        return;
    }
    
    struct dirent *entry;
    printf("Searching for '%s' in %s:\n", pattern, dirpath);
    
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, pattern) != NULL) {
            printf("  %s\n", entry->d_name);
        }
    }
    
    closedir(dir);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    if ((strcmp(argv[1], "-copy") == 0 || strcmp(argv[1], "-cp") == 0) && argc == 4) {
        copy_file(argv[2], argv[3]);
    } else if ((strcmp(argv[1], "-info") == 0 || strcmp(argv[1], "-i") == 0) && argc == 3) {
        file_info(argv[2]);
    } else if ((strcmp(argv[1], "-search") == 0 || strcmp(argv[1], "-s") == 0) && argc == 4) {
        search_files(argv[2], argv[3]);
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
