#include "interface.h"

void show_menu()
{
    printf("+---Galaktyczne Archiwum---+\n");
    printf("1. Dodaj artefakt\n");
    printf("2. Wypisz zawartosc archiwum\n");
    printf("3. Zapisz i wyjdz\n");
    printf("+--------------------------+\n");
    
}

void run_loop(Archive* archive, const char* db_filename)
{
    int running = 1;
    while (running) 
    {
        // clear_console();
        show_menu();
        int choice = get_int_input();
        switch (choice) 
        {
            case 1:
                break;
            case 2:
                print_archive(archive);
                break;
            case 3:
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
