#ifndef ARCHIVE_H
#define CPF_MAGIC { 'C', 'P', 'F' }
typedef struct cpf_entry_header {
    char filename[32];          // up to 31 bytes
    uint32_t filesize;          // size of compressed file data on disk (LE)
    uint32_t buffersize;        // size of decompressed file data in memory (LE)
} cpf_entry_header
typedef struct cpf_entry {
    cpf_entry_header header;
    void* data;
} cpf_entry
typedef struct opened_cpf_archive {
    int fd;                     // file descriptor
    uint32_t position;          // current byte position in file
    cpf_entry current_file;
}
opened_cpf_archive cpf_open(const char* filename);
#define ARCHIVE_H
#else
#warning "Multiple inclusions of archive.h"
#endif
