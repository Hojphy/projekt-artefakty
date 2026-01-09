#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include "artifacts.h"

void show_menu();
void run_loop(Archive* archive, const char* db_filename);
void clear_console();

void print_artifacts_by_fragment(const Archive *arch, const char *fragment);
void print_artifacts_by_threat(const Archive *arch, const int level);

static void clear_input_buffer();
static void get_string_input(char *buffer, int size);
static int get_int_input();

#endif
