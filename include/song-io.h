#ifndef SONG_IO_H
#define SONG_IO_H
#include "song.h"

/**
 * @brief open an ultrastar song
 * @param file_path
 * @param a pointer the variable that receives the song
 * @return OK if everything went well or ERROR_* if there was an error
 */
int
open_song(const char *file_path, song_t *song);

/**
 * @brief save the given song in the given file path
 * @param song
 * @param file_path
 * @return OK if everything went well or ERROR_* if there was an error
 */
int
save_song(const song_t *song, const char *file_path);

#endif
