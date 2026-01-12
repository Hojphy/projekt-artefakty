#ifndef ARTIFACTS_H
#define ARTIFACTS_H

#define NAME_SORT 0x01
#define ORIGIN_SORT 0x02
#define CREATOR_SORT 0x03
#define THREAT_SORT 0x04
#define YEAR_SORT 0x05
#define STATUS_SORT 0x06

#define ASCENDING_SORT 0x00
#define DESCENDING_SORT 0x10

#define SORT_MASK 0x0F

#include "stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "errno.h"

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
int edit_artifact(Archive *arch, const char* name, Artifact new_art);
int remove_artifact_by_name(Archive *arch, const char *name);
Artifact* find_artifact_by_name(Archive *arch, const char *name);

void handle_artifact_status(Artifact* art);

Archive* sort_archive(Archive* arch, int mode);
Node* insert_node(Node* sorted_head, Node* new_node, int mode);

int save_to_file(const Archive *arch, const char *filename);
int load_from_file(Archive *arch, const char *filename);

void print_table_header();
void print_table_row(Artifact *a);
void print_archive(const Archive *arch);
void print_artifacts_by_fragment(const Archive *arch, const char *fragment);
void print_artifacts_by_threat(const Archive *arch, const int min_level);

#endif
