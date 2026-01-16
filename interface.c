#include "interface.h"

void show_menu(int dir)
{
    char* direction = dir == ASCENDING_SORT ? "ROSNACY" : "MALEJACY";
    printf("\n+========== SYSTEM ZARZADZANIA ARTEFAKTAMI ==========+\n");
    printf("| 1. %-47s |\n", "Dodaj nowy artefakt");
    printf("| 2. %-47s |\n", "Edytuj istniejacy artefakt");
    printf("| 3. %-47s |\n", "Usun artefakt z bazy");
    printf("| %50s |\n", "--------------------------------------------------");
    printf("| 4. %-47s |\n", "Wyswietl wszystkie artefakty (Tabela)");
    printf("| 5. %-47s |\n", "Wyszukaj po nazwie (Po fragmencie nazwy)");
    printf("| 6. %-47s |\n", "Filtruj po poziomie zagrozenia");
    printf("| %50s |\n", "--------------------------------------------------");
    printf("| 7. %-33s%-8s%-5s |\n", "Posortuj baze danych (kierunek:   ", direction, "  )");
    printf("| 8. %-47s |\n", "Przelacz kierunek sortowania (Rosnaco/Malejaco)"); 
    printf("| %50s |\n", "--------------------------------------------------");
    printf("| 9. %-47s |\n", "Zapisz zmiany do pliku (bez wychodzenia)");
    printf("| 0. %-47s |\n", "Zapisz i wyjdz");
    printf("+====================================================+\n");
    printf("Twoj wybor: ");
    
}

void run_loop(Archive* archive, const char* db_filename)
{
    int running = 1;
    int current_mode = NAME_SORT;
    int current_direction = ASCENDING_SORT;
    while (running) 
    {
        clear_console();
        show_menu(current_direction);
        int choice = get_int_input();
        switch (choice) 
        {
            case MENU_ADD:
                handle_add_artifact(archive);

                wait_for_enter();
                break;
            case MENU_EDIT:
                handle_edit_artifact(archive);

                wait_for_enter();
                break;
            case MENU_REMOVE:
                handle_remove_artifact(archive);

                wait_for_enter();
                break;
            case MENU_PRINT_ALL:
                clear_console();
                print_archive(archive);
                wait_for_enter();
                break;
            case MENU_SEARCH_NAME:
            {
                char fragment[100];
                printf("Wprowadz fragment elementu: ");
                get_string_input(fragment, 100); 
                clear_console();
                print_artifacts_by_fragment(archive, fragment);
                
                wait_for_enter();
                break;
            }
            case MENU_SEARCH_THREAT:
            {
                printf("Wprowadz minimalny poziom zagrozenia: ");
                int level = get_int_input();
                clear_console();
                print_artifacts_by_threat(archive, level);
                
                wait_for_enter();
                break;
            }
            case MENU_SORT:
                current_mode = get_sort_criteria_from_user();
                sort_archive(archive, current_mode | current_direction);
                printf("Posortowano!\n");

                wait_for_enter();
                break;
            case MENU_TOGGLE_DIR:
                if (current_direction == ASCENDING_SORT) 
                {
                    current_direction = DESCENDING_SORT;
                    printf("Zmieniono na: MALEJACO\n");
                }
                else 
                {
                    current_direction = ASCENDING_SORT;
                    printf("Zmieniono na: ROSNACO\n");
                }
                sort_archive(archive, current_mode | current_direction);

                wait_for_enter();
                break;
            case MENU_SAVE:
                save_to_file(archive, db_filename);
                printf("Zapisano!\n");
                
                wait_for_enter();
                break;
            case MENU_EXIT:
                save_to_file(archive, db_filename);
                running = 0;
                break;
            default:
                printf("Nieznana opcja.\n");
        }
    }
}

static void clear_input_buffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void clear_console()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

static void wait_for_enter()
{
    printf("\n[Nacisnij ENTER, aby kontynuowac...]");
    clear_input_buffer();
}

static void get_string_input(char *buffer, int size) 
{
    if (fgets(buffer, size, stdin) != NULL) 
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') 
        {
            buffer[len-1] = '\0';
        }
    } 
    else 
    {
        clear_input_buffer();
    }
}

static int get_confirmation()
{
    int c = getchar();
    
    if (c == '\n') {
        return 0; 
    }

    int temp;
    while ((temp = getchar()) != '\n' && temp != EOF);

    if (c == 't' || c == 'T' || c == 'y' || c == 'Y') {
        return 1;
    }
    return 0;
}

