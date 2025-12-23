Pour size=6000
Python
[MINION] Connected to manager.
[MINION] Working on task 0 (size=6000)
[MINION] Finished task 0 in 2.748e+00s
[MINION] Working on task 2 (size=6000)
[MINION] Finished task 2 in 2.726e+00s

C++ en mode debug
62.4514s

C++ en mode release  
Received task 0 size=6000  
{"status": "ok"}  
Completed task 0 in 2.87037s  


Explication:
En Python, ici on utilise numpy qui est optimisé pour des listes avec beaucoup d'élements. Ainsi on obtient un temps de résolution équivalent au C++. A noter qu'on ne comptabilise pas le temps que met l'agent C++ à obtenir l'information.


Utilisation:
Pour lancer les différents agent, il faut lancer dans l'ordre:
- manager.py
- proxy.py
- minion.py ou ./build/low_level.cpp
- .build/low_level.cpp ou minion.py
- boss.py
