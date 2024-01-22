#include "utils.h"
#include "song.h"
#include <stdio.h>
#include <stdlib.h>


void
exit_mem(void) {
    fputs("Not enough memory\n", stderr);
    exit(42);
}


void
print_song_info(const song_t* song)
{
    printf(
       "title:  %s\n"
       "artist: %s\n"
       "bpm:    %f\n"
       "gap:    %i\n",
              song->title,
              song->artist,
              song->bpm,
              song->gap);

    for(size_t idx=0; idx < song->headers_count; idx++)
    {
        printf("%s:%s\n", song->headers[idx].key,
                        song->headers[idx].value);
    }
}


void
print_song_notes(const song_t* song, bool yellow_gold, bool lyrics)
{
    for(size_t idx=0; idx < song->notes_count; idx++)
    {
        char * text = song->notes[idx].text != NULL
                      ? song->notes[idx].text
                      : "";

        //
        if(yellow_gold && song->notes[idx].type == '*'){
            printf("\e[93m");
        }
        //
        if(!lyrics){
            printf("%f -> %f : %s\n",
                beat_to_ms_pos(song, song->notes[idx].start)/1000,
                beat_to_ms(song, song->notes[idx].length)/1000,
                text);
        }
        else{
            if(song->notes[idx].type == '-'){
                printf("\n");
            }
            else{
                printf("%s", text);
            }
        }
        //
        if(yellow_gold && song->notes[idx].type == '*'){
            printf("\e[0m");
        }
    }
    printf("\n");
}

void
multiply_bpm(song_t* song, double f){
    song->bpm *= f;
    for(size_t i=0; i<song->notes_count; i++){
        // printf("BEFORE : \n");
        // printf("   - start %d length %d\n", song->notes[i].start, song->notes[i].length);
        // printf("   - %f -> %f : %s\n",
        //     beat_to_ms_pos(song, song->notes[i].start)/1000,
        //     beat_to_ms(song, song->notes[i].length)/1000,
        //     "TEST");
        song->notes[i].start = (int)((double)song->notes[i].start*f);
        song->notes[i].length = (int)((double)song->notes[i].length*f);
        // printf("AFTER : \n");
        // printf("   - start %d length %d\n", song->notes[i].start, song->notes[i].length);
        // printf("   - %f -> %f : %s\n\n\n",
        //     beat_to_ms_pos(song, song->notes[i].start)/1000,
        //     beat_to_ms(song, song->notes[i].length)/1000,
        //     "TEST");
    }
}

void correct_song(song_t* song){
    int compteur_notes_changees=0;
    //TODO, flemme de le faire maintenant
    for(size_t i = 0; i<song->notes_count; i++){
        //TODO
    }
    //
    printf("%d notes ont étés changées pour rendre la chanson plus agréable à chanter!\n", compteur_notes_changees);
}

