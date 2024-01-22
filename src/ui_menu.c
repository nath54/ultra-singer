#include "ui_menu.h"


typedef struct res_menu{
    Texture2D i_mic;
} res_menu;

void template_menu_page(Window* win){
    // RESOURCES
    //...
    // UI
    Ui* ui = init_UI(0, BLACK);
    // ...
    // Mouse position
    Vector2 mpos = {0.0f, 0.0f};
    bool clicked = false;
    //
    bool fini = false;
    //
    double t1=GetTime();
    double t2=GetTime();
    //
    while (!WindowShouldClose() && !fini){
        // EVENTS
        mpos = GetMousePosition();
        if(IsKeyReleased(KEY_ESCAPE)){
            fini=true;
        }
        if(IsMouseButtonReleased(0)){
            if(!clicked){
                clicked = true;

                switch ( test_collision(ui, &mpos, win) )
                {
                // ...
                default:
                    break;
                }
            }
        }
        else{
            clicked = false;
        }
        // AFF

        t2=GetTime();
        if(t2-t1 >= 0.05){
            t1 = t2;
            aff_UI(ui, &mpos, win);
        }
    }
    // Free
    // Free resources ...
    free_UI(ui);
}

void set_page_text(Ui* ui, Window* win){
    int crt = 1 + win->crt_music_pages;
    // Crt page
    if(crt < 10){
        ui->elts[2]->text[5] = '0'+crt % 10;
        ui->elts[2]->text[6] = ' ';
    }
    else{
        ui->elts[2]->text[5] = '0'+(crt/10)%10;
        ui->elts[2]->text[6] = '0'+crt%10;
    }
}

void load_page_songs_menu(
    Window* win,
    SA_DynamicArray* lst_songs,
    size_t* nb_songs,
    SA_DynamicArray* lst_covers,
    size_t* nb_covers,
    Texture2D* im_note,
    Ui* ui
){

    set_page_text(ui, win);

    win->nb_song_on_page = 0;

    for(int i=0; i < PAGE_SONG_SIZE; i++){
        int id_song_page = win->crt_music_pages*PAGE_SONG_SIZE + i;
        if(id_song_page < (int)*nb_songs){
            // On ajoute un bouton
            add_bt_text(ui, "Sing!", false, 80, 0, 20, i * 50, WHITE, font_size(win, 30), font_size(win, 35)); // 5 + 3*i -> song(i) title & artist
            //
            song_t* song = (song_t*)(SA_dynarray_get(song_t*, lst_songs, id_song_page));
            char* texte = (char*)calloc(sizeof(char), 1000);
            // printf("Titre du son : %s, %ld\n", song->title, strlen(song->title));
            // printf("Titre du son : %s\n", song->title);
            strcat(texte, song->title);
            strcat(texte, " - ");
            strcat(texte, song->artist);
            // 
            add_text(ui, texte, true, 15, 45, 20, i * 50, WHITE, font_size(win, 30)); // 6 + 3*i -> song(i) title & artist
            //
            if(song->cover == NULL){
                add_texture(ui, *im_note, 15, 0, 20, i * 50, WHITE); // 7 + 3*i -> song(i) image
            }
            else{
                char img_cover_path[1000] = "";
                strcat(img_cover_path, song->song_dir_path);
                strcat(img_cover_path, song->cover);
                // printf("Cover ! %s \n", img_cover_path);
                Image img_cover = LoadImage(img_cover_path);
                if(img_cover.data == NULL){
                    add_texture(ui, *im_note, 15, 0, 20, i * 50, WHITE); // 7 + 3*i -> song(i) image
                }
                else{
                    ImageResize(&img_cover, 30, 30);
                    Texture2D im_cover = LoadTextureFromImage(img_cover);
                    add_texture(ui, im_cover, 15, 0, 20, i * 50, WHITE); // 7 + 3*i -> song(i) image
                    UnloadImage(img_cover);
                    SA_dynarray_append(Texture2D, lst_covers, im_cover);
                    nb_covers++;
                }
            }
            //
            win->nb_song_on_page++;
            //
        }
    }
}

void clean_page_songs_menu(Window* win, Ui* ui){
    while(win->nb_song_on_page > 0){
        ui_free_last_element(ui); // On supprime le cover
        ui_free_last_element(ui); // On supprime le texte
        ui_free_last_element(ui); // On supprime le bouton
        win->nb_song_on_page--;
    }
}

int ceild(double a){
    int ai = (int)a;
    if((double)ai == a){
        return ai;
    }
    return ai+1;
}

