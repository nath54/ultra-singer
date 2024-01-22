#include "song.h"


double
beat_to_ms_pos(const song_t *song, int beat)
{
    return song->gap + (beat/song->bpm*60000);
}


double
beat_to_ms(const song_t *song, int beat)
{
    return (beat/song->bpm*60000);
}

void
song_free(song_t *song)
{
  size_t idx;

  free(song->title);
  free(song->artist);
  free(song->song_file_path);
  free(song->edition);
  free(song->cover);
  free(song->background);
  free(song->language);

  // free headers
  for(idx = 0; idx < song->headers_count; idx++)
  {
    free(song->headers[idx].key);
    free(song->headers[idx].value);
  }
  free(song->headers);

  // free notes
  for(idx = 0; idx < song->notes_count; idx++)
  {
    free(song->notes[idx].text);
  }
  free(song->notes);
}
