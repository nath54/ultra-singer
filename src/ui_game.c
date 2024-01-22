#include "ui_game.h"

#define UI_BASE_ELEMENTS 1
#define UI_FRONT_ELEMENTS 4

#define DEB_SING_POS_RELATIVE 55
#define END_SING_POS_RELATIVE 45

void aff_game(bool* pause){
    if(!(*pause)){

    }
    else{

    }
}

Ui_Element* get_notes_elt_id(Ui* ui, size_t id_note){
    if(id_note >= ui->nb_elts){
        fprintf(stderr, "Error, depassement! Asked id: %ld, max id : %ld\n", id_note, ui->nb_elts);
        return NULL;
    }
    return ui->elts[UI_BASE_ELEMENTS + id_note];
}

double real_beat_to_ms_pos(song_t* song, size_t id_note, bool with_length){
    if(with_length){
        return (double)song->gap + beat_to_ms(song, song->notes[id_note].start+song->notes[id_note].length) / 4.0;
    }
    else{
        return (double)song->gap + beat_to_ms(song, song->notes[id_note].start) / 4.0;
    }
}

void enter_pause(Ui* ui){
    for(int i=1; i<=4; i++){
        ui->elts[ui->nb_elts-1 - UI_FRONT_ELEMENTS + i]->visible = true;
    }
}

void quit_pause(Ui* ui){
    for(int i=1; i<=4; i++){
        ui->elts[ui->nb_elts-1 - UI_FRONT_ELEMENTS+i]->visible = false;
    }
}

