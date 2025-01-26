####I. Ogólne za³o¿enia projektu 

Projekt realizuj¹cy modularn¹ symulacje stacji narciarskiej w jêzyku C++ w œrodowisku WSL, 
umieszczony i udokumentowany w repozytorium GitHub 
https://github.com/XPiraniaX/SO_PROJEKT_016/ 

Temat 16 – Stacja narciarska Na terenie stacji narciarskiej znajduje siê z krzese³kowa kolej linowa. 
Kolej sk³ada siê z 3 osobowych krzese³ek o ³¹cznej liczbie 80 sztuk. Jednoczeœnie mo¿e byæ zajêtych 
40 krzese³ek na których siedzi maksymalnie 3x40 = 120 osób. Narciarze/turyœci przychodz¹ na teren 
stacji w losowych momentach czasu (nie wszyscy z nich musz¹ jeŸdziæ na nartach). Wejœcie na teren 
kolejki linowej odbywa siê po op³aceniu karnetu w kasie. Karnety s¹ czasowe (Tk1, Tk2, Tk3) lub 
dzienne. Dzieci poni¿ej 12 roku ¿ycia oraz seniorzy powy¿ej 65 roku ¿ycia maj¹ 25% zni¿kê. Dzieci 
poni¿ej 8 roku ¿ycia znajduj¹ siê pod sta³¹ opiek¹ osoby doros³ej. Wejœcie na peron dolnej stacji 
odbywa siê czterema bramkami jednoczeœnie. Na peronie dolnej stacji mo¿e przebywaæ maksymalnie 
N osób. Wyjazd z peronu stacji górnej odbywa siê dwoma drogami jednoczeœnie (ruch 
jednokierunkowy). Stacja dolna jest obs³ugiwana przez pracownika1, stacja górna jest obs³ugiwana 
przez pracownika2. W przypadku zagro¿enia pracownik1 lub pracownik2 zatrzymuj¹ kolej linow¹ 
(sygna³1). Aby wznowiæ dzia³anie pracownik, który zatrzyma³ kolej komunikuje siê z drugim 
pracownikiem – po otrzymaniu komunikatu zwrotnego o gotowoœci kolej jest uruchamiana ponownie 
(sygna³2). Zjazd odbywa siê trzema trasami o ró¿nym stopniu trudnoœci – œredni czas przejazdu dla 
poszczególnych tras jest ró¿ny i wynosi odpowiednio T1, T2 i T3 (T1<T2<T3).  
Zasady dzia³ania stacji ustalone przez kierownika s¹ nastêpuj¹ce:  
• Kolej linowa jest czynna w godzinach od Tp do Tk, W momencie osi¹gniêcia czasu Tk na bramkach 
przestaj¹ dzia³aæ karnety. Wszystkie osoby, które wesz³y na peron maj¹ zostaæ przetransportowane do 
stacji górnej. Nastêpnie po 5 sekundach kolej ma zostaæ wy³¹czona.  
• Dzieci w wieku od 4 do 8 lat siadaj¹ na krzese³ko pod opiek¹ osoby doros³ej;  
• Osoba doros³a mo¿e opiekowaæ siê jednoczeœnie co najwy¿ej dwoma dzieæmi w wieku od 4 do 8 lat;  
• Ka¿de przejœcie przez bramki (u¿ycie danego karnetu) jest rejestrowane (id karnetu - godzina) – na 
koniec dnia jest generowany raport/podsumowanie iloœci wykonanych zjazdów przez poszczególne 
osoby/karnety. 
• Osoby uprawnione VIP wchodz¹ na peron dolnej stacji bez kolejki (u¿ywaj¹c karnetu!); 

