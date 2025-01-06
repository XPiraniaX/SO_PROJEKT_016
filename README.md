# I.Ogólne za³o¿enia projektu

Projekt realizuje zdecentralizowan¹ i modularn¹ symulacje stacji narciarskiej w jêzyku C++



# II.Komponenty

### 1.Podmioty

-Kasjer : Obs³uguje sprzeda¿ biletów i weryfikuje karnety	-TODO

-Pracownik : Zarz¹dzaj¹ dzia³aniem kolejki linowej oraz procedurami zatrzymania/wznowienia w razie zagro¿enia -TODO

-Narciarz : Kupuje karnety, korzysta z bramek i zje¿d¿a trasami - TODO - rozwiniecie o ew mozliwosc wypozyczenia nart

### 2.Funkcjonalnoœæ

-Kolej linowa : Symulacja dzia³ania 80 krzese³ek z maksymalnie 40 w ruchu jednoczeœnie (120 osób) 

-System biletowy : Obs³uguje bilety czasowe i dzienne, z uwzglêdnieniem zni¿ek dla dzieci poni¿ej 12 lat i seniorów powy¿ej 65 lat, z mo¿liwoœci¹ wypo¿yczenia sprzêtu -TODO

-Kontrola wejœcia : Cztery bramki weryfikuj¹ce wa¿noœæ karnetów w czasie rzeczywistym -TODO

-Trasy : Trzy zjazdy o ró¿nych poziomach trudnoœci i œrednich czasach przejazdu - TODO

-Wejœcie VIP : Priorytetowy dostêp do kolejki linowej (z wa¿nym karnetem) -TODO

### 3.B³êdy

-Korzystanie z funkcji perror() i zmiennej errno w przypadku b³êdów systemowych (np. alokacja pamiêci, tworzenie semaforów) -TODO

### 4.Komunikacja

-Wykorzystanie pamiêci wspó³dzielonej, semaforów lub kolejek komunikatów do komunikacji miêdzy procesami

-Symulacja przybywania u¿ytkowników i dzia³ania kolejki w czasie rzeczywistym.



# III.Struktura projektu

#### 1.README.md : Dokumentacja projektu

#### 2.Pliki zród³owe oraz deklaracje:

-main.cpp : plik g³ówny

-narciarz.cpp i narciarz.h : obs³uga narciarza oraz deklaracje
	
-pracownik.cpp i pracownik.h : obs³uga pracownika oraz deklaracje	

-kasjer.cpp i kasjer.h : obs³uga kasjera oraz deklaracje	

-kolejka.cpp i kolejka.h : obs³uga kolejki oraz deklaracje	
