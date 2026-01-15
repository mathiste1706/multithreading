Pour size=6000<br>
Python<br>
[BOSS] Got result 0 time=3.946s<br>
[BOSS] Got result 2 time=4.127s<br>

C++ en mode Release<br>
[BOSS] Got result 1 time=4.577s

C++ en mode debug<br>
[BOSS] Got result 1 time=288.977879245s

Pour size=100<br>
Python<br>
[BOSS] Got result 0 time=0.0008308259998557332s<br>
[BOSS] Got result 2 time=0.0008331899998665904s<br>

C++ en mode Release<br>
[BOSS] Got result 1 time=0.000511059s


Explication:
En Python, ici on utilise numpy qui est optimisé pour des listes avec beaucoup d'élements. Ainsi on obtient un temps de résolution équivalent au C++. A noter qu'on ne comptabilise pas le temps que met l'agent C++ à obtenir l'information. A noter que si size est trop faible, alors numpy n'est pas adapté. Ainsi dans le cas d'un size faible, le C++ est plus rapide.


Pré-requis:
- Cmake
- uv

Pour installer uv:<br>
curl -LsSf https://astral.sh/uv/install.sh | sh

Pour installer Cmake:<br>
Utiliser le gestionnaire de paquet (sudo apt install, sudo pacman...)

Utilisation:<br>
Il faut tout d'abord compiler le C++ en mode Release:<br>
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release<br>
cmake --build build

Pour lancer les fichiers Python on utilise uv run.

Pour lancer les différents agent, il faut lancer dans l'ordre:
- manager.py
- proxy.py
- minion.py ou ./build/low_level.cpp
- .build/low_level.cpp ou minion.py
- boss.py
