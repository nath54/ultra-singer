#pragma once
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iconv.h>


#define W_WIDTH 1700
#define W_HEIGHT 900


#define UI_TEXT 0
#define UI_BT_TEXT 1
#define UI_TEXTURE 2
#define UI_PROGRESSBAR 3
#define UI_RECT 4

// Interface

typedef struct Sprite {
    Texture2D texture;
    Rectangle rect;
    Vector2 position;
} Sprite;

typedef struct{
    /*
        -1 = nothing
        0 = txt
        1 = bt txt
        2 = texture
        3 = progress bar
        4 = rectangle
    */
    int type;
    // Properties
    double posX_relative;
    double posY_relative;
    double posX;
    double posY;
    Color color;
    char* text;
    bool needs_to_free_txt;
    double font1;
    double font2;
    Texture2D texture;
    //
    bool visible;
    // For collisions
    int tailleX_relative;
    int tailleX;
    int tailleY_relative;
    int tailleY;
    // For progress bars
    double progress;
} Ui_Element;

typedef struct {
    size_t max_elts;
    size_t nb_elts;
    Ui_Element** elts;
    int* cliquable;
    size_t nb_cliquable;
    //
    bool custom_cursor;
    Sprite cursor;
    Color bg_color;
} Ui;

typedef struct{
    int width;
    int height;
    int fs_width;
    int fs_height;
    bool fullscreen;
    char* title;
    int monitor;
    //
    Font font;
    double font_size_multiplier;
    //
    int crt_music_pages;
    int nb_music_pages;
    int nb_song_on_page;
    bool closed;
} Window;

int font_size(Window* win, double font_size);

Window* init_window();

void close_window(Window* win);

// Percent Width &nd Height
int prct_w(int prct, Window* win);
int prct_h(int prct, Window* win);

Ui_Element* init_ui_elt();

Ui* init_UI(size_t max_elts, Color bg_color);

void free_UI(Ui* ui);

void add_text(Ui* ui, char* txt, bool needs_to_free_txt, int posX_relative, int posX, int posY_relative, int posY, Color color, int font);

void add_bt_text(Ui* ui, char* txt, bool needs_to_free_txt, int posX_relative, int posX, int posY_relative, int posY, Color color, int baseFont, int hoverFont);

void add_texture(Ui* ui, Texture2D texture, int posX_relative, int posX, int posY_relative, int posY, Color tint);

void add_progress_bar(Ui* ui, int posX_relative, int posX, int posY_relative, int posY, int tailleX_relative, int tailleX, int tailleY_relative, int tailleY, Color tint);

void set_progress_bar_value(Ui* ui, size_t idx_elt, double value);

void add_rect(Ui* ui, int posX_relative, int posX, int posY_relative, int posY, int tailleX_relative, int tailleX, int tailleY_relative, int tailleY, Color cl);

void aff_UI(Ui* ui, Vector2* mpos, Window* win);

bool test_collision_bt_text(Ui_Element* elt, Vector2* v, Window* win);

// Retourne l'index du premier élément cliquable qui a reçu un click, sinon -1
int test_collision(Ui* ui, Vector2* v, Window* win);

void remove_ui_elt(Ui* ui, size_t elt_idx);

void AffBtText(Window* win, char* txt, int posX, int posY, Color color, int baseFont, int hoverFont, Vector2* mpos);

void ui_free_last_element(Ui* ui);