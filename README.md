# Dokumentacja
Projektu SO Temat 
16 Stacja Narciarska
Kamil Gębala 151868

---

## Spis treści

1. Ogólne założenia projektu
2. Komponenty
3. Struktura projektu 
4. Podstawowe działanie systemu
5. Implementacja jednolitego systemu błędów programu i obsługa errno
6. Implementacja zarządzania zasobami, komunikacji oraz synchronizacji procesów 
7. Implementacja głównych funkcji projektu
8. Implementacja pobocznych funkcji projektu
9. Elementy specjalne
10. Przykłady użycia funkcji systemowych 
11. Testy 

---

## I. Ogólne założenia projektu

Projekt realizujący modularną symulację stacji narciarskiej w języku C++ w środowisku WSL, umieszczony i udokumentowany w repozytorium GitHub.

**Temat 16 – Stacja narciarska**

Na terenie stacji narciarskiej znajduje się krzesełkowa kolej linowa. Kolej składa się z 3-osobowych krzesełek o łącznej liczbie 80 sztuk. Jednocześnie może być zajętych 40 krzesełek, na których siedzi maksymalnie 3x40 = 120 osób. Narciarze/turyści przychodzą na teren stacji w losowych momentach czasu (nie wszyscy z nich muszą jeździć na nartach). Wejście na teren kolejki linowej odbywa się po opłaceniu karnetu w kasie. Karnety są czasowe (Tk1, Tk2, Tk3) lub dzienne.

- Dzieci poniżej 12 roku życia oraz seniorzy powyżej 65 roku życia mają 25% zniżkę.
- Dzieci poniżej 8 roku życia znajdują się pod stałą opieką osoby dorosłej.
- Wejście na peron dolnej stacji odbywa się czterema bramkami jednocześnie. Na peronie dolnej stacji może przebywać maksymalnie N osób.
- Wyjazd z peronu stacji górnej odbywa się dwoma drogami jednocześnie (ruch jednokierunkowy).
- Stacja dolna jest obsługiwana przez pracownika1, stacja górna jest obsługiwana przez pracownika2. W przypadku zagrożenia pracownik1 lub pracownik2 zatrzymują kolej linową (sygnał1). Aby wznowić działanie, pracownik, który zatrzymał kolej, komunikuje się z drugim pracownikiem – po otrzymaniu komunikatu zwrotnego o gotowości kolej jest uruchamiana ponownie (sygnał2).

Zjazd odbywa się trzema trasami o różnym stopniu trudności – średni czas przejazdu dla poszczególnych tras wynosi odpowiednio T1, T2 i T3 (T1\<T2\<T3).

**Zasady działania stacji ustalone przez kierownika:**

- Kolej linowa jest czynna w godzinach od Tp do Tk. W momencie osiągnięcia czasu Tk na bramkach przestają działać karnety. Wszystkie osoby, które weszły na peron, mają zostać przetransportowane do stacji górnej. Następnie po 5 sekundach kolej ma zostać wyłączona.
- Dzieci w wieku od 4 do 8 lat siadają na krzesełko pod opieką osoby dorosłej.
- Osoba dorosła może opiekować się jednocześnie co najwyżej dwoma dziećmi w wieku od 4 do 8 lat.
- Każde przejście przez bramki (użycie danego karnetu) jest rejestrowane (id karnetu - godzina) – na koniec dnia jest generowany raport/podsumowanie ilości wykonanych zjazdów przez poszczególne osoby/karnety.
- Osoby uprawnione VIP wchodzą na peron dolnej stacji bez kolejki (używając karnetu).

Zgodnie z moją interpretacją, zrealizowałem symulację, w której jest:

