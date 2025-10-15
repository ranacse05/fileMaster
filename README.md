# FileMaster

A comprehensive command-line file management Swiss Army knife that combines file operations, text processing, log analysis, and backup functionality into a single powerful tool.

![FileMaster](https://img.shields.io/badge/Version-1.0.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%2FUnix-lightgrey.svg)

## 🚀 Features

### 📁 File Operations
- **Copy files** - Simple and reliable file copying
- **Search files** - Find file within a directory
- **File information** - Display detailed file metadata (size, permissions, modification time)

### 📝 Text Processing
- **Extract patterns** - Filter lines matching specific patterns
- **Find and replace** - Bulk text replacement within files
- **Text statistics** - Count lines, words, and characters

### 📊 Log Analysis
- **Error filtering** - Extract only error lines from log files
- **Log statistics** - Generate comprehensive log analytics (totals, errors, warnings)

### 💾 Backup Operations
- **Directory backup** - Create compressed backups with timestamping
- **Backup restoration** - Easy restoration from backup archives
- **Exclusion support** - Exclude specific file patterns from backups


## 📦 Installation

### Prerequisites
- GCC compiler
- Standard C library
- Tar utility (for backup functionality)

### Manual Compilation
```bash
gcc -Wall -O2 -o filemaster filemaster.c
```

## 🛠️ Usage

### Basic Syntax
```bash
./filemaster [command] [options] [arguments]
```

### File Operations
```bash
# Copy a file
./filemaster -copy source.txt destination.txt
./filemaster -cp source.txt destination.txt


# Get file information
./filemaster -info document.pdf
./filemaster -i document.pdf


# Search for files by pattern
./filemaster -search /home/user "*.txt"
./filemaster -s /home/user "*.txt"
```

## Text Processing
```bash
# Extract lines containing a pattern
./filemaster -extract logfile.log "ERROR"
./filemaster -x logfile.log "ERROR"

# Replace text in a file
./filemaster -replace config.txt "old_value" "new_value"
./filemaster -r config.txt "old_value" "new_value"

# Count lines, words, and characters
./filemaster -count document.txt
./filemaster -c document.txt
```

### Backup Operations
```bash
# Create a backup of a directory
./filemaster -backup /path/to/source /path/to/backup/location
./filemaster -b /path/to/source /path/to/backup/location

# Restore from a backup
./filemaster -restore backup.tar.gz /path/to/restore/location
./filemaster -rs backup.tar.gz /path/to/restore/location
```


## Command Reference

| Command | Alias | Parameters | Description |
|:---------|:-------|:------------|:-------------|
| `-copy` | `-cp` | `<src> <dest>` | Copy file |
| `-info` | `-i` | `<file>` | Show file information |
| `-search` | `-s` | `<dir> <pattern>` | Search files in directory matching pattern |
| `-extract` | `-x` | `<file> <pattern>` | Extract lines containing pattern |
| `-replace` | `-r` | `<file> <old> <new>` | Replace text in file |
| `-count` | `-c` | `<file>` | Count lines, words, characters |
| `-backup` | `-b` | `<src> <dest>` | Create backup |
| `-restore` | `-rs` | `<backup> <dest>` | Restore from backup |


## Platform Support

- ✅ Linux
- ✅ macOS
- ✅ BSD systems

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the [MIT License](LICENSE) - see the **LICENSE** file for details.  

## Version

Current version: 1.0.0

---

**FileMaster** - Your all-in-one file management Swiss Army knife! 🔧📁
