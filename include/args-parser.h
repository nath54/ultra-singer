#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef enum flags_t {
  NO_FLAGS = 0,
  PRINT_SONG_INFO = 1,
  PRINT_SONG_NOTES = 1 << 1,
  PRINT_GOLD_NOTE_YELLOW = 1 << 2,
  PRINT_LYRICS = 1 << 3,
  INTERFACE_GRAPHIQUE = 1 << 4
} flags_t;

typedef struct args_t {
  char * file_path;
  char * output_file_path;
  flags_t flags;
  double mult_bpm;
} args_t;


/**
 * @brief Parse les arguments de ligne de commande
 * et definie les attributs de game en fonction.
 * @param argc (standard main argument)
 * @param argv (standard main argument)
 * @param args will receive the argument values
 * @return OK if everything went well or ERROR_* if there was an error
 */
int
parse_args(int argc,  char ** argv, args_t *args);


/**
 * @brief print the usage
 */
void
print_usage(void);


#endif
