#include "artifacts.h"

void init_archive(Archive *arch) 
{ 
  arch->count = 0;
  arch->head = malloc(sizeof(Node));
}

void free_archive(Archive *arch)
{
  Node *current = arch->head;
  
  while (current != NULL) {
      Node *next_node = current->next;
      free(current);
      current = next_node;
  }

  arch->head = NULL;
  arch->count = 0;
}

int add_artifact(Archive *arch, Artifact new_art) 
{
  Node* new = calloc(1, sizeof(Node));
  new->data = new_art;
  new->next = arch->head;
  arch->head = new;
  return 1;
}

int save_to_file(const Archive *arch, const char *filename) 
{
  FILE *fp = fopen(filename, "wb");
  if (!fp) return 0;

  fwrite(&arch->count, sizeof(int), 1, fp);

  Node *current = arch->head;
  while (current != NULL) {
      fwrite(&current->data, sizeof(Artifact), 1, fp);
      current = current->next;
  }

  fclose(fp);
  return 1; 
}

int load_from_file(Archive *arch, const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (!fp) return 0;

  if (arch->head != NULL) {
      free_archive(arch);
  }

  int arch_size = 0;
  if (fread(&arch_size, sizeof(int), 1, fp) != 1) {
      fclose(fp);
      return 0;
  }

  Artifact temp_art;
  for (int i = 0; i < arch_size; i++) {
      if (fread(&temp_art, sizeof(Artifact), 1, fp) == 1) {
          add_artifact(arch, temp_art);
      }
  }

  fclose(fp);
  return 1;
}
