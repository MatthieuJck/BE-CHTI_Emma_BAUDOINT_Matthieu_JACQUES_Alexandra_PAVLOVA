On utilise toujours le signal 0x33
Dans le logic analyser, on va observer les variables suivantes : 
compt[x] pour x allant de 0 à 5, avec un échelle [0-30]
score[x] pour x allant de 0 à 5 avec une échelle [0-6]
TIM3_CCR3 avec une échelle [0-70000]

On va observer qu'à chaque fois que nous recevons un pic, le score est incrémenté et on a l'émission d'un bruit
Les pics attendus sont :
1 pics pour compt[0]
2 pics pour compt[1]
3 pics pour compt[2]
4 pics pour compt[3]
5 pics pour compt[4]
0 pics pour compt[5]
