#include "song-io.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib.h"

int
song_parse_headers(FILE *fp, song_t *song);

int
song_parse_notes(FILE *in, song_t *song);


char* get_song_path_dir(const char* song_file_path){
    char* path_dir = malloc(sizeof(char) * (strlen(song_file_path)+1));
    strcpy(path_dir, song_file_path);
    size_t idx = strlen(path_dir);
    while(path_dir[idx] != '/'){
        path_dir[idx] = '\0';
        idx--;
    }
    return path_dir;
}


int
open_song(const char *file_path, song_t *song)
{
    memset(song, 0, sizeof(song_t));

    FILE *in = fopen(file_path, "r");
    if(in == NULL)
    {
      return ERROR_IO;
    }
    int errcode;

    song->title = NULL;
    song->artist = NULL;
    song->edition = NULL;
    song->language = NULL;
    song->song_file_path = NULL;
    song->song_dir_path = get_song_path_dir(file_path);
    song->cover = NULL;
    song->background = NULL;
    errcode = song_parse_headers(in, song);
    if(errcode != OK) goto error;

    errcode = song_parse_notes(in, song);
    if(errcode != OK) goto error;

    error:
    fclose(in);
    return errcode;
}

void remove_backslash_r_s(char* text){
    size_t tt = strlen(text);
    for(size_t i=0; i<tt; i++){
        if(text[i] == '\r'){
            text[i] = '\0';
        }
    }
}

int
get_header_line(FILE *in, char *buffer)
{
    if(getc(in) != '#'){
        fseek(in, -1, SEEK_CUR);
        return ERROR_NO_MORE_HEADER;
    }
    if( fgets(buffer, 1024, in) == NULL){
        return ERROR_NO_MORE_HEADER;
    }
    unsigned long len = strlen(buffer);
    if(len > 0)
    {
        if(buffer[len-1] == '\n')
        {
            buffer[len-1] = '\0';
        }
        else
        {
            while(getc(in) != '\n');
        }
    }
    return OK;
}

int
song_parse_headers(FILE *in, song_t *song)
{
    char line[1024];
    int errcode;
    while((errcode = get_header_line(in, line)) == OK)
    {
        unsigned long length = strcspn(line, ":");
        char* value = line + length + 1;
        if(!strncmp(line, "ARTIST", length))
        {
            unsigned long n = strlen(value);
            song->artist = calloc(n+1, 1);
            memcpy(song->artist, value, n+1);
            remove_backslash_r_s(song->artist);
            song->artist[n] = '\0';
        }
        else if(!strncmp(line, "TITLE", length))
        {
            unsigned long n = strlen(value);
            song->title = calloc(n+1, 1);
            memcpy(song->title, value, n+1);
            remove_backslash_r_s(song->title);
            song->title[n] = '\0';
            song->title[n-1] = '\0';
        }
        else if(!strncmp(line, "EDITION", length))
        {
            unsigned long n = strlen(value);
            song->edition = calloc(n+1, 1);
            memcpy(song->edition, value, n+1);
            remove_backslash_r_s(song->edition);
            song->edition[n] = '\0';
        }
        else if(!strncmp(line, "LANGUAGE", length))
        {
            unsigned long n = strlen(value);
            song->language = calloc(n+1, 1);
            memcpy(song->language, value, n+1);
            remove_backslash_r_s(song->language);
            song->language[n] = '\0';
        }
        else if(!strncmp(line, "MP3", length))
        {
            unsigned long n = strlen(value);
            song->song_file_path = calloc(n+1, 1);
            memcpy(song->song_file_path, value, n);
            remove_backslash_r_s(song->song_file_path);
            song->song_file_path[n] = '\0';
        }
        else if(!strncmp(line, "COVER", length))
        {
            unsigned long n = strlen(value);
            song->cover = calloc(n+1, 1);
            memcpy(song->cover, value, n+1);
            remove_backslash_r_s(song->cover);
            song->cover[n] = '\0';
        }
        else if(!strncmp(line, "BACKGROUND", length))
        {
            unsigned long n = strlen(value);
            song->background = calloc(n+1, 1);
            memcpy(song->background, value, n+1);
            remove_backslash_r_s(song->background);
            song->background[n] = '\0';
        }
        else if(!strncmp(line, "BPM", length))
        {
            for(size_t i=0; i<strlen(value); i++){
                if(value[i] == ','){
                    value[i] = '.';
                }
            }
            song->bpm = atof(value);
        }
        else if(!strncmp(line, "GAP", length))
        {
            song->gap = atoi(value);
        }
        else
        {
            song->headers_count++;
            song->headers = realloc(
               song->headers,
               song->headers_count*sizeof(song_single_header_t)//song->headers[0])
                    );
            if(song->headers == NULL)
            {
                exit_mem();
            }
            song_single_header_t *hd = &song->headers[song->headers_count-1];

            hd->key = calloc(length+1, 1);
            strncpy(hd->key, line, length);
            hd->key[length] = '\0';

            unsigned long n = strlen(value);
            hd->value = calloc(n+1, 1);
            #pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(hd->value, value, n);
            #pragma GCC diagnostic pop
        }
    }

    if(song->title == NULL){
        printf("Le fichier de son est MALFORME !!!!!! %s \n", song->song_dir_path);
        exit(42);
    }

    if(errcode == ERROR_NO_MORE_HEADER)
      return OK;

    return errcode;
}