Zgodnie z moj¹ interpretacj¹, zrealizowa³em symulacje w której jest: 
Stacja narciarska(w niej jest kasjer, turyœci przychodz¹ i mog¹ zakupiæ bilet aby zostaæ narciarzem 
oraz narciarze przechodz¹ przez bramki) 
Peron dolny/Kolejka do krzese³ek(w nim znajduj¹ siê narciarze po przejœciu przez bramki i ma 
maksymaln¹ pojemnoœæ, obs³uguje go pracownik dol i jest to kolejka FiFo z której narciarze 
„pakowani” s¹ do krzese³ka a ono jest nastêpnie wysy³ane w drogê) 
Krzese³ka(osobne byty jest ich 80 i podró¿uj¹ miedzy peronem dolnym a górnym) 
Peron górny(zarz¹dza nim pracownik gora, „wypakowywuje” narciarzy i odsy³a krzese³ka, z niego 
narciarze wyje¿d¿aj¹ w trasy) 
Trasy zjazdu(narciarze j¹ wybieraj¹ z peronu górnego i po przejechaniu jej wracaj¹ na stacje 
narciarsk¹) 
Taka interpretacja pozwala zachowaæ mi odpowiedni¹ modularyzacje oraz decentralizacje projektu, 
przy zachowaniu jak najwiêkszego realizmu funkcjonowania stacji.   
3 
II. Komponenty 
1.Podmioty -Init : inicjalizuje zasoby i odpowiada za uruchomienie procesów, bierze udzia³ w 
zamykaniu stacji, usuwa zasoby  -Zegar : sygnalizuje rozpoczêcie i zakoñczenie pracy stacji, mierzy czas wewnêtrzny 
symulacji -Kasjer : obs³uguje sprzeda¿ biletów -Turysta : zwiedza stacje, mo¿e kupiæ karnety od kasjera i zostaæ narciarzem -Narciarz : przechodzi przez bramki, u¿ywa kolei narciarskiej i korzysta z tras -Pracownik Dó³ : ³aduje narciarzy na krzese³ka i wysy³a je w drogê na górê -Pracownik Góra: czeka na krzese³ka, roz³adowuje narciarzy i odsy³a je na dó³ -Krzese³ka: pokonuj¹ trasy góra-dó³ przewo¿¹c narciarzy  -Generator Turystów: prosta pêtla tworz¹ca turystów co losowy przedzia³ czasu(max 
wartoœæ co jak¹ pojawia siê turysta mo¿e byæ dostosowana w ustawieniach) 
2.Funkcjonalnoœæ -Kolej linowa : Symulacja dzia³ania 80 krzese³ek(ka¿de ma 3 miejsca) z maksymalnie 
40 w ruchu jednoczeœnie (zak³adam ¿e w ruchu oznacza 40 krzese³ek jedzie w górê, 
40 w dó³ aby zapewniæ system zamkniêty w pracy krzese³ek). -System biletowy : Mo¿liwoœæ wyboru jednego z 5 biletów -Kontrola wejœcia : Cztery bramki weryfikuj¹ce wa¿noœæ karnetów  z uwzglêdnieniem 
VIP wchodz¹cego bez kolejki -Trasy : Trzy zjazdy o ró¿nych poziomach trudnoœci co przek³ada siê na czas zjazdu 
3.Obs³uga b³êdów -Korzystanie z funkcji perror() i zmiennej errno w przypadku b³êdów systemowych  
4.Komunikacja -Wykorzystanie pamiêci wspó³dzielonej, semaforów oraz kolejek komunikatów do 
komunikacji miêdzy procesami 
4 
III. Struktura projektu 
1.README.md : dokumentacja/raport projektu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/README.md 
2.Pliki zród³owe oraz deklaracje: -common.h : plik zawieraj¹cy ustawienia oraz deklaracje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/common.h -init.cpp : plik zawieraj¹cy implementacje podmiotu init oraz zegara  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/init.cpp -kasjer.cpp : plik zawieraj¹cy implementacje podmiotu kasjer 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/kasjer.cpp -turysta.cpp : plik zawieraj¹cy implementacje podmiotu turysta 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/turysta.cpp -narciarz.cpp : plik zawieraj¹cy implementacje podmiotu narciarz 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/narciarz.cpp -pracownik_dol.cpp : plik zawieraj¹cy implementacje podmiotu pracownik_dol 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/pracownik_dol.cpp -pracownik_gora.cpp : plik zawieraj¹cy implementacje podmiotu pracownik_gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/pracownik_gora.cpp -krzeslo.cpp : plik zawieraj¹cy implementacje podmiotu krzese³ko 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/master/krzeslo.cpp 
5 
IV. Podstawowe dzia³anie 
systemu 
1.Kompilacja komend¹ make oraz uruchomienie symulacji ./start 
2.Inicjalizacja zasobów 
3.Uruchomienie procesów kasjer, pracownik_dol i pracownik_gora, 
krzese³ek, turystow, generatora turystow oraz w¹tku zegar 
4.Procesy inicjalizuj¹ zasoby oraz wykonuj¹ swoje zadania opisane w 
II.Komponenty->1.Podmioty 
5.W¹tek g³ówny oczekuje na zakoñczenie zegara po którym nastêpuje 
oznaczenie flagi koniec symulacji dla turystów i narciarzy(zamkniecie 
bramek) 
6.Wys³anie komunikatów do kasjera i pracownika_dol o zamkniêciu stacji 
(pracownik_dol oczekuje na opró¿nienie peronu i krzese³ek z narciarzy 
po czym wysy³a komunikat do pracownika_gora który koñczy dzia³anie i 
sam koñczy dzia³anie) 
7.Oczekiwanie na zakoñczenie pracownika_gora 
8.Wy³¹czenie krzese³ek  
9.Wyproszenie pozosta³ych turystów 
10.Wypisanie informacji o karnetach z bramek 
11.Zwolnienie zasobów i rozpoczêcie kolejnego dnia / zakoñczenie 
programu 
6 
V. Implementacja jednolitego 
systemu b³êdów programu i 
obs³uga errno 
1.B³êdy programu 
Gdy wyst¹pi b³¹d wystarczy skorzystaæ z blad(„komuniakt”), wtedy program koñczy 
dzia³anie i wyœwietlany jest stosowny komunikat 
a)Definicja 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L72 
b)Przyk³ad u¿ycia  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L72 
2.B³êdy errno 
a)Przyk³ad obs³ugi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L42 
7 
VI. Implementacja zarz¹dzania 
zasobami, komunikacji oraz 
synchronizacji procesów 
1.Klucze IPC 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L55 
b)Tworzenie niezbêdnych plików 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L89 
c)Tworzenie kluczy ipc 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L139 
2.Pamiêæ dzielona 
Elementy symulacji operuj¹ na  4 pamiêciach dzielonych: StacjaInfo, WyciagInfo, 
BramkiInfo i ZegarInfo 
a)Definicje pamiêci dzielonych(wraz z definicj¹ kolejki fifo 
potrzebnej do zarz¹dzania narciarzami na peronie) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L79 
b)Utworzenie pamiêci dzielonych 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L156 
c)Przyk³adowe do³¹czenie do pamiêci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L169   - w init 
8 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/narciarz.cpp#L103  - w narciarz  
d)Inicjalizacja pamiêci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L182 
e)Przyk³adowe od³¹czanie pamiêci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L407   - w init 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_gora.cpp#L119  - w pracownik_gora 
f)Zwolnienie pamiêci  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L390 
3.Semafory 
W projekcie operuje na 5 semaforach(z prefixem semId), Stacja,Brami i Wyci¹g 
Odpowiadaj¹ za dostêp do pamiêci o tej samej nazwie BramkiWejscie odpowiada za 
istnienie 4 bramek, Kasjer odpowiada za dostep do kasjera 
a)Uniwersalna implementacja systemu semaforów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L172 
b)Tworzenie i inicjalizacja semaforów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L198 
c)Przyk³adowe pod³¹czenie do semaforów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_dol.cpp#L27 
d)Przyk³adowe u¿ycie semaforów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/pracownik_dol.cpp#L47 
9 
e)Usuwanie semaforów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L397 
4.Kolejki komunikatów 
Procesy komunikuj¹ siê ze sob¹ przy pomocy kolejek: msgIdKasjer(Kasjer-Turysta), 
msgIdWyciag(Pracownicy-Krzese³ka), msgIdNarciarz(Pracownicy-Narciarze) 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L151 
b) Tworzenie kolejek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L229 
c)Przyk³ad do³¹czania do kolejki komunikatów 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L40  - w krzeslo 
d)Przyk³ad wys³ania i oczekiwania na komunikat 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/krzeslo.cpp#L65 
e)Usuniecie kolejek komunikatów  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L403 
5.Plik 
W projekcie u¿ywam jednego pliku jako bezpoœrednie Ÿród³o danych, logi.txt 
a)Definicja œcie¿ki 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/common.h#L68 
b)Przyk³ad U¿ycia 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/07bc999c439075ee1b72a361f
 514779e367a0b69/init.cpp#L50  - w funkcji odczytuj¹cej logi bramek 
