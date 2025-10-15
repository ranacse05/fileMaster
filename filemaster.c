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

// Text Processing
void extract_pattern(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file\n");
        return;
    }
    
    char line[BUFFER_SIZE];
    int line_num = 1;
    
    printf("Lines containing '%s':\n", pattern);
    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, pattern) != NULL) {
            printf("%d: %s", line_num, line);
        }
        line_num++;
    }
    
    fclose(file);
}


void replace_text(const char *filename, const char *old_text, const char *new_text) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file\n");
        return;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    // Replace text
    char *pos = content;
    char *found;
    int count = 0;
    int old_len = strlen(old_text);
    int new_len = strlen(new_text);
    
    while ((found = strstr(pos, old_text)) != NULL) {
        count++;
        pos = found + old_len;
    }
    
    if (count > 0) {
        // Create new content with replacements
        char *new_content = malloc(size + count * (new_len - old_len) + 1);
        char *dest = new_content;
        pos = content;
        
        while ((found = strstr(pos, old_text)) != NULL) {
            int chunk = found - pos;
            memcpy(dest, pos, chunk);
            dest += chunk;
            memcpy(dest, new_text, new_len);
            dest += new_len;
            pos = found + old_len;
        }
        strcpy(dest, pos);
        
        // Write back to file
        file = fopen(filename, "w");
        fputs(new_content, file);
        fclose(file);
        free(new_content);
        
        printf("Replaced %d occurrences of '%s' with '%s'\n", count, old_text, new_text);
    } else {
        printf("Pattern '%s' not found\n", old_text);
    }
    
    free(content);
}

void count_stats(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file\n");
        return;
    }
    
    int lines = 0, words = 0, chars = 0;
    char ch, prev_ch = ' ';
    
    while ((ch = fgetc(file)) != EOF) {
        chars++;
        if (ch == '\n') lines++;
        if (isspace(ch) && !isspace(prev_ch)) words++;
        prev_ch = ch;
    }
    
    if (!isspace(prev_ch)) words++; // Last word
    
    fclose(file);
    printf("Lines: %d, Words: %d, Characters: %d\n", lines, words, chars);
}

// Backup Functions
void create_backup(const char *src, const char *dest) {
    char backup_name[MAX_PATH];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    snprintf(backup_name, MAX_PATH, "%s/backup_%04d%02d%02d_%02d%02d%02d.tar",
             dest, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    char command[MAX_PATH * 2];
    snprintf(command, sizeof(command), "tar -czf \"%s\" \"%s\"", backup_name, src);
    
    int result = system(command);
    if (result == 0) {
        printf("Backup created: %s\n", backup_name);
    } else {
        printf("Error creating backup\n");
    }
}

void restore_backup(const char *backup_file, const char *dest) {
    char command[MAX_PATH * 2];
    snprintf(command, sizeof(command), "tar -xzf \"%s\" -C \"%s\"", backup_file, dest);
    
    int result = system(command);
    if (result == 0) {
        printf("Backup restored to: %s\n", dest);
    } else {
        printf("Error restoring backup\n");
    }
}


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