static int get_int_input() 
{
    int value;
    while (scanf("%d", &value) != 1) 
    {
        printf("Wprowadz liczbe: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return value;
}

static void handle_add_artifact(Archive* arch)
{
    clear_console();
    Artifact new_art;

    printf("Podaj nazwe artefaktu: ");
    get_string_input(new_art.name, 100);

    printf("Podaj pochodzenie artefaktu: ");
    get_string_input(new_art.origin, 50);

    printf("Podaj nazwe cywilizacji artefaktu: ");
    get_string_input(new_art.creator_civilization, 50);

    printf("Podaj poziom zagrozenia artefaktu: ");
    new_art.threat_level = get_int_input();
    if (new_art.threat_level > 10) new_art.threat_level = 10;
    else if (new_art.threat_level < 0) new_art.threat_level = 0;

    printf("Podaj rok odkrycia artefaktu: ");
    new_art.discovery_year = get_int_input();
    

    handle_artifact_status(&new_art);

    if (add_artifact(arch, new_art)) {
        printf("\nSukces! Artefakt dodany.\n");
    } else {
        printf("\nBlad dodawania (brak pamieci?).\n");
    }
}

static void handle_edit_artifact(Archive *arch)
{
    clear_console();
    print_archive(arch);

    printf("\nPodaj DOKLADNA nazwe artefaktu do edycji: ");
    char target_name[100];
    get_string_input(target_name, 100);

    Artifact *found = find_artifact_by_name(arch, target_name);
    
    if (found == NULL) {
        printf("Nie znaleziono artefaktu '%s'!", target_name);
        return;
    }

    int loop = 1;
    
    while(loop)
    {
        clear_console();
        printf("\n--- WYBIERZ KRYTERIUM EDYCJI ARTEFAKTU (wybrano: '%s') ---\n", found->name);
        printf("1. Pochodzenie artefaktu (obecnie: %s)\n", found->origin);
        printf("2. Nazwa cywilizacji tworcow artefaktu (obecnie: %s)\n", found->creator_civilization);
        printf("3. Poziom zagrozenia artefaktu (obecnie: %d)\n", found->threat_level);
        printf("4. Rok odkrycia artefaktu (obecnie: %d)\n", found->discovery_year);
        printf("5. Zakoncz edycje artefaktu\n");
        printf("Wybor: ");
        
        switch(get_int_input())
        {
            case 1:
                printf("Wprowadz nowe pochodzenie: ");
                get_string_input(found->origin, 50);
                printf("Zmieniono pochodzenie na '%s'!\n", found->origin);
                break;
            case 2:
                printf("Wprowadz nowa nazwe cywilizacji tworcow: ");
                get_string_input(found->creator_civilization, 50);
                printf("Zmieniono nazwe cywilizacji tworcow na '%s'!\n", found->creator_civilization);
                break;
            case 3:
                printf("Wprowadz nowy poziom zagrozenia: ");
                found->threat_level = get_int_input();
                handle_artifact_status(found);
                printf("Zmieniono poziom zagrozenia na '%d'!\n", found->threat_level);
                break;
            case 4:
                printf("Wprowadz nowy rok odkrycia: ");
                found->discovery_year = get_int_input();
                printf("Zmieniono rok odkrycia na '%d'!\n", found->discovery_year);
                break;
            case 5:
                loop = 0;
                break;
            default:
                printf("Nieznana opcja!\n");
                break;

        }
    }
}

static void handle_remove_artifact(Archive *arch)
{
    
    int loop = 1;
    while(loop)
    {
        clear_console();
        print_archive(arch);
        printf("\nPodaj DOKLADNA nazwe artefaktu do usuniecia (lub '0' zeby anulowac): ");
        char target_name[100];
        get_string_input(target_name, 100);

        if (strcmp(target_name, "0") == 0) break;

        Artifact *found = find_artifact_by_name(arch, target_name);
        if (found == NULL)
        {
            printf("Nie znaleziono artefaktu '%s'!", target_name);
            continue;
        }

        printf("Czy na pewno chcesz usunac artefakt o nazwie: '%s'?\n[T/N] ", found->name);
        if (!get_confirmation()) break;
        int return_code = remove_artifact_by_name(arch, found->name);
        if (return_code == 1)
        {
            printf("Pomyslnie usunieto artefakt!\n");
            loop = 0;
        }
        else if (return_code == -1)
        {
            printf("Artefakt jest zbyt niebezpieczny, nie mozna go usunac!\n");
            loop = 0;
        }
        else
        {
            printf("Wystapil blad przy usuwaniu artefaktu!\n");
            loop = 0;
        }
    }
}

static int get_sort_criteria_from_user() 
{
    clear_console();
    printf("\n--- WYBIERZ KRYTERIUM SORTOWANIA BAZY ---\n");
    printf("1. Nazwa\n");
    printf("2. Pochodzenie\n");
    printf("3. Poziom zagrozenia\n");
    printf("4. Rok odkrycia\n");
    printf("5. Status\n");
    printf("Wybor: ");
    
    int c = get_int_input();
    
    switch(c) 
    {
        case 1: 
            return NAME_SORT;
        case 2: 
            return ORIGIN_SORT;
        case 3: 
            return THREAT_SORT;
        case 4: 
            return YEAR_SORT;
        case 5: 
            return STATUS_SORT;
        default: 
            printf("Nieprawidlowy wybor. Domyslnie: Nazwa.\n");
            return NAME_SORT;
    }
}
