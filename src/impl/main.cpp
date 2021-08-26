/*
 *    Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>
 *    
 *    This file is part of find_ino.
 * 
 *    find_ino is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    find_ino is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with find_ino.  If not, see <https://www.gnu.org/licenses/>.
 */

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
            if (new_path == nullptr) {
                int err = errno;
                fprintf(stderr, "malloc error: %s\n", strerror(err));
                exit(1);
            }
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