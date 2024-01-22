#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include "song-io.h"
#include <SA/SA.h>
#include <stdio.h>

#define SONGS_PATH "./songs/"

// Renvoie une liste de toutes les musiques
SA_DynamicArray* list_songs();