void load_song_menu(Window* win){
    SA_DynamicArray* lst_songs = list_songs();
    size_t nb_songs = SA_dynarray_size(lst_songs);
    win->nb_music_pages = ceild((double)nb_songs / (double)PAGE_SONG_SIZE);

    // RESOURCES
    SA_DynamicArray* lst_covers = SA_dynarray_create(Texture2D);
    size_t nb_covers = 0;
    Image img_note = LoadImage("res/music_note.png");
    ImageResize(&img_note, 30, 30);
    Texture2D im_note = LoadTextureFromImage(img_note);
    UnloadImage(img_note);
    
    // UI
    char* txt_nb_pages = malloc(sizeof(char)*14);
    txt_nb_pages[0] = '\0';
    strcat(txt_nb_pages, "Page 0 / 0  ");
    Ui* ui = init_UI(35, BLACK);
    add_text(ui, "Songs!", false, 45, 0, 7, 0, PURPLE, font_size(win, 45)); // 0 - Titre
    add_bt_text(ui, "Menu", false, 5, 0, 5, 0, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 1 -> bt menu
    add_text(ui, txt_nb_pages, false, 45, 0, 90, 0, LIGHTGRAY, font_size(win, 30)); // 2 -> text page
    add_bt_text(ui, "(page précédente)", false, 20, 0, 90, 10, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 3 -> bt page précédente
    add_bt_text(ui, "(page suivante)", false, 70, 0, 90, 10, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 4 -> bt page suivante

    // Nb pages tot
    if(win->nb_music_pages < 10){
        ui->elts[2]->text[9] = '0'+win->nb_music_pages % 10;
        ui->elts[2]->text[10] = ' ';
    }
    else{
        ui->elts[2]->text[9] = '0'+(win->nb_music_pages/10)%10;
        ui->elts[2]->text[10] = '0'+win->nb_music_pages%10;
    }

    // printf("NB SONGS : %ld \n", nb_songs);

    load_page_songs_menu(
        win,
        lst_songs,
        &nb_songs,
        lst_covers,
        &nb_covers,
        &im_note,
        ui
    );

    // Mouse position
    Vector2 mpos = {0.0f, 0.0f};
    bool clicked = false;
    //
    bool fini = false;
    //
    double t1=GetTime();
    double t2=GetTime();
    //
    while (!fini && !win->closed){
        if(WindowShouldClose()){
            win->closed = true;
        }
        // EVENTS
        mpos = GetMousePosition();
        if(IsKeyReleased(KEY_ESCAPE)){
            fini=true;
        }
        if(IsMouseButtonReleased(0)){
            if(!clicked){
                clicked = true;

                int id_elt = test_collision(ui, &mpos, win);
                
                if(id_elt == 1){
                    printf("Click Bt Menu !\n");
                    fini = true;
                }
                else if(id_elt == 3){ // Page prec
                    if(win->crt_music_pages > 0){
                        //
                        clean_page_songs_menu(win,ui);
                        //
                        win->crt_music_pages--;
                        //
                        load_page_songs_menu(
                            win,
                            lst_songs,
                            &nb_songs,
                            lst_covers,
                            &nb_covers,
                            &im_note,
                            ui
                        );
                    }
                }
                else if(id_elt == 4){ // Page suiv
                    if(win->crt_music_pages < win->nb_music_pages-1){
                        //
                        clean_page_songs_menu(win, ui);
                        //
                        win->crt_music_pages++;
                        //
                        load_page_songs_menu(
                            win,
                            lst_songs,
                            &nb_songs,
                            lst_covers,
                            &nb_covers,
                            &im_note,
                            ui
                        );
                    }
                }
                else if(id_elt > 4){
                    if((id_elt - 5) % 3 == 0){
                        int id_song = (id_elt - 5) / 3;
                        printf("Bt song %d\n", id_song);
                        song_t* song = SA_dynarray_get(song_t*, lst_songs, id_song+win->crt_music_pages*PAGE_SONG_SIZE);
                        game(win, song);
                    }
                }

            }
        }
        else{
            clicked = false;
        }
        // AFF

        t2=GetTime();
        if(t2-t1 >= 0.05){
            t1 = t2;
            aff_UI(ui, &mpos, win);
        }
    }
    // Free
    // Free resources ...
    UnloadTexture(im_note);
    free_UI(ui);
    free(txt_nb_pages);
    //
    SA_dynarray_free(&lst_covers);
    SA_dynarray_free(&lst_songs);
}

void main_menu(Window* win){
    // RESOURCES
    res_menu res;
    Image img_mic = LoadImage("res/mic.png");
    ImageResize(&img_mic, prct_w(40, win), prct_w(40, win));
    res.i_mic = LoadTextureFromImage(img_mic);
    UnloadImage(img_mic);
    // UI
    Ui* ui = init_UI(5, BLACK);
    add_text(ui, "Ultra Singer!", false, 35, 0, 15, 0, PURPLE, font_size(win, 80));
    add_bt_text(ui, "Play!", false, 55, 0, 40, 0, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 1 -> bt play
    add_bt_text(ui, "Settings", false, 55, 0, 40, 70, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 2 -> bt settings
    add_bt_text(ui, "Exit", false, 55, 0, 40, 140, LIGHTGRAY, font_size(win, 30), font_size(win, 35)); // 3 -> bt exit
    add_texture(ui, res.i_mic, 5, 0, 35, 0, WHITE);
    // Mouse position
    Vector2 mpos = {0.0f, 0.0f};
    bool clicked = false;
    //
    bool fini = false;
    //
    double t1=GetTime();
    double t2=GetTime();
    //
    while (!fini && !win->closed){
        if(WindowShouldClose()){
            win->closed = true;
        }
        // EVENTS
        mpos = GetMousePosition();
        if(IsKeyReleased(KEY_ESCAPE)){
            fini=true;
        }
        if(IsMouseButtonReleased(0)){
            if(!clicked){
                clicked = true;

                switch ( test_collision(ui, &mpos, win) )
                {
                case 1:
                    printf("Click play!\n");
                    load_song_menu(win);
                    break;
                
                case 2:
                    printf("Click settings!\n");
                    break;

                case 3:
                    fini = true;
                    break;

                default:
                    break;
                }
            }
        }
        else{
            clicked = false;
        }
        // AFF

        t2=GetTime();
        if(t2-t1 >= 0.05){
            t1 = t2;
            aff_UI(ui, &mpos, win);
            //aff_main_menu(&res, &mpos);
        }
    }
    // Free
    UnloadTexture(res.i_mic);
    free_UI(ui);
}

void main_ui(){

    Window* win = init_window();

    main_menu(win);

    close_window(win);
}
