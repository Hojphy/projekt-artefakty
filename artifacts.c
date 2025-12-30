#include "artifacts.h"

void init_archive(Archive *arch) { }
int add_artefact(Archive *arch, Artifact new_art) { return 0; }

int save_to_file(const Archive *arch, const char *filename) { return 1; }
int load_from_file(Archive *arch, const char *filename) { return 1; }
void free_archive(Archive *arch) {}
