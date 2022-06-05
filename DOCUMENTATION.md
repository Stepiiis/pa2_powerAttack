TowerAttack
=============

## Introduction

### What is Tower Attack?
Tower attack is a game where the player has control of spawning attackers and tries to conquer the battlefield 
by destroying the tower and entering leaving trough the next door;


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

#### Pohyb útočníků
- Pro hledání cesty útočníků je použit algoritmus Bredth First Search se specializací pro každý druh útočníka.
- Výpočet cesty se provede při každém kroku.
- Pokud má útočník slowEffect, bude po dobu platnosti jeho pohyb ~1.5x zpomalen dokud efekt nevyprší.
- Útočník se pohybuje pouze pokud je na dalším políčku, kam se má posunout podle vypočítané cesty, volno. Pokud je obsazeno, stojí na místě.
- slower attacker je pomalejší, ale má vyšší damage než základní útočník. Vždy směřuje směrem na nejbližší věž, nebo východ.
- faster attacker je rychlejší, ale má nižší HP a damage. Vždy směřuje směrem k východu a při cestě se vyhíbá permieteru věží.
- Tyto specifikace fungují pomocí polymorfních tříd a jejich virtuálních metod, které specializují který prvek můžou navšítit.
- Pohyb entit se projevuje nejen v parameterch entity, ale upravuje se podle něj i obsah třídy Map.
- Bližší info v \src\Attacker.cpp

### AI věží
- Věže jsou vytvářeny vždy na začátku hry podle dané obtížnosti.
- Pokud je daná obtížnost "easy", jde pouze o náhodné vytvoření věží na mapě.
- pokud je daná obtížnost "medium", věže jsou vytvořeny v okolí oblasti kudy by mohli útočníci chodit.
- pokud je daná obtížnost "hard", pouze se zvýší počet věží a použije se stejný algoritmus jako "medium".
- pro zjištění přibližné cesty útočníků je použit algoritmus BFS od výstupu k vstupům.

### Interakce mezi entitami
- nejprve je u každé entity zkontrolován perimetr, který je stanoven v definičním souboru a pokud nenašla entitu na kterou měla focus, 
zaútočí na novou nejližší nepřátelskou entitu a uloží si jeho ID jako současný focus. 
- útoky nejdřív provedou útočníci, které případně mohou i věže zničit.
- poté zbylé věže provedou útoky na útočníky.

## Načítání z definice
- definice mapy je uložena v souboru map1.txt a soubor je postupně načítán a vyvtářeno dvourozměrné pole, které je poté vykresleno na obrazovku
- definice útočníků a věží jsou uloženy ve vlastních kontejnerech std::map, které mají klíče vždy názaev typu a hodnotou
uvnitř mapy je další mapa, která obsahuje dané parametry a klíčem je jejich název.
- k definicím přisupují konstruktory specifických entit pomocí reference a pouze si vyberou která data chtějí použít.

### Ukládání a načítání hry
- hra je načítána podobným způsobem jako načítá z definice. Mapa v mapě v mapě... více v \src\Map.cpp

\ref file::/doc/classbasicAttacker.html#a5b7a46059eaf43894f32016477c1e9a9
