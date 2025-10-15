
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