Pour size=6000<br>
Python<br>
[MINION] Connected to manager.<br>
[MINION] Working on task 0 (size=6000)<br>
[MINION] Finished task 0 in 2.748e+00s<br>
[MINION] Working on task 2 (size=6000)<br>
[MINION] Finished task 2 in 2.726e+00s<br>

C++ en mode debug<br>
62.4514s<br>



C++ en mode release<br>
Received task 0 size=6000<br>
{"status": "ok"}<br>
Completed task 0 in 2.87037s<br>


Explication:
En Python, ici on utilise numpy qui est optimisé pour des listes avec beaucoup d'élements. Ainsi on obtient un temps de résolution équivalent au C++. A noter qu'on ne comptabilise pas le temps que met l'agent C++ à obtenir l'information. A noter que si size est trop faible, alors numpy n'est pas adapté. Ainsi dans le cas d'un size faible, le C++ est plus rapide.


Utilisation:
Pour lancer les différents agent, il faut lancer dans l'ordre:
- manager.py
- proxy.py
- minion.py ou ./build/low_level.cpp
- .build/low_level.cpp ou minion.py
- boss.py
