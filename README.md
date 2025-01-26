####I. Og�lne za�o�enia projektu 

Projekt realizuj�cy modularn� symulacje stacji narciarskiej w j�zyku C++ w �rodowisku WSL, 
umieszczony i udokumentowany w repozytorium GitHub 
https://github.com/XPiraniaX/SO_PROJEKT_016/ 

Temat 16 � Stacja narciarska Na terenie stacji narciarskiej znajduje si� z krzese�kowa kolej linowa. 
Kolej sk�ada si� z 3 osobowych krzese�ek o ��cznej liczbie 80 sztuk. Jednocze�nie mo�e by� zaj�tych 
40 krzese�ek na kt�rych siedzi maksymalnie 3x40 = 120 os�b. Narciarze/tury�ci przychodz� na teren 
stacji w losowych momentach czasu (nie wszyscy z nich musz� je�dzi� na nartach). Wej�cie na teren 
kolejki linowej odbywa si� po op�aceniu karnetu w kasie. Karnety s� czasowe (Tk1, Tk2, Tk3) lub 
dzienne. Dzieci poni�ej 12 roku �ycia oraz seniorzy powy�ej 65 roku �ycia maj� 25% zni�k�. Dzieci 
poni�ej 8 roku �ycia znajduj� si� pod sta�� opiek� osoby doros�ej. Wej�cie na peron dolnej stacji 
odbywa si� czterema bramkami jednocze�nie. Na peronie dolnej stacji mo�e przebywa� maksymalnie 
N os�b. Wyjazd z peronu stacji g�rnej odbywa si� dwoma drogami jednocze�nie (ruch 
jednokierunkowy). Stacja dolna jest obs�ugiwana przez pracownika1, stacja g�rna jest obs�ugiwana 
przez pracownika2. W przypadku zagro�enia pracownik1 lub pracownik2 zatrzymuj� kolej linow� 
(sygna�1). Aby wznowi� dzia�anie pracownik, kt�ry zatrzyma� kolej komunikuje si� z drugim 
pracownikiem � po otrzymaniu komunikatu zwrotnego o gotowo�ci kolej jest uruchamiana ponownie 
(sygna�2). Zjazd odbywa si� trzema trasami o r�nym stopniu trudno�ci � �redni czas przejazdu dla 
poszczeg�lnych tras jest r�ny i wynosi odpowiednio T1, T2 i T3 (T1<T2<T3).  
Zasady dzia�ania stacji ustalone przez kierownika s� nast�puj�ce:  
� Kolej linowa jest czynna w godzinach od Tp do Tk, W momencie osi�gni�cia czasu Tk na bramkach 
przestaj� dzia�a� karnety. Wszystkie osoby, kt�re wesz�y na peron maj� zosta� przetransportowane do 
stacji g�rnej. Nast�pnie po 5 sekundach kolej ma zosta� wy��czona.  
� Dzieci w wieku od 4 do 8 lat siadaj� na krzese�ko pod opiek� osoby doros�ej;  
� Osoba doros�a mo�e opiekowa� si� jednocze�nie co najwy�ej dwoma dzie�mi w wieku od 4 do 8 lat;  
� Ka�de przej�cie przez bramki (u�ycie danego karnetu) jest rejestrowane (id karnetu - godzina) � na 
koniec dnia jest generowany raport/podsumowanie ilo�ci wykonanych zjazd�w przez poszczeg�lne 
osoby/karnety. 
� Osoby uprawnione VIP wchodz� na peron dolnej stacji bez kolejki (u�ywaj�c karnetu!); 

