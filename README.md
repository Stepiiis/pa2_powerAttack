TowerAttack
=============

## Introduction

### What is Tower Attack?
Tower attack is a game where the player has control of spawning attackers and tries to conquer the battlefield 
by destroying all towers and leaving through the exit door.


#### Points on map legend
- ' ' - Empty places where Towers can be placed and Attackers can move
- 'I' - default tower
- 'F' - fast shooting tower
- 'T' - high damage tower
- 'S' - tower which shoots damage and gives slow buff
- '#' - wall
- 'o','O' - watter, only fast attacker can move through it
- '$" - basic attacker
- '@' - slower attacker with higher damage
- '%' - faster attacker which avoids perimeter of towers
- '<=' - entries and exits from map.


## Controls
- DELETE confirms the selected option in menus
- 'a','s' a 'd' chooses different attackers
- '1', '2', '3' chooses different towers
- SPACE spawns chosen attacker on chosen spawn lane
- 'q' pressed during game brings up pause menu where player can save game 
- ESC used to return from menu where player can save game (because i am using ncurses, this can take up to 1-2 seconds)

## Implementace

### Pohyb útočníků
- Pro hledání cesty útočníků je použit algoritmus Bredth First Search se specializací pro každý druh útočníka.
- Výpočet cesty se provede při každém kroku.
- Pokud má útočník slowEffect, bude po dobu platnosti jeho pohyb ~1.5x zpomalen dokud efekt nevyprší.
- Útočník se pohybuje pouze pokud je na dalším políčku, kam se má posunout podle vypočítané cesty, volno. Pokud je obsazeno, stojí na místě.
- [Slower attacker] (classchargerAttacker.html) je pomalejší, ale má vyšší damage než základní útočník. Vždy směřuje směrem na nejbližší věž, nebo východ.
- [Faster attacker] (classfastAttacker.html) je rychlejší, ale má nižší HP a damage. Vždy směřuje směrem k východu a při cestě se vyhíbá permieteru věží.
- Tyto specifikace fungují pomocípolymorfních tříd a jejich [virtuálních metod] (classAttacker.html#ab77eae745103e33b7032878215f57ac1), které specializují který prvek můžou navšítit.
- Pohyb entit se projevuje nejen v parameterch entity, ale upravuje se podle něj i obsah třídy Map.
- Bližší info v Attacker. (Hlavní je funkce [Attacker::findShortestPath()] (classAttacker.html#a711c4a8037d8d97ae9ee525f9b6e439e))

### "AI" protihráče
- Věže jsou vytvářeny vždy na začátku hry podle dané obtížnosti.
- Pokud je daná obtížnost "easy", jde pouze o náhodné vytvoření věží na mapě.
- pokud je daná obtížnost "medium", věže jsou vytvořeny v okolí oblasti kudy by mohli útočníci chodit.
- pokud je daná obtížnost "hard", pouze se zvýší počet věží a použije se stejný algoritmus jako "medium".
- Pro zjištění přibližné cesty útočníků je použit algoritmus BFS od výstupu k vstupům.
- Bližší info ve funkci [Enemy::createTowers()] (classEnemy.html#a238798e0d954a7ee3b6fafb592151cde).

### Interakce mezi entitami
- Nejprve je u každé entity zkontrolován perimetr, který je stanoven v definičním souboru a pokud nenašla entitu na kterou měla focus, 
zaútočí na novou nejližší nepřátelskou entitu a uloží si jeho ID jako současný focus. 
- útoky nejdřív provedou útočníci, které případně mohou i věže zničit.
- Poté zbylé věže provedou útoky na útočníky.
- Bližší info ve funkci [CGame::performAttacks()] (classCGame.html#a746d639da0897949fda403039daf4bf5).
- Zajímavá by mohla být i tato funkce [Map::checkClearSight(...)] (classMap.html#a0fb31ef7fcef2c7071c549ebf4f66153).

### Načítání z definice
- Definice mapy je uložena v souboru map1.txt a soubor je postupně načítán a vyvtářeno dvourozměrné pole, které je poté vykresleno na obrazovku
- Definice útočníků a věží jsou uloženy ve vlastních kontejnerech std::map, které mají klíče vždy názaev typu a hodnotou
uvnitř mapy je další mapa, která obsahuje dané parametry a klíčem je jejich název.
- K definicím přisupují konstruktory specifických entit pomocí reference a pouze si vyberou která data chtějí použít.
- Bližší info ve třídě CDefinitions.
### Ukládání a načítání hry
- Hra je načítána podobným způsobem jako načítá z definice. Mapa v mapě v mapě... 
- Funkce vykonávající tuto funkcionalitu jsou: 
[CGame::loadFromSave(const std::string &path)] (classCGame.html#afbbb4f8d05f753b7c2d6a0d91ca96a9b) a
[CGame::save()] (classCGame.html#a260a60819a5e6a8991e7f7983c51a509) 
