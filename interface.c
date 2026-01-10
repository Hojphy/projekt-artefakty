#include "interface.h"

void show_menu()
{
    printf("\n+========== SYSTEM ZARZADZANIA ARTEFAKTAMI ==========+\n");
    printf("| 1. %-47s |\n", "Dodaj nowy artefakt");
    printf("| 2. %-47s |\n", "Edytuj istniejacy artefakt");
    printf("| 3. %-47s |\n", "Usun artefakt z bazy");
    printf("| %50s |\n", "--------------------------------------------------");
    printf("| 4. %-47s |\n", "Wyswietl wszystkie artefakty (Tabela)");
    printf("| 5. %-47s |\n", "Wyszukaj po nazwie (szukanie tekstu)");
    printf("| 6. %-47s |\n", "Filtruj po zagrozeniu (szukanie liczby)");
    printf("| %50s |\n", "--------------------------------------------------");
    printf("| 7. %-47s |\n", "Posortuj baze danych");
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
        show_menu();
        int choice = get_int_input();
        switch (choice) 
        {
            case MENU_ADD:
                handle_add_artifact(archive);

                wait_for_enter();
                break;
            case MENU_EDIT:
                break;
            case MENU_REMOVE:
                break;
            case MENU_PRINT_ALL:
                print_archive(archive);
                wait_for_enter();
                break;
            case MENU_SEARCH_NAME:
                break;
            case MENU_SEARCH_THREAT:
                break;
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
    
}

static int get_sort_criteria_from_user() 
{
    printf("\n--- WYBIERZ KRYTERIUM ---\n");
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
