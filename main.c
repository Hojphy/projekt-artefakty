#include <stdio.h>
#include <stdlib.h>
#include "artifacts.h"
#include "interface.h"
#include "utils.h"

int main(void) {
    char db_filename[40];
    printf("+---Galaktyczne Archiwum---+\n");
    printf("Podaj nazwe pliku bazy danych (np. baza.txt): ");
    fgets(db_filename, 40, stdin);
    while (getchar() != '\n');

    Archive archive;
    init_archive(&archive);

    if (load_from_file(&archive, db_filename)) {
        printf("Wczytano baze: %s\n", db_filename);
    } else {
        printf("Utworzono nowa baze o nazwie: %s\n", db_filename);
    }

    int running = 1;
    while (running) {
        // clear_console();
        show_menu();
        int choice = get_int_input();

        switch (choice) {
            case 1:
                break;
            case 2:
                print_archive(&archive);
                break;
            case 3:
                save_to_file(&archive, db_filename);
                running = 0;
                break;
            default:
                printf("Nieznana opcja.\n");
        }
        printf("Nacisnij Enter...");
        getchar();
    }

    free_archive(&archive);
    return 0;
}
