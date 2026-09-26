// Header file for the "ANW's Not WAD" format
#ifndef ARCHIVE_H
#include <stdint.h>
typedef struct anw_archive_header {
    char magic[4];                  // must be equal to ANW_MAGIC
    uint16_t file_count;            // who in their right mind would cram more
                                    // than 65535 files into an archive?
} anw_archive_header;
typedef struct anw_entry_header {   // as written into the file
    char filename[32];              // up to 31 bytes
    uint32_t filesize;              // size of file data (LE)
} anw_entry_header;
typedef struct opened_anw_archive {
    int fd;
    anw_archive_header header;
    uint16_t file_index;
    anw_entry_header *current_entry_header;
} opened_anw_archive;
opened_anw_archive* anw_open(const char* filename);
bool anw_next(opened_anw_archive *archive);
void anw_first(opened_anw_archive *archive);
void* anw_load(opened_anw_archive *archive);
#define ARCHIVE_H
#else
#warning "Multiple inclusions of archive.h"
#endif
