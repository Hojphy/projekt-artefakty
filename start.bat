@echo off
title Galactic Archive Launcher
echo --- URUCHAMIANIE ARCHIWUM ---

set "name=baza.bin"

set /p "name=Wprowadz nazwe pliku (domyslnie: baza.bin): "

if not exist start.exe (
    echo BLAD: Nie znaleziono pliku start.exe!
    echo Najpierw skompiluj projekt komenda: gcc main.c interface.c artifacts.c -o start.exe
    pause
    exit /b
)

.\start.exe "%name%"

echo.
echo Program zakonczyl dzialanie.
pause