# Dokumentacja
Projektu SO Temat 
16 Stacja Narciarska
Kamil Gębala 151868

---

## Spis treści

I. [Ogólne założenia projektu](#i-ogólne-założenia-projektu)

II. [Komponenty](#ii-komponenty)

III. [Struktura projektu](#iii-struktura-projektu)

IV. [Podstawowe działanie systemu](#iv-podstawowe-działanie-systemu)

V. [Implementacja jednolitego systemu błędów programu i obsługa errno](#v-implementacja-jednolitego-systemu-błędów-programu-i-obsługa-errno)

VI. [Implementacja zarządzania zasobami, komunikacji oraz synchronizacji procesów](#vi-implementacja-zarządzania-zasobami-komunikacji-oraz-synchronizacji-procesów)

VII. [Implementacja głównych funkcji projektu](#vii-Implementacja-głównych-funkcji-projektu)

VIII. [Implementacja pobocznych funkcji projektu](#viii-implementacja-pobocznych-funkcji-projektu)

IX. [Elementy specjalne](#ix-elementy-specjalne) 

X. [Przykłady użycia funkcji systemowych](#x-przykłady-użycia-funkcji-systemowych) 

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

- [readme.md](README.md)

### 2. Pliki źródłowe oraz deklaracje

- [common.h](common.h) : Plik zawierający ustawienia oraz deklaracje.
- [init.cpp](init.cpp) : Plik zawierający implementację podmiotu init oraz zegara.
- [kasjer.cpp](kasjer.cpp) : Plik zawierający implementację podmiotu kasjer.
- [turysta.cpp](turysta.cpp) : Plik zawierający implementację podmiotu turysta.
- [narciarz.cpp](narciarz.cpp) : Plik zawierający implementację podmiotu narciarz.
- [pracownik_dol.cpp](pracownik_dol.cpp) : Plik zawierający implementację podmiotu pracownik\_dol.
- [pracownik_gora.cpp](pracownik_gora.cpp) : Plik zawierający implementację podmiotu pracownik\_gora.
- [krzeslo.cpp](krzeslo.cpp) : Plik zawierający implementację podmiotu krzesełko.

---

## IV. Podstawowe działanie systemu

1.Kompilacja komendą make oraz uruchomienie symulacji ./start

2.Inicjalizacja zasobów

3.Uruchomienie procesów kasjer, pracownik_dol i pracownik_gora, krzesełek, turystow, generatora turystow oraz wątku zegar

4.Procesy inicjalizują zasoby oraz wykonują swoje zadania opisane w II.Komponenty->1.Podmioty

5.Wątek główny oczekuje na zakończenie zegara po którym następuje oznaczenie flagi koniec symulacji dla turystów i narciarzy(zamkniecie bramek)

6.Wysłanie komunikatów do kasjera i pracownika_dol o zamknięciu stacji (pracownik_dol oczekuje na opróżnienie peronu i krzesełek z narciarzy po czym wysyła komunikat do pracownika_gora który kończy działanie i sam kończy działanie)

7.Oczekiwanie na zakończenie pracownika_gora

8.Wyłączenie krzesełek 

9.Wyproszenie pozostałych turystów

10.Wypisanie informacji o karnetach z bramek

11.Zwolnienie zasobów i rozpoczęcie kolejnego dnia / zakończenie programu

---

## V. Implementacja jednolitego systemu błędów programu i obsługa errno 

### 1.Błędy programu
Gdy wystąpi błąd wystarczy skorzystać z blad(„komuniakt”), wtedy program kończy działanie i wyświetlany jest stosowny komunikat

a)[Definicja](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f514779e367a0b69/common.h#L72)

b)[Przykład użycia](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f514779e367a0b69/krzeslo.cpp#L72)

### 2.Błędy errno

a)[Przykład obsługi](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_gora.cpp#L42)

---

## VI. Implementacja zarządzania zasobami, komunikacji oraz synchronizacji procesów 

### 1.Klucze IPC

a)[Definicje](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/common.h#L55)

b)[Tworzenie niezbędnych plików](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L99)

c)[Tworzenie kluczy ipc](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L135)

### 2.Pamięć dzielona

Elementy symulacji operują na  4 pamięciach dzielonych: StacjaInfo, WyciagInfo, BramkiInfo i ZegarInfo

a)[Definicje pamięci dzielonych](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/common.h#L79) (wraz z definicją kolejki fifo potrzebnej do zarządzania narciarzami na peronie)

b)[Utworzenie pamięci dzielonych](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L152)

c)[Przykładowe dołączenie do pamięci](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L165)

d)[Inicjalizacja pamięci](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L178)

e)[Przykładowe odłączanie pamięci](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/turysta.cpp#L141)

f)[Zwolnienie pamięci](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L394)

### 3.Semafory

W projekcie operuje na 6 semaforach(z prefixem semId), Stacja,Brami i Wyciąg Odpowiadają za dostęp do pamięci o tej samej nazwie BramkiWejscie odpowiada za istnienie 4 bramek, Kasjer odpowiada za dostep do kasjera, a PeronWyjaz za kontolę 2 wyjść z peronu górnego

a)[Uniwersalna implementacja systemu semaforów V i P](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f514779e367a0b69/common.h#L172)

b)[Tworzenie i inicjalizacja semaforów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L194)

c)[Przykładowe podłączenie do semaforów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/pracownik_gora.cpp#L18)

d)[Przykładowe użycie semaforów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/narciarz.cpp#L163)

e)[Usuwanie semaforów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L399)

### 4.Kolejki komunikatów

Procesy komunikują się ze sobą przy pomocy kolejek: msgIdKasjer(Kasjer-Turysta), msgIdWyciag(Pracownicy-Krzesełka), msgIdNarciarz(Pracownicy-Narciarze)

a)[Definicje](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f514779e367a0b69/common.h#L151) 

b)[Tworzenie kolejek](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L231)

c)[Przykład dołączania do kolejki komunikatów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/kasjer.cpp#L12)

d)[Przykład wysłania i oczekiwania na komunikat](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/turysta.cpp#L102)

e)[Usuniecie kolejek komunikatów](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L406) 

