#include "args-parser.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int parse_args(int argc,  char ** argv, args_t *args)
{
  args->file_path = NULL;
  args->flags = NO_FLAGS;
  args->mult_bpm = 1;

  opterr = 0;
  char c;

  while ((c = getopt (argc, argv, "f:hinylgb:o:")) != -1)
  {
    switch (c)
      {
      case 'h':
        print_usage();
        exit(0);
      case 'i':
        args->flags |= PRINT_SONG_INFO;
        break;
      case 'n':
        args->flags |= PRINT_SONG_NOTES;
        break;
      case 'y':
        args->flags |= PRINT_GOLD_NOTE_YELLOW;
        break;
      case 'l':
        args->flags |= PRINT_LYRICS;
        break;
      case 'g':
        args->flags |= INTERFACE_GRAPHIQUE;
        break;
      case 'b':
        args->mult_bpm = atof(optarg);
        break;
      case 'f':
        args->file_path = optarg;
        break;
      case 'o':
        args->output_file_path = optarg;
        break;


      case '?':

        if (optopt == 'f')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
        {
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        }
        else
        {
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        }

        print_usage();
        exit(1);

      default:
        fprintf (stderr,
                 "Unknown option character `%c'.\n",
                 c);
        print_usage();
        exit(1);
      }
    }

    return OK;
}


void print_usage(void)
{
  puts(
  "Usage:\n"
  "-f FICHIER    fichier ultrastar\n"
  "              contenue dans FICHIER\n"
  "-i            Afficher les informations de la chanson\n"
  "-n            Afficher les notes de la chanson\n"
  "-y            Afficher les notes de type gold en jaune\n"
  "-l            Afficher seulement les paroles de la chanson\n"
  "-o FICHIER    chemin du fichier out\n"
  "              sauvergarder la chanson\n"
  "-h            afficher l'aide");
}
