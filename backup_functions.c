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
        fprintf(stderr, "%sError: Error creating backup %s\n", COLOR_RED, COLOR_RESET);
    }
}

void restore_backup(const char *backup_file, const char *dest) {
    char command[MAX_PATH * 2];
    snprintf(command, sizeof(command), "tar -xzf \"%s\" -C \"%s\"", backup_file, dest);
    
    int result = system(command);
    if (result == 0) {
        printf("Backup restored to: %s\n", dest);
    } else {
        fprintf(stderr, "%sError: Error restoring backup %s\n", COLOR_RED, COLOR_RESET);
    }
}
