# Protocol de interacțiune

## Invocarea agentului

Arbitrul va executa programul vostru, numit și **agent**, de fiecare dată cînd este rîndul său la mutare.

Arbitrul îi va trimite agentului, la intrarea standard, situația jocului conform specificațiilor de mai jos. Agentul trebuie să tipărească la ieșirea standard o mutare, conform specificațiilor de mai jos.

Agentul poate tipări orice mesaje la eroarea standard (`cerr` / `stderr`). Arbitrul le va ignora pe toate, cu excepția celor care încep cu prefixul `kibitz<spațiu>`. Pe acestea, arbitrul le va include în partida salvată, iar cînd urmărim reluarea partidei putem citi aceste mesaje. Puteți chibița orice doriți (sau nimic) despre numărul de poziții analizate, scoruri, motivul alegerii mutării pe care ați ales-o etc.

Agentul are la dispoziție **(TBD)** 3 minute per partidă și poate folosi 6 GB de memorie (laptopul meu are 16 GB).

Agentul poate stoca orice date în directorul său, în limite rezonabile (cîțiva GB). Dacă doriți să-mi trimiteți în avans niște date precalculate, trimiteți-mi-le cumva.

Dacă agentul se termină anormal, depășește timpul sau încearcă să facă o mutare incorectă, atunci arbitrul va opri partida și îi va oferi adversarului victoria cu 25-0.

## Datele de intrare

Datele de intrare au următorul format, fără linii goale și fără comentarii, care există doar pentru clarificări.

```
0
// Jucătorul la mutare (0 = primul jucător = roșu sau 1 = al doilea jucător = albastru).

x.....o
.......
.......
.......
.......
.......
o.....x
// O matrice de 7x7 caractere unde 'x' = piesă roșie, 'o' = piesă albastră, '.' = pătrat gol.

180000 180000
// Timpii rămași ai celor doi jucători, valori întregi, în milisecunde.
```

## Datele de ieșire

Programul vostru trebuie să tipărească mutarea dorită, sub una din formele

* `[a-g][1-7]` pentru o clonare, de exemplu `e4`;
* `[a-g][1-7]-[a-g][1-7]` pentru un salt, de exemplu `d2-e4`.

Liniile tablei sînt numerotate de la `1` la `7`, iar coloanele de la `a` la `g`.

În joc pot apărea situații cînd jucătorul la mutare nu mai are mutări valide, fie pentru că nu mai are piese, fie pentru că acele piese nu mai pot ajunge la pătrate goale. În regulile oficiale, jucătorul trebuie să spună „pas”. În implementarea noastră, arbitrul nu va mai invoca programul respectiv. Se presupune că celălalt jucător ocupă toate pătratele rămase goale și arbitrul calculează scorul final cu această presupunere.

Aceasta înseamnă că agentul vostru poate să presupună că are o mutare legală. Totuși, recomandăm ca agentul să țină cont de această situație în analiza arborelui de joc.

## Legături externe

* http://viiksipojat.fi/ataxx/ -- joacă Ataxx online
* https://logic-puzzles-online.com/ataxx/ -- joacă Ataxx online
* https://www.onlinesologames.com/ataxx -- joacă Ataxx online

