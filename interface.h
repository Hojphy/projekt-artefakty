#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include "artifacts.h"

enum MenuOption 
{
    MENU_EXIT = 0,
    MENU_ADD = 1,
    MENU_EDIT = 2,
    MENU_REMOVE = 3,
    MENU_PRINT_ALL = 4,
    MENU_SEARCH_NAME = 5,
    MENU_SEARCH_THREAT = 6,
    MENU_SORT = 7,
    MENU_TOGGLE_DIR = 8,
    MENU_SAVE = 9
};

void show_menu();
void run_loop(Archive* archive, const char* db_filename);

void print_artifacts_by_fragment(const Archive *arch, const char *fragment);
void print_artifacts_by_threat(const Archive *arch, const int level);

static void clear_input_buffer();
static void clear_console();
static void get_string_input(char *buffer, int size);
static int get_int_input();
static void wait_for_enter();
static int get_sort_criteria_from_user();

static void handle_add_artifact(Archive* arch);

#endif
