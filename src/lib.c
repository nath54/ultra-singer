#include "lib.h"

bool endswith(char* s, char* e){
    size_t ls = strlen(s);
    size_t le = strlen(e);
    for(size_t i=ls-le; i < ls; i++){
        if(s[i] != e[i - (ls-le)]){
            return false;
        }
    }
    //
    return true;
}

// Renvoie une liste de toutes les musiques
SA_DynamicArray* list_songs(){

    SA_DynamicArray* lst = SA_dynarray_create(song_t*);

    DIR *dp;
    struct dirent *ep;
    dp = opendir ("./songs/");
    if (dp != NULL)
    {
        while ((ep = readdir (dp)) != NULL){
            if(!strcmp(ep->d_name, "..") || !strcmp(ep->d_name, ".")){
                continue;
            }
            // On a un directory
            puts (ep->d_name);
            // On va chercher le seul fichier qui se termine par un .txt
            DIR *dp2;
            struct dirent *ep2;
            char path_dir_song[1000] = "./songs/";
            strcat(path_dir_song, ep->d_name);
            strcat(path_dir_song, "/");
            dp2 = opendir (path_dir_song);
            if (dp2 != NULL)
            {
                while ((ep2 = readdir (dp2)) != NULL){

                    if(!strcmp(ep2->d_name, "..") || !strcmp(ep2->d_name, ".")){
                        continue;
                    }
                    puts (ep2->d_name);
                    if( endswith(ep2->d_name, ".txt") ){
                        // On calcule le chemin du fichier
                        char* song_path = (char*)malloc(sizeof(char)*1000);
                        song_path[0] = '\0';
                        strcat(song_path, SONGS_PATH);
                        strcat(song_path, ep->d_name);
                        strcat(song_path, "/");
                        strcat(song_path, ep2->d_name);
                        // On a notre fichier de son
                        song_t* song = malloc(sizeof(song_t));
                        open_song(song_path, song);
                        SA_dynarray_append(song_t*, lst, song);
                        break;
                    }
                }
            }
            closedir(dp2);
        }

            
        closedir (dp);
    }
    else
    {
        perror ("Couldn't open the directory");
    }

    return lst;
}