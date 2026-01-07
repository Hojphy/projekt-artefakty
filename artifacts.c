#include "artifacts.h"

void init_archive(Archive *arch) 
{ 
    arch->count = 0;
    arch->head = malloc(sizeof(Node));
}

void free_archive(Archive *arch)
{
    Node *current = arch->head;
    
    while (current != NULL) 
    {
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
    if (new == NULL)
    {
        return 0;
    }
    new->data = new_art;
    new->next = arch->head;
    arch->head = new;
    ++arch->count;
    return 1;
}

int edit_artifact(Archive *arch, Artifact *art, Artifact new_art)
{
    Node *current = arch->head;
    while (current != NULL && strcmp(current->data.name, art->name) != 0)
    {
        current = current->next;
    }

    if (current == NULL) return 0;

    char original_name[100];
    strcpy(original_name, current->data.name);

    current->data = new_art;

    strcpy(current->data.name, original_name);
    return 1;
}

Artifact* find_artifact_by_name(Archive *arch, const char *name)
{
    Node *current = arch->head;
    while (current != NULL && strcmp(current->data.name, name) != 0)
    {
        current = current->next;
    }
    if (current == NULL) return NULL;
    return &current->data;
}

int remove_artifact_by_name(Archive *arch, const char *name)
{
    Node *current = arch->head;
    Node *prev = NULL;
    while (current != NULL && strcmp(current->data.name, name) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return 0;
    if (current->data.threat_level >= 8) return -1;

    if (prev == NULL)
    {
        arch->head = current->next;
    }
    else 
    {
        prev->next = current->next;
    }

    free(current);
    --arch->count;
    return 1;
}

int save_to_file(const Archive *arch, const char *filename) 
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    fwrite(&arch->count, sizeof(int), 1, fp);

    Node *current = arch->head;
    while (current != NULL) 
    {
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

    if (arch->head != NULL) 
    {
        free_archive(arch);
    }

    int arch_size = 0;
    if (fread(&arch_size, sizeof(int), 1, fp) != 1) 
    {
        fclose(fp);
        return 0;
    }

    Artifact temp_art;
    for (int i = 0; i < arch_size; i++)
    {
        if (fread(&temp_art, sizeof(Artifact), 1, fp) == 1) 
        {
            add_artifact(arch, temp_art);
        }
    }

    fclose(fp);
    return 1;
}

Node* insert_node(Node* sorted_head, Node* new_node, int mode) 
{
    if (sorted_head == NULL) 
    {
        new_node->next = NULL;
        return new_node;
    }

    int insert_on_start = 0;
    if (mode == NAME_SORT) 
    {
        if (strcmp(new_node->data.name, sorted_head->data.name) < 0) 
        {
            insert_on_start = 1;
        }
    } 
    else if (mode == THREAT_SORT) 
    {
        if (new_node->data.threat_level > sorted_head->data.threat_level) 
        {
            insert_on_start = 1;
        }
    }

    if (insert_on_start) 
    {
        new_node->next = sorted_head;
        return new_node;
    }

    Node* p = sorted_head;
    int found = 0;

    while (p->next != NULL && !found) 
    {
        int condition_met = 0;
        
        if (mode == NAME_SORT) 
        {
            if (strcmp(p->next->data.name, new_node->data.name) > 0) condition_met = 1;
        }
        else if (mode == THREAT_SORT) 
        {
            if (p->next->data.threat_level < new_node->data.threat_level) condition_met = 1;
        }

        if (condition_met)
        {
            found = 1;
        } 
        else 
        {
            p = p->next;
        }
    }
    new_node->next = p->next;
    p->next = new_node;

    return sorted_head;
}

Archive* sort_archive(Archive* arch, int mode) {
    if (arch->head == NULL) return arch;

    Node* sorted = NULL;
    Node* current = arch->head;

    while (current != NULL)
    {
        Node* next = current->next;
        current->next = NULL;

        sorted = insert_node(sorted, current, mode);

        current = next;
    }
    arch->head = sorted;
    return arch;
}
