# I.Og�lne za�o�enia projektu

Projekt realizuje zdecentralizowan� i modularn� symulacje stacji narciarskiej w j�zyku C++



# II.Komponenty

### 1.Podmioty

-Kasjer : Obs�uguje sprzeda� bilet�w i weryfikuje karnety	-TODO

-Pracownik : Zarz�dzaj� dzia�aniem kolejki linowej oraz procedurami zatrzymania/wznowienia w razie zagro�enia -TODO

-Narciarz : Kupuje karnety, korzysta z bramek i zje�d�a trasami - TODO - rozwiniecie o ew mozliwosc wypozyczenia nart

### 2.Funkcjonalno��

-Kolej linowa : Symulacja dzia�ania 80 krzese�ek z maksymalnie 40 w ruchu jednocze�nie (120 os�b) 

-System biletowy : Obs�uguje bilety czasowe i dzienne, z uwzgl�dnieniem zni�ek dla dzieci poni�ej 12 lat i senior�w powy�ej 65 lat, z mo�liwo�ci� wypo�yczenia sprz�tu -TODO

-Kontrola wej�cia : Cztery bramki weryfikuj�ce wa�no�� karnet�w w czasie rzeczywistym -TODO

-Trasy : Trzy zjazdy o r�nych poziomach trudno�ci i �rednich czasach przejazdu - TODO

-Wej�cie VIP : Priorytetowy dost�p do kolejki linowej (z wa�nym karnetem) -TODO

### 3.B��dy

-Korzystanie z funkcji perror() i zmiennej errno w przypadku b��d�w systemowych (np. alokacja pami�ci, tworzenie semafor�w) -TODO

### 4.Komunikacja

-Wykorzystanie pami�ci wsp�dzielonej, semafor�w lub kolejek komunikat�w do komunikacji mi�dzy procesami

-Symulacja przybywania u�ytkownik�w i dzia�ania kolejki w czasie rzeczywistym.



# III.Struktura projektu

#### 1.README.md : Dokumentacja projektu

#### 2.Pliki zr�d�owe oraz deklaracje:

-main.cpp : plik g��wny

-narciarz.cpp i narciarz.h : obs�uga narciarza oraz deklaracje
	
-pracownik.cpp i pracownik.h : obs�uga pracownika oraz deklaracje	

-kasjer.cpp i kasjer.h : obs�uga kasjera oraz deklaracje	

-kolejka.cpp i kolejka.h : obs�uga kolejki oraz deklaracje	
