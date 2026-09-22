#include "archive.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>

const size_t header_size = sizeof(pkf_archive_header);

opened_pkf_archive pkf_open(const char* filename){
    opened_pkf_archive archive;
    archive.fd = open(filename, O_RDONLY);
    if (read(archive.fd, &archive.header, header_size) != header_size) goto error;
    for (int i = 0; i < 3; i++)
        if (archive.header.magic[i] != PKF_MAGIC[i]) goto error;
    if (archive.header.file_count <= 0) goto error;
    archive.file_index = 0;
    return archive;
error:
    handle_init_failure("Failed to load (an) archive file!");
}
bool pkf_next(opened_pkf_archive archive){
    if (++archive.file_index < archive.header.file_count) return true;
    else { archive.file_index = 0; return false; }
}
pkf_entry pkf_load(opened_pkf_archive archive){

}
