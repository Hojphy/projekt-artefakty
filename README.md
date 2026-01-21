 # Archiwum Artefaktów (Galactic Artifact Archive)


Rok 2471. Ludzkość kontynuuje eksplorację galaktyki, natrafiając na pozostałości po dawno wymarłych lub wciąż aktywnych cywilizacjach obcych. Odkrywane artefakty nierzadko są spektakularne, czasem piękne, a czasem… niebezpieczne. W celu ich zabezpieczania oraz badania powstało Galaktyczne Archiwum Obcych Artefaktów, którego zadaniem jest katalogowanie wszystkich znalezisk przywiezionych z misji kosmicznych.


## 📋 Opis Projektu


Aplikacja konsolowa napisana w języku C, służąca do zarządzania bazą danych niebezpiecznych i cennych artefaktów. Program implementuje dynamiczne struktury danych (lista jednokierunkowa) oraz operacje na plikach binarnych, zapewniając trwałość danych między sesjami.


## 🚀 Możliwości programu

* **Baza danych:** Dodawanie, edycja i trwałe usuwanie artefaktów.
* **Wyszukiwanie:**
    * Po nazwie (wyszukiwanie fragmentu tekstu).
    * Po poziomie zagrożenia (filtrowanie liczbowe).
* **Sortowanie:** Możliwość sortowania po różnych kryteriach (Nazwa, Rok, Zagrożenie) w obu kierunkach (Rosnąco/Malejąco).
* **Zapis:** Automatyczny odczyt i zapis do pliku binarnego `.bin`.
* **Interfejs:** Przejrzysty widok archiwum artefaktów.


## 💻 Instalacja i Uruchomienie


### Wymagania

* Kompilator GCC (Linux) lub MinGW (Windows).


### Windows


#### Metoda Szybka (Zalecana):

1. Uruchom skrypt `compile.bat`, aby skompilować program.

2. Uruchom skrypt `start.bat`.

3. Skrypt zapyta o nazwę pliku bazy danych i uruchomi program automatycznie.


#### Metoda Ręczna:

1. Otwórz terminal (CMD lub PowerShell) w folderze projektu.

2. Skompiluj projekt poleceniem:

```cmd

gcc main.c interface.c artifacts.c -o start.exe

```

3. Uruchom program podając nazwę pliku bazy danych:

```cmd

.\start.exe baza.bin

```

### Linux / MacOS


1. Skompiluj program komendą:

```bash

make

```

Alternatywnie ręczna kompilacja:

```bash

gcc main.c interface.c artifacts.c -o start -Wall

```

2. Uruchom program:

```bash

./start baza.bin

```


## 📂 Struktura Plików


* `main.c` - Punkt wejścia programu, inicjalizacja i główna pętla.

* `interface.c / .h` - Warstwa prezentacji. Obsługa menu, danych użytkownika i wyświetlania tabel.

* `artifacts.c / .h` - Logika artefaktów. Operacje na strukturach, sortowanie, obsługa pamięci i plików.

* `Makefile` - Automatyzacja kompilacji. 
