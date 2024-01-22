#include "lib_ui.h"


int font_size(Window* win, double font_size){
    double res =((double)font_size * (double)win->font_size_multiplier);
    int resi = (int)round(res);
    return resi;
}

Window* init_window(){
    Window* win = (Window*)malloc(sizeof(Window));
    win->title = "Ultra-Singer!";
    win->width = 1700;
    win->height = 900;
    win->fullscreen = false;
    //
    InitWindow(win->width, win->height, win->title);
    //
    win->monitor = GetCurrentMonitor();
    win->fs_height = GetMonitorHeight(win->monitor);
    win->fs_width = GetMonitorWidth(win->monitor);
    //
    if(win->fullscreen){
        SetWindowSize(win->fs_width, win->fs_height);
        ToggleFullscreen();
    }
    //
    win->crt_music_pages = 0;
    win->nb_music_pages = 0;
    win->nb_song_on_page = 0;
    win->closed = false;
    // Font
    win->font = LoadFont("res/fonts/modern_dos/ModernDOS9x16.ttf");
    win->font_size_multiplier = 1.0;
    // Set our game to run at 144 frames-per-second
    SetTargetFPS(144);
    //
    return win;
}

void close_window(Window* win){
    free(win);
    CloseWindow();
}

double absd(double v){
    if(v < 0) v = -v;
    return v;
}

double clampd(double val, double min, double max){
    if(val < min) val = min;
    if(val > max) val = max;
    return val;
}

int clampi(int val, int min, int max){
    if(val < min) val = min;
    if(val > max) val = max;
    return val;
}

int mini(int a, int b){
    if(a <= b) return a;
    return b;
}

int maxi(int a, int b){
    if(a >= b) return a;
    return b;
}

double maxd(double a, double b){
    if(a >= b) return a;
    return b;
}

int mini4(int a, int b, int c, int d){
    return mini(mini(a, b), mini(c, d));
}

int prct_w(int prct, Window* win){
    if(win->fullscreen){
        return win->fs_width * prct / 100;
    }
    else{
        return win->width * prct / 100;
    }
}

int prct_h(int prct, Window* win){
    if(win->fullscreen){
        return win->fs_height * prct / 100;
    }
    else{
        return win->height * prct / 100;
    }
}

///////////////////////////////////////////////////

