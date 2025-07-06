# NavWebServer
Serveur local sur Arduino Giga R1 Wifi pour affichage des données de navigation issues d'un Miniplex Shipmodul



## TODO

- Attention aux unités ... es ce que changer la config sur un instrument change l'unité retournée par le mux ? a prendre en charge ...



## Choix / questions
- Tout en int ou juste à l'affichage ?
- Moyenne : sur une durée fixe ou sur un nombre de mesures fixe ?


vitesse bateau, vitesse vent 1 décimale
caps : pas de décimale
profondeur : décimale
température de l'eau : pas de décimale
moyenne vitesse sol sur 30min



### moyenne

garder une mesure sur 60
au bout de 30 : calculer la moyenne
pareil pour vitesse eau




calcul de distance parcourue



durée de nav en heure minutes


Terre -> Route fond
Mer -> route surface