10 
VII. Implementacja g³ównych 
funkcji projektu 
Przebieg ka¿dej funkcji wi¹¿e siê z wysy³aniem odpowiednich komunikatów na 
wyjœcie 
1.System biletowy 
a)Turysta po czasie zwiedzania stacji i decyzji o zostaniu 
narciarzem zajmuje kasjera i wysy³a komunikat z biletem jaki 
chce otrzymaæ 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L48 
b)Kasjer oczekuj¹cy na komunikat odbiera jego 
wiadomoœæ(mo¿e to byæ te¿ komunikat o zamkniêciu stacji) i 
wysy³a stosowny komunikat z biletem do turysty i oczekuje na 
kolejnego turyste 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L22 
c)Turysta otrzymuje bilet, odchodzi od kasjera(zwalnia kasê dla 
kolejnego turysty) i po czasie zostaje turyst¹ 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L103 
2.System bramek 
a)Narciarz wraz z dzieæmi pod opiek¹ (je¿eli takie posiada) po 
czasie podchodzi do jednej z 4 bramek (zajmuje semafor z 
wartoœci¹ 4 upewnia siê ¿e maksymalnie 4 narciarzy bêdzie 
przechodziæ przez bramki naraz) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L148 
11 
b)Bramki weryfikuj¹ wa¿noœæ jego biletu oraz miejsca na 
peronie, je¿eli warunki s¹ spe³nione to przepuszczaj¹ narciarza 
i rejestruj¹ u¿ycie karnetu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L173 
c)Narciarz przechodzi przez bramki i do³¹cza do kolejki do 
krzese³ek (je¿eli posiada karnet VIP bêdzie pierwszy w kolejce 
do krzese³ka wraz z dzieæmi) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L180 
3.System krzese³ek 
a)Pracownik dol sprawdza czy krzese³ka w trasie nie 
przekraczaj¹ 40, szuka wolnego krzese³ka i „³aduje” na niego 
narciarzy(je¿eli narciarz posiada dzieci pod opiek¹ to zawsze 
pojedzie ze swoimi dzieæmi nawet je¿eli wi¹¿e siê to z wolnym 
miejscem) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L59 
b)Pracownik dol wysyla komunikat o starcie do krzese³ka które 
wczeœniej „za³adowa³” narciarzami i wraca do punktu a) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L117 
c)Krzes³o które oczekuje na start odbiera komunikat i 
rozpoczyna drogê na górê, gdy tam dotrze wysy³a stosowny 
komunikat do pracownika gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/krzeslo.cpp#L50 
12 
d)Pracownik gora oczekuj¹cy na komunikat o dojechaniu 
krzese³ka otrzymuje go, „roz³adowywuje” narciarzy i wysy³a do 
nich komunikat powiadamiaj¹cy ich ¿e s¹ na górze, oraz wysy³a 
komunikat do krzese³ka o powrocie na dó³  i oczekuje na 
kolejne krzese³ko 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L47 
e)Krzese³ko po odebraniu komunikatu wraca na dó³ i znowu 
staje siê wolne 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/krzeslo.cpp#L75 
4.System zjazdu narciarza 
a)Narciarz po otrzymaniu komunikatu o wyjœciu z górnego 
peronu i wybraniu trasy zjazdu, zje¿d¿a i je¿eli stacja siê nie 
zamyka wraca na ni¹ i idzie do bramek  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L217 
5.System zamykania stacji 
a)Po up³ywie czasu dzia³ania stacji zegar zamyka bramki 
zmienia flagê dla narciarzy i turystów koniecSymulacji 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L18 – zegar 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L163 – zamkniecie bramek 
b)Po zakoñczeniu pracy zegara init wysy³a komunikat do 
kasjera i pracownika dol o zakoñczeniu pracy stacji oraz 
oczekuje na zamkniecie pracownika gora 
13 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L351 
c)Pracownik dol po otrzymaniu komunikatu o zamknieciu stacji 
wypuszcza tyle krzese³ek a¿ peron bêdzie pusty i wszyscy 
narciarze bêd¹ na górnym peronie 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L50 – odebranie komunikatu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L101 – wys³anie komunikatu do pracownika 
gora po opró¿nieniu peronu i krzese³ek 
d)Pracownik gora otrzymuje komunikat i koñczy pracê 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L37 
e)Po zakoñczeniu pracy krzese³ek nastêpuje wyproszenie 
pozosta³ych turystów i zamkniecie i wyœwietlenie raportu z 
bramek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L370 
14 
VIII. Implementacja pobocznych 
funkcji projektu 
1.Implementacja wyboru karnetów 
Zaimplementowa³em 5 rodzajów karnetów, na 3, 5, 10 zjazdów, dzienny oraz vip 
który jest biletem dziennym z przywilejem wchodzenia na krzese³ka bez kolejki na 
peronie  
a)Definicja  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L51 
b)Wybór karnetu z osobn¹ szans¹ na wybór karnetu vip (aby 
mo¿na by³o regulowaæ czêstotliwoœæ vipów) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L65 
2.Implementacja zni¿ki 
a)Turysta losuje i podaje wiek podczas proœby o bilet(za³o¿y³em 
¿e na narty chodz¹ ludzie do 75 roku ¿ycia) 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/turysta.cpp#L66 
b)Kasjer wysy³a stosowny komunikat je¿eli zni¿ka zostanie 
przyznana  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L63 
15 
3.Implementacja dzieci pod opiek¹  
przyjmujê ¿e dzieci nie podchodz¹ do kasjera poniewa¿ robi to tylko rodzic oraz nie 
potrzebuj¹ karnetu aby wejœæ na peron dolny, jednak zajmuj¹ miejsce na peronie 
oraz na krzese³ku 
a)Losowanie czy narciarz posiada dzieci 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L148 
b)Przechodzenie z dzieæmi przez bramki  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L173 
c)Wsiadanie do krzese³ka z dzieæmi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L69 
4.Implementacja sygna³u o awarii oraz 
zatrzymanie krzese³ek 
zak³adam ¿e awaria mo¿e pojawiæ siê podczas za³adunku narciarzy 
a)Pracownik dol wyst¹pienie awarii, zatrzymanie krzese³ek oraz 
komunikat o awarii do pracownika gora 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L129 
b)Reakcja pracownika gora, naprawa awarii oraz potwierdzenie 
o naprawie do pracownika dol 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_gora.cpp#L54 
c)Oczekiwanie na potwierdzenie o naprawie od pracownika 
gora oraz wznowienie dzia³ania krzese³ek 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L149 
16 
5.Implementacja ró¿nych tras zjazdu 
a)Definicja 3 tras o ró¿nym czasie zjadu 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L49 
b)Wybór trasy zajdu przez narciarza  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L224 
6.Zapisywanie informacji o godzinie narciarzu i 
karnecie przez bramki i wyœwietlenie ich na 
koniec 
Po ka¿dorazowym przejœciu przez bramki, dane zapisywane s¹ do pliku logi z 
którego na koniec s¹ odczytywane  
a)Zapis do pliku logi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L215 – rejestracja przejœcia 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L3  - funkcja zapisuj¹ca 
b)Odczyt z pliku logi 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L387  - moment wypisania  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L47  - funkcja wypisuj¹ca  
17 
IX. Elementy specjalne 
1.Kolorowe komunikaty, podzielone kolorami 
wed³ug czêœci systemu który go wysy³a: -Systemu Kasowego – niebieski 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L55 -Systemu Krzese³ek – zielony 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L114 -Systemu Bramek – szary 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L208 -Systemu Zjazdu – Magenta 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/narciarz.cpp#L250 -Zegara  – Czerwony 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L21 -Wszystkie komunikaty zwi¹zane z vipem maj¹ ¿ó³ty tag [V.I.P] 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/kasjer.cpp#L58 -Komunikaty o awarii maj¹ czerwony tag [AWARIA] 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/pracownik_dol.cpp#L159 
18 
2.Dodanie komunikatów o bie¿¹cej godzinie 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L35 
3.Obs³uga sygna³u ctrl + C 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L8 
4.Implementacja ustawien stacji które 
pozwalaj¹ na dostosowanie symulacji do 
swoich potrzeb 
a)Definicje  
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/common.h#L27 
b)Walidacja 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L68 
19 
X. Przyk³ady u¿ycia funkcji 
systemowych 
1.Tworzenie i obs³uga plików 
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
2.Tworzenie procesów 
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
3.Tworzenie i obs³uga w¹tków 
a)timerThread() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L252 
b)this_thread::sleep_for() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L29 
4.Ob³uga sygna³ów 
a)kill() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L375 
b)signal() 
https://github.com/XPiraniaX/SO_PROJEKT_016/blob/e79bc99caf6dd04f61a220203
 908e180d4f1d670/init.cpp#L244 
