#include "archive.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>

const size_t glob_header_size = sizeof(pkf_archive_header);
const size_t entry_header_size = sizeof(pkf_entry_header);

opened_pkf_archive pkf_open(const char* filename){
    opened_pkf_archive archive;
    archive.fd = open(filename, O_RDONLY);
    if (read(archive.fd, &archive.header, glob_header_size) != glob_header_size) goto error;
    for (int i = 0; i < 3; i++)
        if (archive.header.magic[i] != PKF_MAGIC[i]) goto error;
    if (archive.header.file_count <= 0) goto error;
    archive.file_index = 0;
    return archive;
error:
    handle_init_failure("Failed to load (an) archive file!");
}
bool pkf_next(opened_pkf_archive archive){
    archive.file_index++;
    if (archive.file_index >= archive.header.file_count) { archive.file_index = 0; return false; }
    if (read(archive.fd, &archive.current_entry_header, entry_header_size) != entry_header_size) return false;
    return true;
}
void* pkf_load(opened_pkf_archive archive){
    void* data;
    if (read(archive.fd, data, archive.current_entry_header.filesize) != archive.current_entry_header.filesize) { free(data); return NULL; }
    return data;
}
