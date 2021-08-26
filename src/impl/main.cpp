extern "C" {
    #include <dirent.h>
    #include <errno.h>
    #include <string.h>
}
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

void get_inodes(std::vector<int> &inodes) {
    int inode;
    while (std::cin >> inode) {
        inodes.push_back(inode);
    }
}

void find_paths(const std::vector<int> &inodes, const char *path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d == nullptr) {
        int err = errno;
        fprintf(stderr, "Error: %s\n", strerror(err));
        exit(1);
    }
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_DIR && dir->d_name[0] != '.') {
            char *new_path = (char *)malloc(sizeof(char) * strlen(path) + strlen(dir->d_name) + 2);
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, dir->d_name);
            find_paths(inodes, new_path);
            free(new_path);
        } else {
            for (long unsigned int inode : inodes) {
                if (inode == dir->d_ino) {
                    printf("%lu %s\n", inode, dir->d_name);
                }
            }
        }
    }
    closedir(d);
}

int main(int argc, char *argv[]) {
    std::vector<int> inodes;
    if (argc < 2) {
        printf("%s", "Must pass path to file system");
    }
    get_inodes(inodes);
    find_paths(inodes, argv[1]);
}