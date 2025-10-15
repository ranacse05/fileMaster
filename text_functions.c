
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