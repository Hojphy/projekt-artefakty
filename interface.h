#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include "artifacts.h"

void show_menu();

void print_artifacts_by_fragment(const Archive *arch, const char *fragment);
void print_artifacts_by_threat(const Archive *arch, const int level);


#endif
