#include "artifacts.h"

void init_archive(Archive *arch) 
{ 
  arch->count = 0;
  arch->head = malloc(sizeof(Node));
}
int add_artifact(Archive *arch, Artifact new_art) 
{
  Node* new = calloc(1, sizeof(Node));
  new->data = new_art;
  new->next = arch->head;
  arch->head = new;
  return 1;
}

int save_to_file(const Archive *arch, const char *filename) { return 1; }
int load_from_file(Archive *arch, const char *filename) { return 1; }
void free_archive(Archive *arch) {}