void game(Window* win, song_t* song){
    //
    if(song == NULL){
        return;
    }
    //
    double speed = 5.0;

    // Initialize audio device
    float crt_time = 0;
    float prev_crt = 0;
    InitAudioDevice();

    // RESOURCES
    char* path_music_file = malloc(sizeof(char)*2000);
    path_music_file[0] = '\0';
    strcat(path_music_file, song->song_dir_path);
    strcat(path_music_file, song->song_file_path);
    Music music = LoadMusicStream(path_music_file);

    // UI
    Ui* ui = init_UI(UI_BASE_ELEMENTS + song->notes_count + 10, BLACK);
    add_progress_bar(ui, 0, 0, 0, 0, 100, 0, 0, 10, WHITE); // 0 = Progress bar

    // -- On ajoute toutes les notes
    size_t id_note = 0;
    size_t id_deb_sentence = 0;
    size_t id_fin_sentence = 0;

    int posY = 0;
    int debX = 0;
    int precX = 0;
    int taille_tot = 0;
    while(id_note < song->notes_count){
        if(song->notes[id_note].type == '-'){
            //
            id_fin_sentence = id_note;
            //
            debX = (win->width - taille_tot) / 2;
            precX = 0;
            posY = win->height/2 + real_beat_to_ms_pos(song, id_deb_sentence, false) / speed;
            for(size_t idn=id_deb_sentence; idn<id_fin_sentence; idn++){
                if(song->notes[idn].text != NULL){
                    add_text(ui, song->notes[idn].text, false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
                    precX += strlen(song->notes[idn].text) * (30 / win->font_size_multiplier);
                }
                else{
                    add_text(ui, &song->notes[id_note].type, false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
                    precX += (30 / win->font_size_multiplier);
                }
            }
            debX = (win->width - (30 / win->font_size_multiplier)) / 2;
            precX = 0;
            posY = win->height/2 + real_beat_to_ms_pos(song, id_fin_sentence-1, false) / speed;
            add_text(ui, " ", false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
            precX += (30 / win->font_size_multiplier);
            //
            taille_tot = 0;
            id_deb_sentence = id_fin_sentence + 1;
            id_fin_sentence = id_deb_sentence;
            id_note++;
            //
            continue;
        }
        //
        taille_tot += strlen(song->notes[id_note].text) * (30 / win->font_size_multiplier);
        //
        id_note++;
    }

    // On fait la dernière phrase
    if(id_fin_sentence != id_note){
        id_fin_sentence = id_note;
        //
        debX = (win->width - taille_tot) / 2;
        precX = 0;
        posY = win->height/2 + real_beat_to_ms_pos(song, id_deb_sentence, false) / speed;
        for(size_t idn=id_deb_sentence; idn<id_fin_sentence; idn++){
            if(song->notes[idn].text != NULL){
                add_text(ui, song->notes[idn].text, false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
                precX += strlen(song->notes[idn].text) * (30 / win->font_size_multiplier);
            }
            else{
                add_text(ui, &song->notes[id_note].type, false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
                precX += (30 / win->font_size_multiplier);
            }
        }
        debX = (win->width - (30 / win->font_size_multiplier)) / 2;
        precX = 0;
        posY = win->height/2 + real_beat_to_ms_pos(song, id_fin_sentence+1, false) / speed;
        add_text(ui, " ", false, 0, debX+precX, 0, posY, WHITE, font_size(win, 50));
        precX += (30 / win->font_size_multiplier);
    }

    // * UI : Menu Pause
    add_rect(ui, 20, 0, 20, 0, 60, 0, 60, 0, (Color){2, 2, 2, 200}); // ui->nb_elts - 1 - UI_FRONT_ELTS = Rect
    add_text(ui, "Pause", false, 30, 0, 30, 0, WHITE, 40); // ui->nb_elts - 1 - UI_FRONT_ELTS - 1 = Titre pause
    add_bt_text(ui, "Resume", false, 40, 0, 50, 0, WHITE, 30, 35); // ui->nb_elts - 1 - UI_FRONT_ELTS - 2 = Bt resume
    add_bt_text(ui, "Menu", false, 40, 0, 60, 0, WHITE, 30, 35); // ui->nb_elts - 1 - UI_FRONT_ELTS - 3 = Bt Menu
    quit_pause(ui);
    const int id_bt_resume = ui->nb_elts-1 - UI_FRONT_ELEMENTS + 3;
    const int id_bt_menu = ui->nb_elts-1 - UI_FRONT_ELEMENTS + 4;

    // Mouse position
    Vector2 mpos = {0.0f, 0.0f};
    bool clicked = false;
    //
    PlayMusicStream(music);

    // Time played normalized [0.0f..1.0f]
    float timePlayed = 0.0f;

    // Music playing paused
    bool pause = false;
    bool fini = false;
    //
    double t1=GetTime()*1000.0;
    double t2=GetTime()*1000.0;
    // 
    float next_letter_sung = song->gap;
    size_t id_letter_sung = 0;
    size_t deb_crt_sentence_sung = 0;
    size_t fin_crt_sentence_sung = 0;
    size_t deb_next_sentence_sung = 0;
    size_t fin_next_sentence_sung = 0;
    double depY_avant = 0;
    double depY_entrain = 0;
    double deltaY_entrain = prct_h(DEB_SING_POS_RELATIVE, win) - prct_h(END_SING_POS_RELATIVE, win);
    double depY_next = 0;

    // On va chercher la current sentence chantee
    fin_crt_sentence_sung = deb_crt_sentence_sung;
    while(fin_crt_sentence_sung < song->notes_count && song->notes[fin_crt_sentence_sung].type != '-'){
        fin_crt_sentence_sung++;
    }
    fin_crt_sentence_sung--;

    // On va cherche la sentence d'après
    if(fin_crt_sentence_sung < song->notes_count - 2){
        deb_next_sentence_sung = fin_crt_sentence_sung + 2;
        fin_next_sentence_sung = deb_next_sentence_sung;
        while(fin_next_sentence_sung < song->notes_count && song->notes[fin_next_sentence_sung].type != '-'){
            fin_next_sentence_sung++;
        }
        fin_next_sentence_sung--;
    }
    else{
        deb_next_sentence_sung = 0;
        fin_next_sentence_sung = 0;
    }

    //
    depY_avant = get_notes_elt_id(ui, deb_crt_sentence_sung)->posY - prct_h(DEB_SING_POS_RELATIVE, win);
    depY_avant /= real_beat_to_ms_pos(song, deb_crt_sentence_sung, false) - crt_time;
    depY_entrain = deltaY_entrain / (real_beat_to_ms_pos(song, fin_crt_sentence_sung, true) - real_beat_to_ms_pos(song, deb_crt_sentence_sung, false));
    if(deb_next_sentence_sung != 0){
        depY_next = get_notes_elt_id(ui, deb_next_sentence_sung)->posY - prct_h(DEB_SING_POS_RELATIVE, win);
        depY_next /= real_beat_to_ms_pos(song, deb_next_sentence_sung, false) - crt_time;
    }
    else{
        depY_next = 0;
    }

    // float last_update = 0;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!fini && timePlayed < 1.0f)    // Detect window close button or ESC key
    {
        if(WindowShouldClose()){
            win->closed = true;
        }

        // EVENTS
        mpos = GetMousePosition();
        if(IsKeyReleased(KEY_ESCAPE)){
            fini=true;
        }

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause){
                PauseMusicStream(music);
                enter_pause(ui);
            }
            else{
                ResumeMusicStream(music);
                quit_pause(ui);
            }
        }

        // Mouse Click
        if(IsMouseButtonReleased(0)){
            if(!clicked){
                clicked = true;

                int col = test_collision(ui, &mpos, win);

                if(col!=-1){

                    if(col==id_bt_resume){
                        pause = false;
                        ResumeMusicStream(music);
                        quit_pause(ui);
                    }

                    else{
                        if(col == id_bt_menu){
                            fini = true;
                        }

                    }

                }
            }
        }
        else{
            clicked = false;
        }

        // Update
        //----------------------------------------------------------------------------------
        
        // Update music buffer with new stream data
        UpdateMusicStream(music);
        
        // Get normalized time played for current music stream
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);

        prev_crt = crt_time;
        crt_time = GetMusicTimePlayed(music) * 1000.0; // en ms

        // Make sure time played is no longer than music
        if (timePlayed >= 1.0f || crt_time < prev_crt/10.0){
            timePlayed = 1.0f;
            fini = true;
            printf("Musique finie!\n");
        }

        set_progress_bar_value(ui, 0, timePlayed);

        //----------------------------------------------------------------------------------

        // Update text
        //----------------------------------------------------------------------------------
        
        //
        double depY = 1;
        if(crt_time < real_beat_to_ms_pos(song, deb_crt_sentence_sung, false)){
            depY = depY_avant;
        }
        else{
            if(depY_next <= depY_entrain){
                depY = depY_next;
            }
            else{
                depY = depY_entrain;
            }
        }
        t2=GetTime()*1000.0;
        double delta_t = t2-t1;
        depY *= delta_t;
        t1=t2;
        //
        if(!pause){
            for(size_t i = UI_BASE_ELEMENTS; i<ui->nb_elts-UI_FRONT_ELEMENTS; i++){
                ui->elts[i]->posY -= depY;
                if(ui->elts[i]->font1 >= 50){
                    ui->elts[i]->font1 -= 0.01 * delta_t;
                }
            }
        }
        //
        if(crt_time >= next_letter_sung){
            if((id_letter_sung < (song->notes_count)) && (UI_BASE_ELEMENTS + id_letter_sung < ui->nb_elts)){
                ui->elts[UI_BASE_ELEMENTS + id_letter_sung]->color = PURPLE;
                ui->elts[UI_BASE_ELEMENTS + id_letter_sung]->font1 += 5.0;
                //
                id_letter_sung++;
                if(id_letter_sung < song->notes_count){
                    if(song->notes[id_letter_sung].type == '-'){
                        // On va chercher la sentence suivante
                        deb_crt_sentence_sung = fin_crt_sentence_sung+2;
                        if(deb_crt_sentence_sung >= song->notes_count){
                            deb_crt_sentence_sung = song->notes_count-1;
                        }
                        fin_crt_sentence_sung = deb_crt_sentence_sung;
                        while(fin_crt_sentence_sung < song->notes_count && song->notes[fin_crt_sentence_sung].type != '-' && song->notes[fin_crt_sentence_sung].type != 'E'){
                            fin_crt_sentence_sung++;
                        }
                        fin_crt_sentence_sung--;

                        // On va cherche la sentence d'après
                        if(fin_crt_sentence_sung < song->notes_count - 2){
                            deb_next_sentence_sung = fin_crt_sentence_sung + 2;
                            fin_next_sentence_sung = deb_next_sentence_sung;
                            while(fin_next_sentence_sung < song->notes_count && song->notes[fin_next_sentence_sung].type != '-'){
                                fin_next_sentence_sung++;
                            }
                            fin_next_sentence_sung--;
                        }
                        else{
                            deb_next_sentence_sung = 0;
                            fin_next_sentence_sung = 0;
                        }
                        //
                        Ui_Element* deb_crt_note = get_notes_elt_id(ui, deb_crt_sentence_sung);
                        if(deb_crt_note != NULL){
                            depY_avant = deb_crt_note->posY - prct_h(DEB_SING_POS_RELATIVE, win);
                            depY_avant /= real_beat_to_ms_pos(song, deb_crt_sentence_sung, false) - crt_time;
                            depY_entrain = deltaY_entrain / (real_beat_to_ms_pos(song, fin_crt_sentence_sung, true) - real_beat_to_ms_pos(song, deb_crt_sentence_sung, false));
                        }
                        if(deb_next_sentence_sung != 0){
                            depY_next = get_notes_elt_id(ui, deb_next_sentence_sung)->posY - prct_h(DEB_SING_POS_RELATIVE, win);
                            depY_next /= real_beat_to_ms_pos(song, deb_next_sentence_sung, false) - crt_time;
                        }
                        else{
                            depY_next = 0;
                        }
                    }
                    next_letter_sung = real_beat_to_ms_pos(song, id_letter_sung, false);
                }
            }
        }

        //----------------------------------------------------------------------------------

        // Display
        //----------------------------------------------------------------------------------

        aff_UI(ui, &mpos, win);

        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    // Freeing
    free(path_music_file);
    //
}
