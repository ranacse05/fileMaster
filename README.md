# FileMaster

A comprehensive command-line file management Swiss Army knife that combines file operations, text processing, log analysis, and backup functionality into a single powerful tool.

![FileMaster](https://img.shields.io/badge/Version-1.0.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%2FUnix-lightgrey.svg)

## 🚀 Features

### 📁 File Operations
- **Copy files** - Simple and reliable file copying
- **Search files** - Find patterns within files
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

### Manual Compilation
```bash
gcc -Wall -O2 -o filemaster filemaster.c
```