5.Synchronizacja procesów 
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
6.Segmenty pamiêci dzielonej 
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
7.Kolejki komunikatów 
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
Test 1: Przekroczenie limitu osób na peronie 
dolnej stacji 
Sprawdzenie, czy system prawid³owo blokuje wejœcie na peron dolnej stacji, gdy 
liczba osób osi¹gnie maksymalny limit  
Narciarze przekraczaj¹cy pojemnoœæ peronu nie przechodz¹ przez bramki 
23 
Test 2: Obs³uga VIP 
Weryfikacja, czy osoby z uprawnieniem VIP przechodz¹ na peron bez kolejki, z 
uwzglêdnieniem priorytetu. 
Turysta po zakupie karnetu VIP przechodzi przez bramki i jako pierwszy wsiada do 
krzese³ka 
24 
Test 3: Wy³¹czenie kolejki po zakoñczeniu 
czasu pracy 
Sprawdzenie, czy system prawid³owo koñczy dzia³anie kolejki linowej po godzinie Tk 
Stacja prawid³owo zamyka siê po godzinie Tk 
25 
Test 4: Weryfikacja opieki dzieci na krzese³kach 
Sprawdzenie, czy dzieci w wieku 4–8 lat siadaj¹ na krzese³ko wy³¹cznie z doros³ym 
opiekunem. 
Narciarz zosta³ za³adowany na krzese³ko razem ze swoim dzieckiem 
Test 5: Obs³uga awarii kolejki linowej 
Sprawdzenie, czy system prawid³owo obs³uguje awariê kolejki i wznawia dzia³anie po 
komunikacie zwrotnym. 
Pracownicy po potwierdzeniu awarii stopuj¹ krzese³ka, i po potwierdzeniu naprawy 
wznawiaj¹ krzese³ka 
26 