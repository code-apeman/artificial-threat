#ifndef ARCHIVE_H
#include <stdint.h>
const char PKF_MAGIC[3] = { 'P', 'K', 'F' };
typedef struct pkf_archive_header {
    const char magic[3];            // must be equal to PKF_MAGIC
    uint16_t file_count;            // who in their right mind would cram more
                                    // than 65535 files into an archive?
} pkf_archive_header;
typedef struct pkf_entry_header {   // as written into the file
    char filename[32];              // up to 31 bytes
    uint32_t filesize;              // size of file data (LE)
} pkf_entry_header;
typedef struct opened_pkf_archive {
    int fd;
    pkf_archive_header header;
    uint16_t file_index;
    pkf_entry_header current_entry_header;
} opened_pkf_archive;
opened_pkf_archive pkf_open(const char* filename);
bool pkf_next(opened_pkf_archive archive);
pkf_entry pkf_load(opened_pkf_archive archive);
#define ARCHIVE_H
#else
#warning "Multiple inclusions of archive.h"
#endif