- **Stacja narciarska** (w niej jest kasjer, turyści przychodzą i mogą zakupić bilet, aby zostać narciarzem, oraz narciarze przechodzą przez bramki).
- **Peron dolny/Kolejka do krzesełek** (w nim znajdują się narciarze po przejściu przez bramki i ma maksymalną pojemność, obsługuje go pracownik dol i jest to kolejka FIFO, z której narciarze „pakowani” są do krzesełka, a ono jest następnie wysyłane w drogę).
- **Krzesełka** (osobne byty – jest ich 80 i podróżują między peronem dolnym a górnym).
- **Peron górny** (zarządza nim pracownik góra, „wypakowuje” narciarzy i odsyła krzesełka, z niego narciarze wyjeżdżają w trasy).
- **Trasy zjazdu** (narciarze ją wybierają z peronu górnego i po przejechaniu jej wracają na stację narciarską).

Taka interpretacja pozwala zachować mi odpowiednią modularyzację oraz decentralizację projektu, przy zachowaniu jak największego realizmu funkcjonowania stacji.

---

## II. Komponenty

### 1. Podmioty

- **Init**: Inicjalizuje zasoby i odpowiada za uruchomienie procesów, bierze udział w zamykaniu stacji, usuwa zasoby.
- **Zegar**: Sygnalizuje rozpoczęcie i zakończenie pracy stacji, mierzy czas wewnętrzny symulacji.
- **Kasjer**: Obsługuje sprzedaż biletów.
- **Turysta**: Zwiedza stację, może kupić karnety od kasjera i zostać narciarzem.
- **Narciarz**: Przechodzi przez bramki, używa kolei narciarskiej i korzysta z tras.
- **Pracownik Dół**: Ładuje narciarzy na krzesełka i wysyła je w drogę na górę.
- **Pracownik Góra**: Czeka na krzesełka, rozładowuje narciarzy i odsyła je na dół.
- **Krzesełka**: Pokonują trasy góra-dół przewożąc narciarzy.
- **Generator Turystów**: Prosta pętla tworząca turystów co losowy przedział czasu (max wartość co jaką pojawia się turysta może być dostosowana w ustawieniach).

### 2. Funkcjonalność

- **Kolej linowa**: Symulacja działania 80 krzesełek (każde ma 3 miejsca) z maksymalnie 40 w ruchu jednocześnie (zakładam, że w ruchu oznacza 40 krzesełek jedzie w górę, 40 w dół, aby zapewnić system zamknięty w pracy krzesełek).
- **System biletowy**: Możliwość wyboru jednego z 5 biletów.
- **Kontrola wejścia**: Cztery bramki weryfikujące ważność karnetów z uwzględnieniem VIP wchodzącego bez kolejki.
- **Trasy**: Trzy zjazdy o różnych poziomach trudności, co przekłada się na czas zjazdu.

### 3. Obsługa błędów

- Korzystanie z funkcji `perror()` i zmiennej `errno` w przypadku błędów systemowych.

### 4. Komunikacja

- Wykorzystanie pamięci współdzielonej, semaforów oraz kolejek komunikatów do komunikacji między procesami.

---

## III. Struktura projektu

### 1. Dokumentacja/raport projektu

- [readme.md](readme.md)

### 2. Pliki źródłowe oraz deklaracje

- [common.h](blob/common.h) : Plik zawierający ustawienia oraz deklaracje.
- [init.cpp](init.cpp) : Plik zawierający implementację podmiotu init oraz zegara.
- [kasjer.cpp](kasjer.cpp) : Plik zawierający implementację podmiotu kasjer.
- [turysta.cpp](turysta.cpp) : Plik zawierający implementację podmiotu turysta.
- [narciarz.cpp](narciarz.cpp) : Plik zawierający implementację podmiotu narciarz.
- [pracownik_dol.cpp](pracownik_dol.cpp) : Plik zawierający implementację podmiotu pracownik\_dol.
- [pracownik_gora.cpp](pracownik_gora.cpp) : Plik zawierający implementację podmiotu pracownik\_gora.
- [krzeslo.cpp](krzeslo.cpp) : Plik zawierający implementację podmiotu krzesełko.

**(...)**



