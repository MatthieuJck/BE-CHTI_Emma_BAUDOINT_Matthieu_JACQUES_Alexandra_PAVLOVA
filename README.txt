Il faut observer la valeur compt[x] pour x de 0 à 5 avec une échelle allant de 0 à 20 
On utilise le signal 0x33 donc nous sommes sensés observer:
1 pic sur compt[0]
2 pics sur compt[1]
3 pics sur compt[2]
4 pics sur compt[3]
5 pics sur compt[4]
pas de pic sur le compt[5]

Il faut aussi observer les score[x] de 0 à 5, et vérifier qu'il s'incrémente à chaque fois qu'on a un pic sur le compt[] correspondant 
Lancer le programme, attendre environ 10 secondes puis l'arrêter (on observe pas forcément les signaux en temps réel pendant les 10 secondes de fonctionnement du programme)
Faire un zoom: all
Vérifier les observations


On peut utiliser le signal 0x52 pour aussi vérifier que le compteur 5 fonctionne. 
Il y aura alors:
1 tir  a  85kHz (k=17)     
2 tirs a  90kHz (k=18)     
3 tirs a  95kHz (k=19)     
4 tirs a 100kHz (k=20)     
5 tirs a 115kHz (k=23)   
signal 2 : (en parallele, decale de 50 ms)    15 tirs a 120kHz (k=24)