## Fichier Ultrastar

Le logiciel Ultrastar est un jeu de Karaoke open source similaire à Singstar.

Le jeu consiste à jouer une musique et à afficher les paroles avec la hauteur des notes
correspondantes. Le joueur doit alors chanter juste pour gagner des points.

Le jeu fonctionne avec n'importe quelle musique (sous format mp3, wav, ou autre).
Les informations concernant le fichier de musique utilisé, les paroles et les hauteurs de notes sont
stockées dans un fichier texte appelé "fichier ultrastar". Le format est le suivant.

Tout d'abord des informations générales (appelées entêtes) sont inscrite comme le chemin vers le fichier de musique, le genre, la durée, etc... Puis viens les paroles avec les hauteurs de note.

#### Entêtes
Une information d'entête est inscrite sur une ligne qui commence par `#` suivi du nom de l'information (ex: TITLE ou ARTIST) suivi de `:` et de l'information elle-même jusqu'à la fin de la ligne.
Exemple d'entêtes:
```
#TITLE:On the run
#ARTIST:Joshua Morin
#EDITION:Creative Commons
#LANGUAGE:English
#MP3:Joshua Morin - On the run.ogg
#COVER:Joshua Morin - On the run [CO].jpg
#BACKGROUND:Joshua Morin - On the run [BG].jpg
#BPM:297,5
#GAP:11250
```

* `GAP` correspond au nombre de milliseconde avant la première note.

#### Paroles et Notes
Les lignes suivantes commencent par `:`, `-`, `*`, `F` ou `E`.
Exemple:
```
: 0 3 9 So
: 6 4 11  far
: 12 2 9  a
: 16 4 11 way
* 25 8 14  from
: 38 4 7  home,
- 44
: 51 3 7 so
: 58 4 7  far
: 64 2 4  a
: 68 3 7 way,
: 74 2 9  that
: 77 6 11  I
: 87 4 7  don't
: 94 3 2  know,
- 99 100
: 110 2 2 where
: 113 3 -1  my
: 119 5 2  home
: 129 3 7  is
: 135 2 11  a
: 138 2 9 ny
: 143 7 7 more.
E
```

Les lignes qui commencent par `-` servent à délimiter les phrases de la chanson. Le reste de la ligne contient un ou deux entiers qui délimitent le début et la fin de la transition entre les phrases. Le premier nombre indique le moment où la phrase disparaît de l'écran et le deuxième nombre (ou le premier s'il n'y en a qu'un) indique le moment où la phrase suivante doit s'afficher à l'écran. Les valeur représentent des nombre de temps depuis le début (la durée d'un temps dépend de la valeur de BPM).


Si la ligne commence par `:`, `*` ou `F`, c'est une ligne contenant une note à chanter. `:` est une note normale, `*` est une note en or (rapporte plus de points) et `F` est une note libre (non comptabilisée).

La suite de la ligne contient trois nombre et le texte qu'il faut chanter:
* le premier nombre correspondant à la position de la note dans la chanson en nombre de temps depuis le début (la durée d'un temps dépend de la valeur BPM).
* le deuxième nombre correspond à la longueur de la note (en nombre de temps).
* le troisième nombre correspond à la hauteur de la note (0 est un DO).

La fin de la ligne contient généralement une syllabe, qu'il faut chanter.


Dans l'exemple précédent, la première phrase contient 6 syllabes (dont une en or). Au temps 44 la première phrase disparaît et la deuxième phrase apparaît. au temps 99 la deuxième phrase disparaît et au temps 100 la troisième phrase apparaît.


Le but du projet est d'améliorer le logiciel d'édition de fichier Ultrastar.
