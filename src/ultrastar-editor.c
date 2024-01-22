#include <stdio.h>
#include <SA/SA.h>
#include "args-parser.h"
#include "song.h"
#include "utils.h"
#include "song-io.h"
#include "ui_menu.h"


int main(int argc, char ** argv)
{
    args_t args;
    args.output_file_path = NULL;

    SA_init();

    parse_args(argc, argv, &args);

    if(args.flags & INTERFACE_GRAPHIQUE){
        main_ui();
    }
    else{
        if(!args.file_path)
        {
            puts("l'option -f est requise");
            print_usage();
            exit(1);
        }
        else{
            song_t song;
            open_song(args.file_path, &song);

            if(args.mult_bpm != 1.0){
                multiply_bpm(&song, args.mult_bpm);
            }

            if(args.output_file_path != NULL)
            {
                save_song(&song, args.output_file_path);
            }

            if(args.flags & PRINT_SONG_INFO)
            {
                print_song_info(&song);
            }
            if(args.flags & PRINT_SONG_NOTES)
            {
                print_song_notes(
                    &song,
                    args.flags & PRINT_GOLD_NOTE_YELLOW,
                    args.flags & PRINT_LYRICS
                );
            }

            song_free(&song);
        }
    }

    SA_destroy();
}
