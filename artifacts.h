#ifndef ARTIFACTS_H
#define ARTIFACTS_H

#include "stdlib.h"

typedef struct {
    char name[100];
    char origin[50];
    char creator_civilization[50];
    int threat_level;
    int discovery_year;
    char status[30];
} Artifact;

typedef struct Node {
    Artifact data;
    struct Node *next;
} Node;

typedef struct {
  Node *head;
  int count;
} Archive;

void init_archive(Archive *arch);
void free_archive(Archive *arch);

int add_artifact(Archive *arch, Artifact new_art);
int remove_artifact_by_index(Archive *arch, int index);
int remove_artifacts_by_criteria(Archive *arch, const char *origin);

void sort_archive(Archive *arch, int mode);
Archive filter_archive(const Archive *arch, const char *query, int min_threat);

int save_to_file(const Archive *arch, const char *filename);
int load_from_file(Archive *arch, const char *filename);

#endif
