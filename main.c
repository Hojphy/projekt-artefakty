#include <stdio.h>
#include "artifacts.h"
#include "interface.h"

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        fprintf(stderr, "BLAD: Nie podano nazwy pliku bazy danych.\n");
        fprintf(stderr, "Uzycie: %s <plik_danych>\n", argv[0]);
        return 1;
    }
    const char *db_filename = argv[1];

    Archive archive;
    init_archive(&archive);

    Artifact test_art = {"zubrowka3", "bialystok", "podlasie", 1, 2020, "bezpieczny"};
    add_artifact(&archive, test_art);

    if (load_from_file(&archive, db_filename)) {
        printf("Wczytano baze: %s\n", db_filename);
    } else {
        printf("Utworzono nowa baze o nazwie: %s\n", db_filename);
    }
    
    run_loop(&archive, db_filename);

    free_archive(&archive);
    return 0;
}
