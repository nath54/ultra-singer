#ifndef SONG_H
#define SONG_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct song_single_header_t {
    char *key;
    char *value;
} song_single_header_t;


typedef struct song_note_t {
    char type;
    char *text;
    int pitch;
    int start;
    int length;
    //
} song_note_t;


typedef struct song_t {
    double bpm;
    int    gap;
    char*  title;
    char*  artist;
    char*  song_dir_path;
    char*  song_file_path;
    char*  edition;
    char*  language;
    char*  cover;
    char*  background;

    size_t                notes_count;
    song_note_t*          notes;

    size_t                headers_count;
    song_single_header_t* headers;
} song_t;

/**
 * @brief Converts a beat to a number of milliseconds from the begining of the song
 * @param song
 * @param beat
 * @return the number of milliseconds from the begining
 */
double
beat_to_ms_pos(const song_t *song, int beat);

/**
 * @brief Converts a beat to a number of milliseconds
 * @param song
 * @param beat
 * @return the equivalent number of milliseconds
 */
double
beat_to_ms(const song_t *song, int beat);

/**
 * @brief free the song
 * @param song
 */
void
song_free(song_t *song);

#endif
