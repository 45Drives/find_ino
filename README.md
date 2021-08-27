# find_ino
Quickly find file paths from inodes

## Usage
```bash
cat inodes.txt | find_ino /path/to/fs | tee files.txt
```
inodes.txt format: newline delimited decimal inodes

## Output
Table of inodes and file paths:
```
123 /path/to/fs/file1
124 /path/to/fs/file2
...
```