// "ANW's Not WAD"

#include "includes.h"
#include "archive.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>

const char ANW_MAGIC[4] = { 'A', 'N', 'W', '0' };
const size_t glob_header_size = sizeof(anw_archive_header);
const size_t entry_header_size = sizeof(anw_entry_header);

opened_anw_archive* anw_open(const char* filename){
    opened_anw_archive* archive;
    archive = malloc(sizeof(opened_anw_archive));
    archive->fd = open(filename, O_RDONLY);
    if (read(archive->fd, &archive->header, glob_header_size) != glob_header_size) { free(archive); return NULL; }
    for (int i = 0; i < 4; i++)
        if (archive->header.magic[i] != ANW_MAGIC[i]) { free(archive); return NULL; }
    if (archive->header.file_count <= 0) { free(archive); return NULL; }
    archive->file_index = 0;
    archive->current_entry_header = malloc(entry_header_size);
    if (read(archive->fd, archive->current_entry_header, entry_header_size) != entry_header_size) { free(archive); return NULL; }
    lseek(archive->fd, archive->current_entry_header->filesize, SEEK_CUR);
    return archive;
}
bool anw_next(opened_anw_archive *archive){
    archive->file_index++;
    if (archive->file_index >= archive->header.file_count) { anw_first(archive); return false; }
    if (read(archive->fd, archive->current_entry_header, entry_header_size) != entry_header_size) return false;
    lseek(archive->fd, archive->current_entry_header->filesize, SEEK_CUR);
    return true;
}
void anw_first(opened_anw_archive *archive){
    archive->file_index = 0;
    lseek(archive->fd, glob_header_size, SEEK_SET);
    read(archive->fd, archive->current_entry_header, entry_header_size);
    lseek(archive->fd, archive->current_entry_header->filesize, SEEK_CUR);
}
memchunk* anw_load(opened_anw_archive *archive){
    memchunk* chunk = malloc(sizeof(memchunk));
    chunk->size = archive->current_entry_header->filesize;
    chunk->pointer = malloc(chunk->size);
    lseek(archive->fd, -((off_t)chunk->size), SEEK_CUR);
    ssize_t count = read(archive->fd, chunk->pointer, chunk->size);
    if (count != chunk->size){
        if (count != -1) lseek(archive->fd, -count, SEEK_CUR);
        free(chunk->pointer); free(chunk); return NULL;
    } return chunk;
}
memchunk* anw_find_and_load(opened_anw_archive *archive, const char *filename){
    anw_first(archive);
    bool file_found = false;
    memchunk* chunk = NULL;
    do if (strcmp(archive->current_entry_header->filename, filename) == 0) {
        file_found = true; break;
    } while (anw_next(archive));
    if (file_found) chunk = anw_load(archive);
    anw_first(archive);
    return chunk;
}