void printbits(unsigned char v) {
    int i; // for C89 compatability
    for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

// Used the UTF-8 Rule found here : https://github.com/douglascrockford/JSON-c/blob/master/utf8_decode.c
int utf8_decode(const char *str,int *delta) {
    unsigned char* ustr = (unsigned char*)str;
    unsigned char u = *ustr;
    // Pour l'instant, u = XXXXXXXXs
    if(!(u & 0b10000000)){  // Pas unicode
        // u = 0XXXXXXX
        *delta=1;
        return (int)u;
    }
    // Pour l'instant, u = 1XXXXXXX
    if(!(u & 0b01000000)){ // Erreur
        // u = 10XXXXXX
        printf("Erreur 1\n");
        *delta=1;
        //return (int)u;
        return (char)'_';
    }
    // Pour l'instant, u = 11XXXXXX
    if(!(u & 0b00100000)){ // 1 bit unicode supplémentaire
        // u = 110XXXXX
        *delta=2;
        int res = (int)u + (int)(*(ustr+1)) - 0b11000000;
        switch(res){
            case 35: // û
                return (int)'u';
            case 162:
                return (int)'a';
            case 163: // à
                return (int)'a';
            case 165: // â
                return (int)'a';
            case 166: // ä
                return (int)'a';
            case 170: // ç
                return (int)'c';
            case 171: // è
                return (int)'e';
            case 172: // é
                return (int)'e';
            case 173: // ê
                return (int)'e';
            case 174: // ë
                return (int)'e';
            case 177: // î
                return (int)'i';
            case 178: // ï
                return (int)'i';
            case 183: // ô
                return (int)'o';
            case 184: // ö
                return (int)'o';
            case 188: // ù
                return (int)'u';
            case 190: // û
                return (int)'u';
            case 191: // ü
                return (int)'u';
            default:
                printbits(u);
                printf(" ");
                printbits(*(ustr+1));
                printf("\n");
                printf("Unicode 1 : %d\n", res);
                //return res;
                return (char)'_';
        }
    }
    // Pour l'instant, u = 111XXXXX
    if(!(u & 0b00010000)){ // 2 bits unicodes supplémentaires
        // u = 1110XXXX
        *delta=3;
        //int res = (int)u + (int)(*(ustr+1)) + (int)(*(ustr+2)) - 0b11100000;
        // printbits(u);
        // printf(" ");
        // printbits(*(ustr+1));
        // printf(" ");
        // printbits(*(ustr+2));
        // printf("\n");
        // printf("Unicode 2 : %d\n", res);
        //return res;
        return (char)'_';
    }
    // Pour l'instant, u = 1111XXXX
    if(!(u & 0b00001000)){ // 3 bits unicodes supplémentaires
        // u = 11110XXX
        *delta=4;
        //int res = (int)u + (int)(*(ustr+1)) + (int)(*(ustr+2)) + (int)(*(ustr+3)) - 0b11110000;
        // printbits(u);
        // printf(" ");
        // printbits(*(ustr+1));
        // printf(" ");
        // printbits(*(ustr+2));
        // printf(" ");
        // printbits(*(ustr+3));
        // printf("\n");
        // printf("Unicode 3 : %d\n", res);
        //return res;
        return (char)'_';
    }
    // u = 11111XXX
    // Should be an error
    *delta=1;
    //printf("Erreur 2\n");
    //return (int)u;
    return (char)'_';
}

int* unicode_code_points(char* txt, int* taille_decoded){
    int n = (int)strlen(txt);
    int i = 0;
    int delta = 0;
    *taille_decoded = 0;
    int* codepoints = calloc(sizeof(int), n+1);
    int j = 0;
    while(i<n){
        delta=1;
        codepoints[j] = utf8_decode(&(txt[i]), &delta);
        i+=delta;
        (*taille_decoded)++;
        j++;
    }
    return codepoints;
}


void draw_text(Font font, char* txt, int posX, int posY, int font_size, Color color){
    int taille_decoded = 0;
    int* codepoints = unicode_code_points(txt, &taille_decoded);
    // if((int)strlen(txt) !=  taille_decoded)
    //     printf("Txt : %s, encoded : %ld, decoded : %d\n", txt, strlen(txt), taille_decoded);
    // printf("%d %d - %s\n", posX, posY, txt);
    DrawTextCodepoints(font, codepoints, taille_decoded, (Vector2){posX, posY}, font_size, 0, color);
    free(codepoints);
    //DrawTextEx(font, txt, (Vector2){posX, posY}, font_size, 0, color);
    //DrawText(txt, posX, posY, font_size, color);
}


void AffBtText(Window* win, char* txt, int posX, int posY, Color color, int baseFont, int hoverFont, Vector2* mpos){
    //int max_range = (hoverFont-baseFont)*strlen(txt);
    int l = strlen(txt);
    int tx = l*baseFont;
    int ty = baseFont;
    double t = 1.0;
    double max_range = 20.0;
    double max_range_X = max_range + (double)(tx)/2.0; // px
    double max_range_Y = max_range + (double)(ty)/2.0; // px
    //
    if(mpos->x >= posX-max_range && mpos->x <= posX+tx+max_range && mpos->y >= posY-max_range && mpos->y <= posY+ty+max_range){
        
        double diff_h = absd((double)(posX+tx/2) - mpos->x) / max_range_X;
        double diff_v = absd((double)(posY+ty/2) - mpos->y) / max_range_Y;

        double diff = maxd(diff_v, diff_h);

        // printf("Diff : %lf %lf %lf\n", diff_h, diff_v, diff);

        t = 1.0-clampd(diff, 0.0, 1.0);

        // DrawRectangle(posX-max_range, posY-max_range, tx+2*max_range, ty+2*max_range, (Color){255, 0, 0, 100.0*t});
    }
    int font = (int)((t)*(double)baseFont + (1.0-t)*(double)hoverFont);
    
    //DrawText(txt, posX, posY, font, color);
    //DrawTextEx(win->font, txt, (Vector2){.x=posX, .y=posY}, font, 0, color);
    draw_text(win->font, txt, posX, posY, font, color);
}

Ui* init_UI(size_t max_elts, Color bg_color){
    Ui* ui = (Ui*)malloc(sizeof(Ui));
    ui->bg_color = bg_color;
    ui->max_elts = max_elts;
    ui->nb_elts = 0;
    ui->nb_cliquable = 0;
    ui->elts = (Ui_Element**)malloc(sizeof(Ui_Element*)*max_elts);
    ui->cliquable = (int*)malloc(sizeof(int)*max_elts);
    //
    for(size_t i=0; i<max_elts; i++){
        ui->elts[i] = NULL;
        ui->cliquable[i] = -1;
    }
    //
    ui->custom_cursor = false;
    ui->cursor = (Sprite) {
        .texture = LoadTexture("res/custom_mouse1-16.png"),
        .position = {0.0f, 0.0f},
        .rect = {0.0f, 0.0f, 16.0f, 16.0f},
    };
    //
    if(ui->custom_cursor){
        HideCursor();
    }
    //
    return ui;
}

void free_UI(Ui* ui){
    // On libère tous les éléments
    // for(size_t i=0; i<ui->nb_elts; i++){
    //     if(ui->elts[i] != NULL){
    //         free(ui->elts[i]);
    //     }
    // }
    while(ui->nb_elts > 0){
        ui_free_last_element(ui);
    }
    //
    free(ui->elts);
    free(ui->cliquable);
    free(ui);
}

Ui_Element* init_ui_elt(){
    Ui_Element* elt = (Ui_Element*)calloc(sizeof(Ui_Element), 1);
    if(elt == NULL){
        return NULL;
    }
    elt->type = -1;
    elt->needs_to_free_txt = false;
    elt->visible = true;
    return elt;
}

void add_text(Ui* ui, char* txt, bool needs_to_free_txt, int posX_relative, int posX, int posY_relative, int posY, Color color, int font){
    if(ui->nb_elts >= ui->max_elts){
        printf("Error: Can't add more elements to UI because you have already the maximum of elements!\n");
        return;
    }
    Ui_Element* elt = init_ui_elt();
    if(elt == NULL){
        printf("Error when adding elt to UI: elt is NULL\n");
        return;
    }
    elt->type = UI_TEXT;
    elt->text = txt;
    elt->needs_to_free_txt = needs_to_free_txt;
    elt->posX_relative = posX_relative;
    elt->posY_relative = posY_relative;
    elt->posX = posX;
    elt->posY = posY;
    elt->color = color;
    elt->font1 = font;
    //
    ui->elts[ui->nb_elts] = elt;
    ui->nb_elts++;
}

void add_bt_text(Ui* ui, char* txt, bool needs_to_free_txt, int posX_relative, int posX, int posY_relative, int posY, Color color, int baseFont, int hoverFont){
    if(ui->nb_elts >= ui->max_elts){
        printf("Error: Can't add more elements to UI because you have already the maximum of elements!\n");
        return;
    }
    Ui_Element* elt = init_ui_elt();
    if(elt == NULL){
        printf("Error when adding elt to UI: elt is NULL\n");
        return;
    }
    elt->type = UI_BT_TEXT;
    elt->text = txt;
    elt->needs_to_free_txt = needs_to_free_txt;
    elt->posX_relative = posX_relative;
    elt->posY_relative = posY_relative;
    elt->posX = posX;
    elt->posY = posY;
    elt->color = color;
    elt->font1 = baseFont;
    elt->font2 = hoverFont;
    //
    int l = strlen(elt->text);
    elt->tailleX = l*(elt->font1);
    elt->tailleY = elt->font1;
    //
    ui->elts[ui->nb_elts] = elt;
    ui->cliquable[ui->nb_cliquable] = ui->nb_elts;
    ui->nb_elts++;
    ui->nb_cliquable++;
    //
}

void add_texture(Ui* ui, Texture2D texture, int posX_relative, int posX, int posY_relative, int posY, Color tint){
    if(ui->nb_elts >= ui->max_elts){
        printf("Error: Can't add more elements to UI because you have already the maximum of elements!\n");
        return;
    }
    Ui_Element* elt = init_ui_elt();
    if(elt == NULL){
        printf("Error when adding elt to UI: elt is NULL\n");
        return;
    }
    elt->type = UI_TEXTURE;
    elt->texture = texture;
    elt->posX_relative = posX_relative;
    elt->posY_relative = posY_relative;
    elt->posX = posX;
    elt->posY = posY;
    elt->color = tint;
    //
    ui->elts[ui->nb_elts] = elt;
    ui->nb_elts++;
}

void add_progress_bar(Ui* ui, int posX_relative, int posX, int posY_relative, int posY, int tailleX_relative, int tailleX, int tailleY_relative, int tailleY, Color color){
    if(ui->nb_elts >= ui->max_elts){
        printf("Error: Can't add more elements to UI because you have already the maximum of elements!\n");
        return;
    }
    Ui_Element* elt = init_ui_elt();
    if(elt == NULL){
        printf("Error when adding elt to UI: elt is NULL\n");
        return;
    }
    //
    elt->type = UI_PROGRESSBAR;
    elt->posX = posX;
    elt->posX_relative = posX_relative;
    elt->posY = posY;
    elt->posY_relative = posY_relative;
    elt->tailleX = tailleX;
    elt->tailleX_relative = tailleX_relative;
    elt->tailleY = tailleY;
    elt->tailleY_relative = tailleY_relative;
    elt->color = color;
    //
    ui->elts[ui->nb_elts] = elt;
    ui->nb_elts++;
}

void set_progress_bar_value(Ui* ui, size_t idx_elt, double value){
    if(idx_elt >= ui->nb_elts || ui->elts[idx_elt]->type != UI_PROGRESSBAR || value < 0 || value > 1)
        return;
    //
    ui->elts[idx_elt]->progress = value;
}

void add_rect(Ui* ui, int posX_relative, int posX, int posY_relative, int posY, int tailleX_relative, int tailleX, int tailleY_relative, int tailleY, Color cl){
if(ui->nb_elts >= ui->max_elts){
        printf("Error: Can't add more elements to UI because you have already the maximum of elements!\n");
        return;
    }
    Ui_Element* elt = init_ui_elt();
    if(elt == NULL){
        printf("Error when adding elt to UI: elt is NULL\n");
        return;
    }
    //
    elt->type = UI_RECT;
    elt->posX = posX;
    elt->posX_relative = posX_relative;
    elt->posY = posY;
    elt->posY_relative = posY_relative;
    elt->tailleX = tailleX;
    elt->tailleX_relative = tailleX_relative;
    elt->tailleY = tailleY;
    elt->tailleY_relative = tailleY_relative;
    elt->color = cl;
    //
    ui->elts[ui->nb_elts] = elt;
    ui->nb_elts++;
}

void aff_UI(Ui* ui, Vector2* mpos, Window* win){
    ui->cursor.position = *mpos;
    //
    BeginDrawing();

    ClearBackground(BLACK);
    //
    for(size_t i = 0; i<ui->nb_elts; i++){
        Ui_Element* elt = ui->elts[i];
        if(elt != NULL && elt->visible){
            //
            int xx = prct_w(elt->posX_relative, win) + elt->posX;
            int yy = prct_h(elt->posY_relative, win) + elt->posY;
            //
            switch (elt->type)
            {
                case UI_TEXT:
                    draw_text(win->font, elt->text, xx, yy, elt->font1, elt->color);
                    break;
                
                case UI_BT_TEXT:
                    AffBtText(win, elt->text, xx, yy, elt->color, elt->font1, elt->font2, mpos);
                    break;
                
                case UI_TEXTURE:
                    DrawTexture(elt->texture, xx, yy, elt->color);
                    break;
                
                case UI_PROGRESSBAR:
                    //
                    DrawRectangle(
                        xx,
                        yy,
                        (int)( elt->progress * (double)(elt->tailleX + prct_w(elt->tailleX_relative, win)) ),
                        elt->tailleY + prct_h(elt->tailleY_relative, win),
                        elt->color
                    );
                    break;

                case UI_RECT:
                    //
                    DrawRectangle(
                        xx,
                        yy,
                        elt->tailleX + prct_w(elt->tailleX_relative, win),
                        elt->tailleY + prct_h(elt->tailleY_relative, win),
                        elt->color
                    );
                    break;


                default:
                    break;
            }
        }
    }
    //
    if(win->fullscreen){
        DrawFPS(win->fs_width-80, 5);
    }
    else{
        DrawFPS(win->width-80, 5);
    }
    
    //
    if(ui->custom_cursor){
        DrawTextureRec(ui->cursor.texture,ui->cursor.rect, ui->cursor.position,WHITE);
    }
    //
    EndDrawing();
}

bool test_collision_bt_text(Ui_Element* elt, Vector2* v, Window* win){
    if(elt->type != UI_BT_TEXT){
        return false;
    }
    Rectangle r = {
        elt->posX + prct_w(elt->posX_relative, win),
        elt->posY + prct_h(elt->posY_relative, win),
        elt->tailleX + prct_w(elt->tailleX_relative, win),
        elt->tailleY + prct_h(elt->tailleY_relative, win)
    };

    //
    return CheckCollisionRecs(r, (Rectangle){v->x, v->y, 1, 1});
}

// Retourne l'index du premier élément cliquable qui a reçu un click, sinon -1
int test_collision(Ui* ui, Vector2* v, Window* win){
    for(size_t i = 0; i < ui->nb_cliquable; i++){
        int idx = ui->cliquable[i];
        if(idx != -1 && idx >= 0 && (size_t)idx <= ui->nb_elts){
            Ui_Element* elt = ui->elts[idx];
            if(!elt->visible){
                continue;
            }
            //
            switch (elt->type)
            {
            case UI_BT_TEXT:
                if(test_collision_bt_text(elt, v, win)){
                    return idx;
                }
                break;
            
            default:
                break;
            }
        }
    }
    return -1;
}

void ui_free_last_element(Ui* ui){
    if(ui->nb_elts == 0) return;
    //
    ui->nb_elts--;
    if(ui->nb_cliquable > 0 && (size_t)ui->cliquable[ui->nb_cliquable-1] == ui->nb_elts){
        ui->nb_cliquable--;
    }
    //
    if(ui->elts[ui->nb_elts]->needs_to_free_txt){
        free(ui->elts[ui->nb_elts]->text);
    }
    free(ui->elts[ui->nb_elts]);
    ui->elts[ui->nb_elts] = NULL;
}
