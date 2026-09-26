// ANW's Not WAD packer
// (c) GhostNoise, 2026
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/archive.h"

int main(int argc, char **argv){
    if (argc <= 2) {
        fprintf(stderr, "usage: %s [anwfile] [file1] <file2, file3...>\n", argv[0]);
        return -1;
    } unlink(argv[1]);
    int archive_fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    anw_archive_header header = {
        .magic = { 'A', 'N', 'W', '0' },
        .file_count = argc - 2,
    }; if (write(archive_fd, &header, sizeof(anw_archive_header)) != sizeof(anw_archive_header)){
        fprintf(stderr, "Failed to write the header to the archive.\n");
        return -1;
    } for (int i = 2; i < argc; i++){
        bool filename_exceeded = false;
        int entry_fd = open(argv[i], O_RDONLY);
        anw_entry_header header = {
            .filesize = lseek(entry_fd, 0, SEEK_END),
        }; lseek(entry_fd, 0, SEEK_SET);
        // this automatically truncates the filename if it's longer than 31
        // that's exactly what we need
        for (int j = 0; j < 31; j++){
            if (!filename_exceeded){
                if (argv[i][j] == 0) filename_exceeded = true;
                header.filename[j] = argv[i][j];
            } else header.filename[j] = 0;
        } header.filename[31] = 0;
        if (write(archive_fd, &header, sizeof(anw_entry_header)) != sizeof(anw_entry_header)){
            fprintf(stderr, "Failed to write entry header for file #%d (%s) to the archive.\n", i - 1, argv[i]);
            return -1;
        } void *databuf = malloc(header.filesize);
        if (!databuf){
            fprintf(stderr, "Failed to allocate memory for file #%d (%s).\n", i - 1, argv[i]);
            return -1;
        } if (read(entry_fd, databuf, header.filesize) != header.filesize){
            fprintf(stderr, "Failed to read file #%d (%s).\n", i - 2, argv[i]);
            return -1;
        } if (write(archive_fd, databuf, header.filesize) != header.filesize){
            fprintf(stderr, "Failed to write file #%d (%s) to the archive.\n", i - 1, argv[i]);
            return -1;
        } free(databuf);
        close(entry_fd);
    } close(archive_fd);
    return 0;
}
