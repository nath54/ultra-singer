#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

typedef struct song_t song_t;


/**
 * Enum containing all the possible values returned by
 * a function.
 */
enum {
    OK,
    ERROR_PARSE,
    ERROR_NO_MORE_HEADER,
    ERROR_IO
};


/**
 * @brief exit the program printing a message telling the user
 * that there is not enough memory
 */
void
exit_mem(void);

/**
 * @brief print in the standard output the information related to the song
 * @param song
 */
void
print_song_info(const song_t* song);


/**
 * @brief print in the standard output the notes of the song
 * @param song
 */
void
print_song_notes(const song_t* song, bool yellow_gold, bool lyrics);




/**
 * @brief multiply the bpm of the song by a factor
 * @param song
 */
void
multiply_bpm(song_t* song, double f);


#endif