Zgodnie z moj� interpretacj�, zrealizowa�em symulacje w kt�rej jest: 
Stacja narciarska(w niej jest kasjer, tury�ci przychodz� i mog� zakupi� bilet aby zosta� narciarzem 
oraz narciarze przechodz� przez bramki) 
Peron dolny/Kolejka do krzese�ek(w nim znajduj� si� narciarze po przej�ciu przez bramki i ma 
maksymaln� pojemno��, obs�uguje go pracownik dol i jest to kolejka FiFo z kt�rej narciarze 
�pakowani� s� do krzese�ka a ono jest nast�pnie wysy�ane w drog�) 
Krzese�ka(osobne byty jest ich 80 i podr�uj� miedzy peronem dolnym a g�rnym) 
Peron g�rny(zarz�dza nim pracownik gora, �wypakowywuje� narciarzy i odsy�a krzese�ka, z niego 
narciarze wyje�d�aj� w trasy) 
Trasy zjazdu(narciarze j� wybieraj� z peronu g�rnego i po przejechaniu jej wracaj� na stacje 
narciarsk�) 
Taka interpretacja pozwala zachowa� mi odpowiedni� modularyzacje oraz decentralizacje projektu, 
przy zachowaniu jak najwi�kszego realizmu funkcjonowania stacji.   
3 
II. Komponenty 
1.Podmioty -Init : inicjalizuje zasoby i odpowiada za uruchomienie proces�w, bierze udzia� w 
zamykaniu stacji, usuwa zasoby  -Zegar : sygnalizuje rozpocz�cie i zako�czenie pracy stacji, mierzy czas wewn�trzny 
symulacji -Kasjer : obs�uguje sprzeda� bilet�w -Turysta : zwiedza stacje, mo�e kupi� karnety od kasjera i zosta� narciarzem -Narciarz : przechodzi przez bramki, u�ywa kolei narciarskiej i korzysta z tras -Pracownik D� : �aduje narciarzy na krzese�ka i wysy�a je w drog� na g�r� -Pracownik G�ra: czeka na krzese�ka, roz�adowuje narciarzy i odsy�a je na d� -Krzese�ka: pokonuj� trasy g�ra-d� przewo��c narciarzy  -Generator Turyst�w: prosta p�tla tworz�ca turyst�w co losowy przedzia� czasu(max 
warto�� co jak� pojawia si� turysta mo�e by� dostosowana w ustawieniach) 
2.Funkcjonalno�� -Kolej linowa : Symulacja dzia�ania 80 krzese�ek(ka�de ma 3 miejsca) z maksymalnie 
40 w ruchu jednocze�nie (zak�adam �e w ruchu oznacza 40 krzese�ek jedzie w g�r�, 
40 w d� aby zapewni� system zamkni�ty w pracy krzese�ek). -System biletowy : Mo�liwo�� wyboru jednego z 5 bilet�w -Kontrola wej�cia : Cztery bramki weryfikuj�ce wa�no�� karnet�w  z uwzgl�dnieniem 
VIP wchodz�cego bez kolejki -Trasy : Trzy zjazdy o r�nych poziomach trudno�ci co przek�ada si� na czas zjazdu 
3.Obs�uga b��d�w -Korzystanie z funkcji perror() i zmiennej errno w przypadku b��d�w systemowych  
4.Komunikacja -Wykorzystanie pami�ci wsp�dzielonej, semafor�w oraz kolejek komunikat�w do 
komunikacji mi�dzy procesami 
4 
III. Struktura projektu 
1.README.md : dokumentacja/raport projektu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/README.md 
2.Pliki zr�d�owe oraz deklaracje: -common.h : plik zawieraj�cy ustawienia oraz deklaracje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/common.h -init.cpp : plik zawieraj�cy implementacje podmiotu init oraz zegara  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/init.cpp -kasjer.cpp : plik zawieraj�cy implementacje podmiotu kasjer 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/kasjer.cpp -turysta.cpp : plik zawieraj�cy implementacje podmiotu turysta 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/turysta.cpp -narciarz.cpp : plik zawieraj�cy implementacje podmiotu narciarz 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/narciarz.cpp -pracownik_dol.cpp : plik zawieraj�cy implementacje podmiotu pracownik_dol 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/pracownik_dol.cpp -pracownik_gora.cpp : plik zawieraj�cy implementacje podmiotu pracownik_gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/pracownik_gora.cpp -krzeslo.cpp : plik zawieraj�cy implementacje podmiotu krzese�ko 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/krzeslo.cpp 
5 
IV. Podstawowe dzia�anie 
systemu 
1.Kompilacja komend� make oraz uruchomienie symulacji ./start 
2.Inicjalizacja zasob�w 
3.Uruchomienie proces�w kasjer, pracownik_dol i pracownik_gora, 
krzese�ek, turystow, generatora turystow oraz w�tku zegar 
4.Procesy inicjalizuj� zasoby oraz wykonuj� swoje zadania opisane w 
II.Komponenty->1.Podmioty 
5.W�tek g��wny oczekuje na zako�czenie zegara po kt�rym nast�puje 
oznaczenie flagi koniec symulacji dla turyst�w i narciarzy(zamkniecie 
bramek) 
6.Wys�anie komunikat�w do kasjera i pracownika_dol o zamkni�ciu stacji 
(pracownik_dol oczekuje na opr�nienie peronu i krzese�ek z narciarzy 
po czym wysy�a komunikat do pracownika_gora kt�ry ko�czy dzia�anie i 
sam ko�czy dzia�anie) 
7.Oczekiwanie na zako�czenie pracownika_gora 
8.Wy��czenie krzese�ek  
9.Wyproszenie pozosta�ych turyst�w 
10.Wypisanie informacji o karnetach z bramek 
11.Zwolnienie zasob�w i rozpocz�cie kolejnego dnia / zako�czenie 
programu 
6 
V. Implementacja jednolitego 
systemu b��d�w programu i 
obs�uga errno 
1.B��dy programu 
Gdy wyst�pi b��d wystarczy skorzysta� z blad(�komuniakt�), wtedy program ko�czy 
dzia�anie i wy�wietlany jest stosowny komunikat 
a)Definicja 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L72 
b)Przyk�ad u�ycia  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L72 
2.B��dy errno 
a)Przyk�ad obs�ugi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L42 
7 
VI. Implementacja zarz�dzania 
zasobami, komunikacji oraz 
synchronizacji proces�w 
1.Klucze IPC 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L55 
b)Tworzenie niezb�dnych plik�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L89 
c)Tworzenie kluczy ipc 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L139 
2.Pami�� dzielona 
Elementy symulacji operuj� na  4 pami�ciach dzielonych: StacjaInfo, WyciagInfo, 
BramkiInfo i ZegarInfo 
a)Definicje pami�ci dzielonych(wraz z definicj� kolejki fifo 
potrzebnej do zarz�dzania narciarzami na peronie) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L79 
b)Utworzenie pami�ci dzielonych 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L156 
c)Przyk�adowe do��czenie do pami�ci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L169   - w init 
8 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/narciarz.cpp#L103  - w narciarz  
d)Inicjalizacja pami�ci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L182 
e)Przyk�adowe od��czanie pami�ci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L407   - w init 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_gora.cpp#L119  - w pracownik_gora 
f)Zwolnienie pami�ci  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L390 
3.Semafory 
W projekcie operuje na 5 semaforach(z prefixem semId), Stacja,Brami i Wyci�g 
Odpowiadaj� za dost�p do pami�ci o tej samej nazwie BramkiWejscie odpowiada za 
istnienie 4 bramek, Kasjer odpowiada za dostep do kasjera 
a)Uniwersalna implementacja systemu semafor�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L172 
b)Tworzenie i inicjalizacja semafor�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L198 
c)Przyk�adowe pod��czenie do semafor�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_dol.cpp#L27 
d)Przyk�adowe u�ycie semafor�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_dol.cpp#L47 
9 
e)Usuwanie semafor�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L397 
4.Kolejki komunikat�w 
Procesy komunikuj� si� ze sob� przy pomocy kolejek: msgIdKasjer(Kasjer-Turysta), 
msgIdWyciag(Pracownicy-Krzese�ka), msgIdNarciarz(Pracownicy-Narciarze) 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L151 
b) Tworzenie kolejek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L229 
c)Przyk�ad do��czania do kolejki komunikat�w 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L40  - w krzeslo 
d)Przyk�ad wys�ania i oczekiwania na komunikat 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L65 
e)Usuniecie kolejek komunikat�w  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L403 
5.Plik 
W projekcie u�ywam jednego pliku jako bezpo�rednie �r�d�o danych, logi.txt 
a)Definicja �cie�ki 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L68 
b)Przyk�ad U�ycia 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L50  - w funkcji odczytuj�cej logi bramek 
10 
VII. Implementacja g��wnych 
funkcji projektu 
Przebieg ka�dej funkcji wi��e si� z wysy�aniem odpowiednich komunikat�w na 
wyj�cie 
1.System biletowy 
a)Turysta po czasie zwiedzania stacji i decyzji o zostaniu 
narciarzem zajmuje kasjera i wysy�a komunikat z biletem jaki 
chce otrzyma� 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L48 
b)Kasjer oczekuj�cy na komunikat odbiera jego 
wiadomo��(mo�e to by� te� komunikat o zamkni�ciu stacji) i 
wysy�a stosowny komunikat z biletem do turysty i oczekuje na 
kolejnego turyste 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L22 
c)Turysta otrzymuje bilet, odchodzi od kasjera(zwalnia kas� dla 
kolejnego turysty) i po czasie zostaje turyst� 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L103 
2.System bramek 
a)Narciarz wraz z dzie�mi pod opiek� (je�eli takie posiada) po 
czasie podchodzi do jednej z 4 bramek (zajmuje semafor z 
warto�ci� 4 upewnia si� �e maksymalnie 4 narciarzy b�dzie 
przechodzi� przez bramki naraz) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L148 
11 
b)Bramki weryfikuj� wa�no�� jego biletu oraz miejsca na 
peronie, je�eli warunki s� spe�nione to przepuszczaj� narciarza 
i rejestruj� u�ycie karnetu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L173 
c)Narciarz przechodzi przez bramki i do��cza do kolejki do 
krzese�ek (je�eli posiada karnet VIP b�dzie pierwszy w kolejce 
do krzese�ka wraz z dzie�mi) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L180 
3.System krzese�ek 
a)Pracownik dol sprawdza czy krzese�ka w trasie nie 
przekraczaj� 40, szuka wolnego krzese�ka i ��aduje� na niego 
narciarzy(je�eli narciarz posiada dzieci pod opiek� to zawsze 
pojedzie ze swoimi dzie�mi nawet je�eli wi��e si� to z wolnym 
miejscem) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L59 
b)Pracownik dol wysyla komunikat o starcie do krzese�ka kt�re 
wcze�niej �za�adowa�� narciarzami i wraca do punktu a) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L117 
c)Krzes�o kt�re oczekuje na start odbiera komunikat i 
rozpoczyna drog� na g�r�, gdy tam dotrze wysy�a stosowny 
komunikat do pracownika gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/krzeslo.cpp#L50 
12 
d)Pracownik gora oczekuj�cy na komunikat o dojechaniu 
krzese�ka otrzymuje go, �roz�adowywuje� narciarzy i wysy�a do 
nich komunikat powiadamiaj�cy ich �e s� na g�rze, oraz wysy�a 
komunikat do krzese�ka o powrocie na d�  i oczekuje na 
kolejne krzese�ko 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L47 
e)Krzese�ko po odebraniu komunikatu wraca na d� i znowu 
staje si� wolne 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/krzeslo.cpp#L75 
4.System zjazdu narciarza 
a)Narciarz po otrzymaniu komunikatu o wyj�ciu z g�rnego 
peronu i wybraniu trasy zjazdu, zje�d�a i je�eli stacja si� nie 
zamyka wraca na ni� i idzie do bramek  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L217 
5.System zamykania stacji 
a)Po up�ywie czasu dzia�ania stacji zegar zamyka bramki 
zmienia flag� dla narciarzy i turyst�w koniecSymulacji 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L18 � zegar 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L163 � zamkniecie bramek 
b)Po zako�czeniu pracy zegara init wysy�a komunikat do 
kasjera i pracownika dol o zako�czeniu pracy stacji oraz 
oczekuje na zamkniecie pracownika gora 
13 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L351 
c)Pracownik dol po otrzymaniu komunikatu o zamknieciu stacji 
wypuszcza tyle krzese�ek a� peron b�dzie pusty i wszyscy 
narciarze b�d� na g�rnym peronie 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L50 � odebranie komunikatu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L101 � wys�anie komunikatu do pracownika 
gora po opr�nieniu peronu i krzese�ek 
d)Pracownik gora otrzymuje komunikat i ko�czy prac� 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L37 
e)Po zako�czeniu pracy krzese�ek nast�puje wyproszenie 
pozosta�ych turyst�w i zamkniecie i wy�wietlenie raportu z 
bramek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L370 
14 
VIII. Implementacja pobocznych 
funkcji projektu 
1.Implementacja wyboru karnet�w 
Zaimplementowa�em 5 rodzaj�w karnet�w, na 3, 5, 10 zjazd�w, dzienny oraz vip 
kt�ry jest biletem dziennym z przywilejem wchodzenia na krzese�ka bez kolejki na 
peronie  
a)Definicja  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L51 
b)Wyb�r karnetu z osobn� szans� na wyb�r karnetu vip (aby 
mo�na by�o regulowa� cz�stotliwo�� vip�w) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L65 
2.Implementacja zni�ki 
a)Turysta losuje i podaje wiek podczas pro�by o bilet(za�o�y�em 
�e na narty chodz� ludzie do 75 roku �ycia) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L66 
b)Kasjer wysy�a stosowny komunikat je�eli zni�ka zostanie 
przyznana  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L63 
15 
3.Implementacja dzieci pod opiek�  
przyjmuj� �e dzieci nie podchodz� do kasjera poniewa� robi to tylko rodzic oraz nie 
potrzebuj� karnetu aby wej�� na peron dolny, jednak zajmuj� miejsce na peronie 
oraz na krzese�ku 
a)Losowanie czy narciarz posiada dzieci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L148 
b)Przechodzenie z dzie�mi przez bramki  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L173 
c)Wsiadanie do krzese�ka z dzie�mi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L69 
4.Implementacja sygna�u o awarii oraz 
zatrzymanie krzese�ek 
zak�adam �e awaria mo�e pojawi� si� podczas za�adunku narciarzy 
a)Pracownik dol wyst�pienie awarii, zatrzymanie krzese�ek oraz 
komunikat o awarii do pracownika gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L129 
b)Reakcja pracownika gora, naprawa awarii oraz potwierdzenie 
o naprawie do pracownika dol 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L54 
c)Oczekiwanie na potwierdzenie o naprawie od pracownika 
gora oraz wznowienie dzia�ania krzese�ek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L149 
16 
5.Implementacja r�nych tras zjazdu 
a)Definicja 3 tras o r�nym czasie zjadu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L49 
b)Wyb�r trasy zajdu przez narciarza  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L224 
6.Zapisywanie informacji o godzinie narciarzu i 
karnecie przez bramki i wy�wietlenie ich na 
koniec 
Po ka�dorazowym przej�ciu przez bramki, dane zapisywane s� do pliku logi z 
kt�rego na koniec s� odczytywane  
a)Zapis do pliku logi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L215 � rejestracja przej�cia 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L3  - funkcja zapisuj�ca 
b)Odczyt z pliku logi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L387  - moment wypisania  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L47  - funkcja wypisuj�ca  
17 
IX. Elementy specjalne 
1.Kolorowe komunikaty, podzielone kolorami 
wed�ug cz�ci systemu kt�ry go wysy�a: -Systemu Kasowego � niebieski 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L55 -Systemu Krzese�ek � zielony 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L114 -Systemu Bramek � szary 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L208 -Systemu Zjazdu � Magenta 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L250 -Zegara  � Czerwony 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L21 -Wszystkie komunikaty zwi�zane z vipem maj� ��ty tag [V.I.P] 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L58 -Komunikaty o awarii maj� czerwony tag [AWARIA] 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L159 
18 
2.Dodanie komunikat�w o bie��cej godzinie 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L35 
3.Obs�uga sygna�u ctrl + C 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L8 
4.Implementacja ustawien stacji kt�re 
pozwalaj� na dostosowanie symulacji do 
swoich potrzeb 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L27 
b)Walidacja 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L68 
19 
X. Przyk�ady u�ycia funkcji 
systemowych 
1.Tworzenie i obs�uga plik�w 
a)open() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L90 
b)close() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L71 
c)read() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L57 
d)write() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L67 
2.Tworzenie proces�w 
a)fork 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L258() 
b)exit() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L76 
c)wait() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L371 
20 
3.Tworzenie i obs�uga w�tk�w 
a)timerThread() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L252 
b)this_thread::sleep_for() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L29 
4.Ob�uga sygna��w 
a)kill() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L375 
b)signal() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L244 
5.Synchronizacja proces�w 
a)ftok() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L140 
b)semget() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L121 
c)semctl() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L397 
d)semop() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L185 
21 
6.Segmenty pami�ci dzielonej 
a)ftok() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L13 
b)shmget() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L22 
c)shmat() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L24 
d)shmdt() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L178 
e)shmctl()  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L395 
7.Kolejki komunikat�w 
a)ftok() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L10 
b)msgget() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L12 
c)msgsnd() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L51 
d)msgrcv() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L24 
e)msgctl() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L405 
22 
XI. Testy 
Test 1: Przekroczenie limitu os�b na peronie 
dolnej stacji 
Sprawdzenie, czy system prawid�owo blokuje wej�cie na peron dolnej stacji, gdy 
liczba os�b osi�gnie maksymalny limit  
Narciarze przekraczaj�cy pojemno�� peronu nie przechodz� przez bramki 
23 
Test 2: Obs�uga VIP 
Weryfikacja, czy osoby z uprawnieniem VIP przechodz� na peron bez kolejki, z 
uwzgl�dnieniem priorytetu. 
Turysta po zakupie karnetu VIP przechodzi przez bramki i jako pierwszy wsiada do 
krzese�ka 
24 
Test 3: Wy��czenie kolejki po zako�czeniu 
czasu pracy 
Sprawdzenie, czy system prawid�owo ko�czy dzia�anie kolejki linowej po godzinie Tk 
Stacja prawid�owo zamyka si� po godzinie Tk 
25 
Test 4: Weryfikacja opieki dzieci na krzese�kach 
Sprawdzenie, czy dzieci w wieku 4�8 lat siadaj� na krzese�ko wy��cznie z doros�ym 
opiekunem. 
Narciarz zosta� za�adowany na krzese�ko razem ze swoim dzieckiem 
Test 5: Obs�uga awarii kolejki linowej 
Sprawdzenie, czy system prawid�owo obs�uguje awari� kolejki i wznawia dzia�anie po 
komunikacie zwrotnym. 
Pracownicy po potwierdzeniu awarii stopuj� krzese�ka, i po potwierdzeniu naprawy 
wznawiaj� krzese�ka 
26 