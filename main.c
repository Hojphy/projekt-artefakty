#include <stdio.h>
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

    Artifact test_art = {"zubrowka", "bialystok", "podlasie", 1, 2020, "bezpieczny"};
    add_artifact(&archive, test_art);

    if (load_from_file(&archive, db_filename)) {
        printf("Wczytano baze: %s\n", db_filename);
    } else {
        printf("Utworzono nowa baze o nazwie: %s\n", db_filename);
    }

    int running = 1;
    while (running) {
        // clear_console();
        show_menu();
        //int choice = get_int_input();
        int choice = 3;
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
        getchar();
    }

    free_archive(&archive);
    return 0;
}