### 5.Plik

W projekcie używam jednego pliku jako bezpośrednie źródło danych, logi.txt

a)[Definicja ścieżki](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f514779e367a0b69/common.h#L68)

b)[Przykład Użycia- w funkcji odczytującej logi bramek](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/b984c3b2960265b29e8ca6f41251044407038352/init.cpp#L46)

---

## VII. Implementacja głównych funkcji projektu

Przebieg każdej funkcji wiąże się z wysyłaniem odpowiednich komunikatów na wyjście

### 1.System biletowy

a)Turysta po czasie zwiedzania stacji i decyzji o zostaniu narciarzem zajmuje kasjera i wysyła komunikat z biletem jaki chce otrzymać - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/turysta.cpp#L60)

b)Kasjer oczekujący na komunikat odbiera jego wiadomość(może to być też komunikat o zamknięciu stacji) i wysyła stosowny komunikat z biletem do turysty i oczekuje na kolejnego turyste - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/kasjer.cpp#L22)

c)Turysta otrzymuje bilet, odchodzi od kasjera(zwalnia kasę dla kolejnego turysty) i po czasie zostaje turystą - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/turysta.cpp#L104)

### 2.System bramek

a)Narciarz wraz z dziećmi pod opieką (jeżeli takie posiada) po czasie podchodzi do jednej z 4 bramek (zajmuje semafor z wartością 4 upewnia się że maksymalnie 4 narciarzy będzie przechodzić przez bramki naraz) - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L188)

b)Bramki weryfikują ważność jego biletu oraz miejsca na peronie, jeżeli warunki są spełnione to przepuszczają narciarza i rejestrują użycie karnetu - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L192)

c)Narciarz przechodzi przez bramki i dołącza do kolejki do krzesełek (jeżeli posiada karnet VIP będzie pierwszy w kolejce do krzesełka wraz z dziećmi) - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L220)

### 3.System krzesełek

a)Pracownik dol sprawdza czy krzesełka w trasie nie przekraczają 40, szuka wolnego krzesełka i „ładuje” na niego narciarzy(jeżeli narciarz posiada dzieci pod opieką to zawsze pojedzie ze swoimi dziećmi nawet jeżeli wiąże się to z wolnym miejscem) - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L59)

b)Pracownik dol wysyla komunikat o starcie do krzesełka które wcześniej „załadował” narciarzami i wraca do punktu a) - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L117)

c)Krzesło które oczekuje na start odbiera komunikat i rozpoczyna drogę na górę, gdy tam dotrze wysyła stosowny komunikat do pracownika gora - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/krzeslo.cpp#L50)

d)Pracownik gora oczekujący na komunikat o dojechaniu krzesełka otrzymuje go, „rozładowywuje” narciarzy i wysyła do nich komunikat powiadamiający ich że są na górze, oraz wysyła komunikat do krzesełka o powrocie na dół  i oczekuje na kolejne krzesełko - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_gora.cpp#L47)

e)Krzesełko po odebraniu komunikatu wraca na dół i znowu staje się wolne - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/krzeslo.cpp#L75)

### 4.System zjazdu narciarza

a)Narciarz po otrzymaniu komunikatu o wyjściu z górnego peronu przez jedno z dwóch wyjść wybiera trasę, zjeżdża i jeżeli stacja się nie zamyka wraca na nią i idzie do bramek - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L263)

### 5.System zamykania stacji

a)Po upływie czasu działania stacji zegar zamyka bramki zmienia flagę dla narciarzy i turystów koniecSymulacji – link [zegar](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L18) / [zamkniecie bramek](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L192)

b)Po zakończeniu pracy zegara init wysyła komunikat do kasjera i pracownika dol o zakończeniu pracy stacji oraz oczekuje na zamkniecie pracownika gora - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L358)

c)Pracownik dol po otrzymaniu komunikatu o zamknieciu stacji wypuszcza tyle krzesełek aż peron będzie pusty i wszyscy narciarze będą na górnym peronie – link [odebranie komunikatu](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L50) / [wyslanie komunikatu do pracownika gora](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L101)