void
append_note(song_t *song, song_note_t note)
{
    song->notes_count++;
    song->notes = realloc(
       song->notes,
       song->notes_count*sizeof(song->notes[0])
            );
    if(song->notes == NULL)
    {
        exit_mem();
    }
    song->notes[song->notes_count - 1] = note;
}

int
song_parse_notes(FILE *in, song_t *song)
{
    char buffer[1024];
    int c = getc(in);
    for(; c != 'E'; c = getc(in))
    {
        song_note_t note;
        note.type = c;
        if(c == '-')
        {
            note.text = NULL;
            note.pitch=0;
            note.length=0;
            note.start=0;
            int end = -1;
            if(fscanf(in, "%d %d", &note.start, &end) == 2)
            {
                note.length = end - note.start;
                //ignore end of line
                while(getc(in) != '\n') ;
            }
            // else the end of line has been consumed by the scanf

        }
        else
        {

            int __attribute__((unused)) x = fscanf(in, "%d %d %d", &note.start, &note.length, &note.pitch);
            if(getc(in) !='\n')
            {
                char* __attribute__((unused)) x = fgets(buffer, 1024, in);
                buffer[strcspn(buffer, "\n\r")] = '\0';
                size_t len = strlen(buffer);
                note.text = calloc(1, len+1);
                // strncpy(note.text, buffer, len);
                memcpy(note.text, buffer, len);
            }
            else
            {
                note.text = NULL;
            }
        }
        append_note(song, note);
    }
    return OK;
}



int
save_song(const song_t *song, const char *file_path)
{
    FILE* fp = fopen(file_path, "w");

    if(song->title != NULL)
        fprintf(fp, "#TITLE:%s\n", song->title);
    if(song->artist != NULL)
        fprintf(fp, "#ARTIST:%s\n", song->artist);
    if(song->edition != NULL)
        fprintf(fp, "#EDITION:%s\n", song->edition);
    if(song->language != NULL)
        fprintf(fp, "#LANGUAGE:%s\n", song->language);
    if(song->song_file_path != NULL)
        fprintf(fp, "#MP3:%s\n", song->song_file_path);
    if(song->cover != NULL)
        fprintf(fp, "#COVER:%s\n", song->cover);
    if(song->background != NULL)
        fprintf(fp, "#BACKGROUND:%s\n", song->background);
    //
    fprintf(fp, "#BPM:%lf\n", song->bpm);
    fprintf(fp, "#GAP:%d\n", song->gap);
    //
    for(size_t i=0; i<song->notes_count; i++){
        song_note_t note = song->notes[i];
        if(note.type == '-'){
            if(note.length == 0){
                fprintf(fp, "%c %d\n", note.type, note.start);
            }
            else{
                fprintf(fp, "%c %d %d\n", note.type, note.start, note.start+note.length);
            }
        }
        else{
            fprintf(fp, "%c %d %d %d %s\n", note.type, note.start, note.length, note.pitch, note.text);
        }
    }

    fprintf(fp, "E\n");

    fclose(fp);

    return OK;
}
