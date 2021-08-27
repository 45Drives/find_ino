# find_ino
Quickly find file paths from inodes

## Installation
```bash
git clone https://github.com/45Drives/find_ino.git
cd find_ino
make
sudo make install
```

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

## Finding Missing Object File Paths in Cephfs
```bash
ceph pg dump_stuck | grep recovery_unfound | awk '{print $1}' | while read pg; do ceph pg $pg list_unfound | jq '.objects[].oid.oid' | sed -e 's/"//g' | cut -d'.' -f1 | xargs -I{} printf "%d\n" 0x{}; done | find_ino /mnt/fsgw | tee missing_files.txt
```