d)Pracownik gora otrzymuje komunikat i kończy pracę - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_gora.cpp#L36)

e)Po zakończeniu pracy krzesełek następuje wyproszenie pozostałych turystów, zamkniecie i wyświetlenie raportu z bramek - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L372)

---

## VIII. Implementacja pobocznych funkcji projektu

### 1.Implementacja wyboru karnetów

Zaimplementowałem 8 rodzajów karnetów, na 3, 5, 10 zjazdów, dzienny, na 2, 4, 6  godzin oraz vip który jest biletem dziennym z przywilejem wchodzenia na krzesełka bez kolejki na peronie 

a)[Definicja](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/82d5bfd9eda8118fba82b21a9d4a523f4106ec9f/common.h#L51) 

b)[Wybór karnetu](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/turysta.cpp#L64) z osobną szansą na wybór karnetu vip (aby można było regulować częstotliwość vipów)

### 2.Implementacja zniżki

a)Turysta losuje i podaje wiek podczas prośby o bilet(założyłem że na narty chodzą ludzie do 75 roku życia) - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/turysta.cpp#L66)

b)Kasjer wysyła stosowny komunikat jeżeli zniżka zostanie przyznana - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/kasjer.cpp#L66)

### 3.Implementacja dzieci pod opieką 

a)[Losowanie czy narciarz posiada dzieci pod opieką](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L173)

b)[Przechodzenie z dziećmi przez bramki](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L213) 

c)[Wsiadanie do krzesełka z dziećmi](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L71)

### 4.Implementacja sygnału o awarii oraz zatrzymanie krzesełek

a)Pracownik dol zgłasza wystąpienie awarii, zatrzymanie krzesełek oraz komunikat o awarii do pracownika gora - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L129)

b)Reakcja pracownika gora, naprawa awarii oraz potwierdzenie o naprawie do pracownika dol - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_gora.cpp#L54)

c)Oczekiwanie na potwierdzenie o naprawie od pracownika gora oraz wznowienie działania krzesełek - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L148)

### 5.Implementacja różnych tras zjazdu

a)Definicja 3 tras o różnym czasie zjadu - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/82d5bfd9eda8118fba82b21a9d4a523f4106ec9f/common.h#L49)

b)Wybór trasy zajdu przez narciarza - [link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L267)

### 6.Zapisywanie informacji o godzinie narciarzu i karnecie przez bramki i wyświetlenie ich na koniec

Po każdorazowym przejściu przez bramki, dane zapisywane są do pliku logi z którego na koniec są odczytywane 

a)Zapis do pliku logi – [link rejestracja przejścia](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L255) / [funkcja zapisujaca](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/narciarz.cpp#L6)

b)Odczyt z pliku logi – [moment wypisania](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L389) / [funkcja wypisujaca](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L43)

---

## IX. Elementy specjalne

### 1.Kolorowe komunikaty, podzielone kolorami według części systemu który go wysyła:

-[Systemu Kasowego – niebieski](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/kasjer.cpp#L55)
 
-[Systemu Krzesełek – zielony](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L114)

-[Systemu Bramek – szary](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/narciarz.cpp#L208)

-[Systemu Zjazdu – Magenta](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/narciarz.cpp#L250)

-[Zegara  – Czerwony](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L21)

-Wszystkie komunikaty związane z vipem mają żółty tag [V.I.P](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/kasjer.cpp#L58)

-Komunikaty o awarii mają czerwony tag [AWARIA](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/pracownik_dol.cpp#L137)

### 2.Dodanie komunikatów o bieżącej godzinie 
[link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L32)

### 3.Obsługa sygnału ctrl + C 
[link](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L8)

### 4.Implementacja ustawien stacji które pozwalają na dostosowanie symulacji do swoich potrzeb

a)[Definicje](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/82d5bfd9eda8118fba82b21a9d4a523f4106ec9f/common.h#L27)

b)[Walidacja](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/bb11266bd46ccb7a539af80e6828eb36268741df/init.cpp#L64)

---

## X. Przykłady użycia funkcji systemowych

1.Tworzenie i obsługa plików ([open()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L90), [close()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/narciarz.cpp#L71), [read()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L57), [write()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/narciarz.cpp#L67))

2.Tworzenie procesów ([fork()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L258()), [exit()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/common.h#L76), [wait()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L371))

3.Obługa sygnałów ([kill()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L375), [signal()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L244))

4.Synchronizacja procesów ([ftok()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L140), [semget()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/narciarz.cpp#L121), [semctl()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L397), [semop()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/common.h#L185))

5.Segmenty pamięci dzielonej ([ftok()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L13), [shmget()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L22), [shmat()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L24), [shmdt()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L24), [shmctl()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L395)) 

6.Kolejki komunikatów ([ftok()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/pracownik_dol.cpp#L10), [msgget()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/kasjer.cpp#L12), [msgsnd()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/kasjer.cpp#L51), [msgrcv()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/kasjer.cpp#L24), [msgctl()](https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203908e180d4f1d670/init.cpp#L405))




